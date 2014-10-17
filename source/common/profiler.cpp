//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>

XProfiler::XProfiler() :
	m_root(0),
	m_currentNode(0),
	m_samples(0),
	m_enabled(false),
	m_toggle(false)
{
}

XProfiler::~XProfiler()
{
	recursiveDelete(m_root);
}

void XProfiler::recursiveDelete(Node *node)
{
	for(map<string, Node*>::iterator itr = node->children.begin(); itr != node->children.end(); ++itr)
	{
		recursiveDelete(itr->second);
	}
	delete node;
}

void XProfiler::push(asIScriptContext *ctx)
{
	if(m_enabled && ctx)
	{
		// Get function
		asIScriptFunction *func = ctx->GetFunction();
		if(func)
		{
			Node *node;
			string decl = func->GetDeclaration();
			if(m_currentNode->children.find(decl) == m_currentNode->children.end())
			{
				// Create node if it doesn't exist
				node = new Node(decl);
				node->parent = m_currentNode;
				m_currentNode->children[decl] = node;
			}
			else
			{
				// Get node
				node = m_currentNode->children[decl];
			}

			// Store time and set node
			m_currentNode = node;
			m_currentNode->currentTime = chrono::high_resolution_clock::now();
		}
	}
}

void XProfiler::pop()
{
	if(m_enabled)
	{
		if(m_currentNode)
		{
			m_currentNode->durrations.push_back(chrono::high_resolution_clock::now() - m_currentNode->currentTime);
			m_currentNode = m_currentNode->parent;
		}
		else
		{
			LOG("XProfiler::pop() - No current node");
		}
	}
}

void XProfiler::sendStats(Node *node)
{
	// Calculate stuffs
	int total = 0, max = INT_MIN, min = INT_MAX;
	for(chrono::high_resolution_clock::duration durr : node->durrations)
	{
		int time = (int)chrono::duration<float, milli>(durr).count();
		if(time < min) min = time;
		if(time > max) max = time;
		total += time;
	}

	stringstream ss;
	ss << node->name; ss << ";";
	ss << total; ss << ";";
	ss << max; ss << ";";
	ss << min; ss << ";";
	ss << node->durrations.size();
	m_debugger->sendPacket(XD_PUSH_NODE_PACKET, ss.str());

	// Recursive call to children
	for(map<string, Node*>::iterator itr = node->children.begin(); itr != node->children.end(); ++itr)
	{
		sendStats(itr->second);
	}
	
	// Pop stack level
	m_debugger->sendPacket(XD_POP_NODE_PACKET);

	// Delete node
	delete node;
}

void XProfiler::enable()
{
	// Toggle if its not enabled
	if(!m_enabled)
	{
		m_toggle = true;
	}
}

void XProfiler::disable()
{
	// Toggle if its enabled
	if(m_enabled)
	{
		m_toggle = true;
	}
}

void XProfiler::stepBegin()
{
	assert(m_currentNode == m_root);
	
	if(m_root)
	{
		// Get step time
		m_root->durrations.push_back(chrono::high_resolution_clock::now() - m_root->currentTime);

		// Send stats
		sendStats(m_root);
	
		// Tell the profiler that a step was done
		if(m_debugger->sendPacket(XD_STEP_DONE_PACKET))
		{
			// Toggle the profiler
			if(m_toggle)
			{
				m_enabled = !m_enabled;
				m_toggle = false;
			}
		}
	}

	// Create new root node
	m_root = m_currentNode = new Node("Step");
	m_root->currentTime = chrono::high_resolution_clock::now();
}