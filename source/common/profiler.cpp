//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "engine.h"

XProfiler *XProfiler::s_this = 0;

AS_REG_SINGLETON(XProfiler)

int XProfiler::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	// Profiler
	r = scriptEngine->RegisterObjectMethod("XProfiler", "void start()", asMETHOD(XProfiler, start), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XProfiler", "void stop()", asMETHOD(XProfiler, stop), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XProfiler", "void push(const string &in)", asMETHOD(XProfiler, push), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XProfiler", "void pop()", asMETHOD(XProfiler, pop), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XProfiler", "void print()", asMETHOD(XProfiler, printResults), asCALL_THISCALL); AS_ASSERT

	return r;
}

XProfiler::XProfiler(XTimer *timer) :
	m_timer(timer),
	m_active(false),
	m_currentNode(0),
	m_root(0),
	m_rootName("")
{
	assert(timer != 0);
}

XProfiler::~XProfiler()
{
}

void XProfiler::start()
{
	// Clear eplaced times
	m_root = 0;
	m_profiles.clear();
	m_startTime = m_timer->getTime();
	m_active = true;
}

void XProfiler::stop()
{
	m_active = false;
	m_totalTime = m_timer->getTime()-m_startTime;
}

void XProfiler::push(const string &name)
{
	if(!m_active)
	{
		return;
	}
	
	Profile *p = &m_profiles[name];
	if(m_currentNode == 0)
	{
		// Set as root
		m_root = p;
		m_rootName = name;
		p->parent = 0;
	}
	else
	{
		p->parent = m_currentNode;
		m_currentNode->children[name] = p;
	}

	p->currentTime = m_timer->getTime();
	m_currentNode = p;
}

void XProfiler::pop()
{
	if(!m_active)
	{
		return;
	}

	if(!m_currentNode)
	{
		LOG("XProfiler::pop() - No current node");
		return;
	}

	m_currentNode->eplacedTimes.push_back(m_timer->getTime()-m_currentNode->currentTime);
	m_currentNode = m_currentNode->parent;
}


void XProfiler::printChildren(Profile *node, const string &name, const int depth, const float parentTotal, const float globalTotal)
{
	// Print current node info

	// Perform calulations
	node->eplacedTimes;
	float total = 0.0f, max = -1.0f, min = -1.0f;
	for(uint i = 0; i < node->eplacedTimes.size(); i++)
	{
		float time = node->eplacedTimes[i];
		if(time < min || min < 0.0f) min = time;
		if(time > max || max < 0.0f) max = time;
		total += node->eplacedTimes[i];
	}
	float ave = total/node->eplacedTimes.size();

	// Format print
	string indentString(depth, '-');

	// Format tree string
	char out[1024];
#ifdef USE_CTR_SECURE
	sprintf_s(out, "-%s%s", indentString.c_str(), name.c_str());
#elif
	sprintf(out, "-%s%s", indentString.c_str(), name.c_str());
#endif
	string treeString(out);
	if(treeString.size() > 30)
	{
		// String is too lagre for our format
		treeString.resize(27);
		treeString += "...";
	}else{
		treeString += string(30-treeString.size(), ' ');
	}

	// Format average
	//memset(out, 0, 1024);
#ifdef USE_CTR_SECURE
	sprintf_s(out, " %i ms", int(ave*1000));
#elif
	sprintf(out, " %i ms", int(ave*1000));
#endif
	string aveString(out);
	aveString += string(10-aveString.size(), ' ') + "|";

	// Format max
#ifdef USE_CTR_SECURE
	sprintf_s(out, " %i ms", int(max*1000));
#elif
	sprintf(out, " %i ms", int(max*1000));
#endif
	string maxString(out);
	maxString += string(10-maxString.size(), ' ') + "|";

	// Format min
#ifdef USE_CTR_SECURE
	sprintf_s(out, " %i ms", int(min*1000));
#elif
	sprintf(out, " %i ms", int(min*1000));
#endif
	string minString(out);
	minString += string(10-minString.size(), ' ') + "|";

	// Format total
#ifdef USE_CTR_SECURE
	sprintf_s(out, " %i ms %i%% (%i%%)", int(total*1000), int((total/globalTotal)*100), int((total/parentTotal)*100));
#elif
	sprintf(out, " %i ms %i%% (%i%%)", int(total*1000), int((total/globalTotal)*100), int((total/parentTotal)*100));
#endif
	string totalString(out);

	// Print formated strings
	LOG("%s%s%s%s%s", treeString, aveString, maxString, minString, totalString);

	// Loop children nodes
	for(map<string, Profile*>::iterator itr = node->children.begin(); itr != node->children.end(); ++itr)
		printChildren(itr->second, itr->first, depth+1, total, globalTotal);
}

void XProfiler::printResults()
{
	// Check that it is stopped
	if(m_active)
	{
		LOG("XProfiler::printResults() - Cannot print while the profilier is running");
		return;
	}

	// Validate stack
	if(m_currentNode != 0)
	{
		LOG("XProfiler::printResults() - The current profile is not 0");
		return;
	}

	// Print eplaced times
	LOG("\n--------------------------- Profiler Results ---------------------------\n");
	LOG("                                  Ave   |    Max   |    Min   |   Total  ");
	printChildren(m_root, m_rootName, 0, m_totalTime, m_totalTime);
	LOG("\n----------------------------------- End --------------------------------\n");
}
