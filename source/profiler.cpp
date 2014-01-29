//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)

#include "x2d/math/profiler.h"
#include "x2d/app.h"
#include "x2d/engine.h"
#include "x2d/iosystem.h"

X2DProfiler::X2DProfiler() :
	m_active(false),
	m_currentNode(0),
	m_root(0),
	m_rootName("")
{
}

X2DProfiler::~X2DProfiler()
{
}

void X2DProfiler::start()
{
	// Clear eplaced times
	m_root = 0;
	m_profiles.clear();
	m_startTime = gameEngine->app->getHrTime();
	m_active = true;
}

void X2DProfiler::stop()
{
	m_active = false;
	m_totalTime = gameEngine->app->getHrTime()-m_startTime;
}

void X2DProfiler::pushProfile(const string &name)
{
	if(!m_active) return;
	
	Profile *p = &m_profiles[name];
	if(m_currentNode == 0)
	{
		// Set as root
		m_root = p;
		m_rootName = name;
		p->parent = 0;
	}else{
		p->parent = m_currentNode;
		m_currentNode->children[name] = p;
	}

	p->currentTime = gameEngine->app->getHrTime();
	m_currentNode = p;
}

void X2DProfiler::popProfile()
{
	if(!m_active) return;

	if(!m_currentNode)
	{
		iosystem::error("appPopProfile() no current node");
		return;
	}

	m_currentNode->eplacedTimes.push_back(gameEngine->app->getHrTime()-m_currentNode->currentTime);
	m_currentNode = m_currentNode->parent;
}


void X2DProfiler::printChildren(Profile *node, const string &name, const int depth, const float parentTotal, const float globalTotal)
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
	sprintf(out, "-%s%s", indentString.c_str(), name.c_str());
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
	sprintf(out, " %i ms", int(ave*1000));
	string aveString(out);
	aveString += string(10-aveString.size(), ' ') + "|";

	// Format max
	sprintf(out, " %i ms", int(max*1000));
	string maxString(out);
	maxString += string(10-maxString.size(), ' ') + "|";

	// Format min
	sprintf(out, " %i ms", int(min*1000));
	string minString(out);
	minString += string(10-minString.size(), ' ') + "|";

	// Format total
	sprintf(out, " %i ms %i%% (%i%%)", int(total*1000), int((total/globalTotal)*100), int((total/parentTotal)*100));
	string totalString(out);

	// Print formated strings
	iosystem::print(treeString + aveString + maxString + minString + totalString);

	// Loop children nodes
	for(map<string, Profile*>::iterator itr = node->children.begin(); itr != node->children.end(); ++itr)
		printChildren(itr->second, itr->first, depth+1, total, globalTotal);
}

void X2DProfiler::printResults()
{
	// Check that it is stopped
	if(m_active)
	{
		iosystem::error("appPrintProfileResults() cannot print while the profilier is running");
		return;
	}

	// Validate stack
	if(m_currentNode != 0)
	{
		iosystem::error("appPrintProfileResults() the current profile is not 0");
		return;
	}

	// Print eplaced times
	iosystem::print("\n--------------------------- Profiler Results ---------------------------\n");
	iosystem::print("                                  Ave   |    Max   |    Min   |   Total  ");
	printChildren(m_root, m_rootName, 0, m_totalTime, m_totalTime);
	iosystem::print("\n----------------------------------- End --------------------------------\n");
}
