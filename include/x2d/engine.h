#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include "platform.h"

class X2DApp;
class X2DRender;
class X2DSound;
class X2DDebug;
class X2DAssetLoader;
enum X2DRetCode;
enum X2DRunFlag;
enum X2DState;

/*********************************************************************
**	Abstract? Engine												**
**********************************************************************/
// TODO: Make this semi-virtual having the application writer implement:
// - Printing
// - Game loop
// (- Asset loading)
class X2DAPI X2DEngine
{
public:
	X2DEngine(const int flags);
	~X2DEngine();

	X2DRetCode init();
	X2DRetCode run();

	// Game loop
	void draw();
	void update();
	void pause() { m_paused = true; }
	void resume() { m_paused = false; }
	void close() { m_running = false; }

	// Profiler
	void startProfiler();
	void stopProfiler();
	void pushProfile(const string &name);
	void popProfile();
	void printProfilerResults();
	
	// Refresh rate
	void setRefreshRate(const int hz);
	int getRefreshRate();
	float getTimeStep();

	// Run flags
	bool isEnabled(const X2DRunFlag flag)
	{
		return (m_flags & flag) != 0;
	}

public:
	// The engine parts
	X2DApp *app;
	X2DRender *gfx;
	X2DSound *sfx;
	X2DDebug *debug;
	X2DAssetLoader *assetLoader;

	class ScriptManager *scripts;

private:

	// Flags
	int m_flags;

	// State
	bool m_initialized;
	bool m_running;
	bool m_paused;
	
	// Profiler
	class X2DProfiler *m_profiler;
	bool m_toggleProfiler;

	// Time step
	float m_timeStep;

	// Fps
	int m_framesPerSecond;

	// Refresh rate
	int m_refreshRate;

	// Event functions
	class asIScriptFunction *m_updateFunc;
	class asIScriptFunction *m_drawFunc;
};

X2DAPI X2DEngine *CreateEngine(const int flags);

// Game engine
extern X2DEngine *gameEngine;

#endif // CORE_ENGINE_H