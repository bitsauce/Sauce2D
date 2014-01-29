#ifndef CORE_PROFILER_H
#define CORE_PROFILER_H

#include "x2d/platform.h"
#include "x2d/engine.h"

struct Profile
{
	Profile *parent;
	map<string, Profile*> children;
	float currentTime;
	vector<float> eplacedTimes;
};

class Profiler
{
public:
	Profiler();
	~Profiler();

	void start();
	void stop();

	void pushProfile(const string &name);
	void popProfile();

	void printResults();
	void printChildren(Profile *node, const string &name, const int depth, const float parentTotal, const float globalTotal);

	bool isActive() const { return m_active; }

private:
	
	bool m_active;
	float m_startTime;
	float m_totalTime;
	Profile *m_root;
	string m_rootName;
	Profile *m_currentNode;
	map<string, Profile> m_profiles;
};

#endif // CORE_PROFILER_H
