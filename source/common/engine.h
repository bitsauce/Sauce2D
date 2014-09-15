#ifndef X2D_ENGINE_H
#define X2D_ENGINE_H

#include "classreg.h"
#include "engine.h"
#include "assetmanager.h"
#include "console.h"
#include "debugger.h"
#include "filesystem.h"
#include "math/math.h"
#include "common/scripts/scriptarray.h"

#define LOG(str, ...) XEngine::GetConsole()->logf(str, __VA_ARGS__)

#define NOT_IMPLEMENTED(func)			LOG("%s does not have a implementation.", #func);
#define NOT_IMPLEMENTED_RET(func, ret)	LOG("%s does not have a implementation.", #func); return ret;

class XEngine;
class XTimer;
class XWindow;
class XMath;
class XInput;
class XFileSystem;
class XScriptEngine;
class XGraphics;
class XAudio;
class XConsole;
class XProfiler;
class XDebug;
class XAssetLoader;
enum XRetCode;
enum XEngineFlag;
enum XState;

typedef int (*LoadPluginsFunc)(XEngine*);
typedef int (*LoadEventsFunc)(XEngine*);

struct XDAPI xdConfig
{
	xdConfig();

	bool isValid() const {
		return strlen(platform) > 0 && strlen(workDir) > 0 &&
			fileSystem != 0 && timer != 0 && input != 0 &&
			graphics != 0 && audio != 0;
	}

	void operator=(const xdConfig &other) {
		flags = other.flags;
		platform = other.platform;
		workDir = other.workDir;
		saveDir = other.saveDir;
		loadPluginsFunc = other.loadPluginsFunc;
		timer = other.timer;
		fileSystem = other.fileSystem;
		window = other.window;
		input = other.input;
		math = other.math;
		graphics = other.graphics;
		audio = other.audio;
		console = other.console;
	}

	// Engine running flags (optional)
	int				flags;

	// Platform string (required)
	const char*		platform;

	// Engine working directory (required)
	const char*		workDir;

	// System save directory (optional)
	const char*		saveDir;

	// Syster user directory (optional)
	const char*		userDir;

	// Plugin load function (optional)
	LoadPluginsFunc	loadPluginsFunc;

	// Load events
	LoadEventsFunc loadEventsFunc;

	// Engine timer object (required)
	XTimer*		timer;

	// File system manager (required)
	XFileSystem*	fileSystem;

	// Game window manager (optional)
	XWindow*		window;

	// Input class (required)
	XInput*		input;

	// Math class (optional)
	XMath*			math;

	// Graphics manager (required)
	XGraphics*		graphics;

	// Audio manager (required)
	XAudio*		audio;

	// Game console (optional)
	XConsole*		console;
};

/*********************************************************************
**	Game Engine														**
**********************************************************************/
class XDAPI XEngine
{
	friend class OpenGL;
public:
	AS_DECL_SINGLETON

	XEngine();
	~XEngine();

	// Initialize the engine
	int init(const xdConfig &config);

	// Run game
	int run();
	
	// Exit game
	void exit();

	// Exception
	//virtual void exception(const xdRetCode) = 0;

	// Platform string
	string getPlatformString() const;

	// Working directory
	string getWorkingDirectory() const;

	// Save directory
	string getSaveDirectory() const;

	// Profiler
	void toggleProfiler();
	void pushProfile(const string &profile);
	void popProfile();
	void setProfiler(XProfiler *profiler) { m_profiler = profiler; }

	// Scene
	void pushScene(asIScriptObject *object);
	void popScene();

	// Debugger
	void setDebugger(XDebug *debugger) { m_debugger = debugger; }

	// Exceptions
	void exception(XRetCode errorCode, const char* message);

	// Run flags
	static bool IsEnabled(const XEngineFlag flag) { return (s_this->m_flags & flag) != 0; }
	
	// Static engine modules
	static XConsole* GetConsole() { return s_this->m_console; }
	XScriptEngine *getScriptEngine() const { return m_scripts; }

private:
	int m_flags;

	string m_platformString;
	string m_workDir;
	string m_saveDir;
	
	XFileSystem *m_fileSystem;
	XGraphics *m_graphics;
	XAudio *m_audio;
	XProfiler *m_profiler;
	XDebug *m_debugger;
	XTimer *m_timer;
	XScriptEngine *m_scripts;
	XConsole *m_console;
	XWindow *m_window;
	XMath *m_math;
	XInput *m_input;
	XAssetLoader *m_assetLoader;

	// Game loop
	void draw();
	void update();
	void pause() { m_paused = true; }
	void resume() { m_paused = false; }
	void close() { m_running = false; }
	
	// State
	bool m_running;
	bool m_paused;

	bool m_toggleProfiler;

	// Scene stack
	stack<asIScriptObject*> m_sceneStack;

	// Event functions
	asIScriptFunction *m_defaultUpdateFunc;
	asIScriptFunction *m_defaultDrawFunc;
	asIScriptFunction *m_sceneUpdateFunc;
	asIScriptFunction *m_sceneDrawFunc;

	bool m_initialized;

	static XEngine *s_this;
};

extern XEngine *g_engine;

XDAPI XEngine *CreateEngine();

#endif // X2D_ENGINE_H