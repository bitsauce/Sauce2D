#ifndef X2D_PROFILER_H
#define X2D_PROFILER_H

#include <x2d/config.h>
#include <x2d/util.h>
#include <x2d/base.h>

class xdTimer;

class XDAPI xdProfiler
{
	struct Profile
	{
		Profile *parent;
		map<string, Profile*> children;
		float currentTime;
		vector<float> eplacedTimes;
	};
public:
	AS_DECL_SINGLETON

	xdProfiler(xdTimer *timer);
	virtual ~xdProfiler();

	void start();
	void stop();

	void pushProfile(const string &name);
	void popProfile();

	void printResults();
	void printChildren(Profile *node, const string &name, const int depth, const float parentTotal, const float globalTotal);

	bool isActive() const { return m_active; }

private:
	
	xdTimer *m_timer;
	bool m_active;
	float m_startTime;
	float m_totalTime;
	Profile *m_root;
	string m_rootName;
	Profile *m_currentNode;
	map<string, Profile> m_profiles;
};

#endif // X2D_PROFILER_H