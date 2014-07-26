#ifndef X2D_ENGINE_H
#define X2D_ENGINE_H

#include <x2d/config.h>
#include <x2d/base.h>
#include <x2d/util.h>

class xdEngine;
class xdTimer;
class xdWindow;
class xdMath;
class xdInput;
class xdFileSystem;
class xdScriptEngine;
class xdGraphics;
class xdAudio;
class xdConsole;
class xdProfiler;
class xdDebug;
class xdAssetLoader;
enum xdRetCode;
enum xdEngineFlag;
enum xdState;

typedef int (*LoadPluginsFunc)(xdEngine*);
typedef int (*LoadEventsFunc)(xdEngine*);

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
	xdTimer*		timer;

	// File system manager (required)
	xdFileSystem*	fileSystem;

	// Game window manager (optional)
	xdWindow*		window;

	// Input class (required)
	xdInput*		input;

	// Math class (optional)
	xdMath*			math;

	// Graphics manager (required)
	xdGraphics*		graphics;

	// Audio manager (required)
	xdAudio*		audio;

	// Game console (optional)
	xdConsole*		console;
};

/*********************************************************************
**	Game Engine														**
**********************************************************************/
class XDAPI xdEngine
{
	friend class OpenGL;
public:
	AS_DECL_SINGLETON

	xdEngine();
	~xdEngine();

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
	void setProfiler(xdProfiler *profiler) { m_profiler = profiler; }

	// Scene
	void pushScene(asIScriptObject *object);
	void popScene();

	// Debugger
	void setDebugger(xdDebug *debugger) { m_debugger = debugger; }

	// Exceptions
	void exception(xdRetCode errorCode, const char* message);

	// Run flags
	static bool IsEnabled(const xdEngineFlag flag) { return (s_this->m_flags & flag) != 0; }
	
	// Static engine modules
	static xdConsole* GetConsole() { return s_this->m_console; }
	xdScriptEngine *getScriptEngine() const { return m_scripts; }

private:
	int m_flags;

	string m_platformString;
	string m_workDir;
	string m_saveDir;
	
	xdFileSystem *m_fileSystem;
	xdGraphics *m_graphics;
	xdAudio *m_audio;
	xdProfiler *m_profiler;
	xdDebug *m_debugger;
	xdTimer *m_timer;
	xdScriptEngine *m_scripts;
	xdConsole *m_console;
	xdWindow *m_window;
	xdMath *m_math;
	xdInput *m_input;
	xdAssetLoader *m_assetLoader;

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

	static xdEngine *s_this;
};

extern xdEngine *g_engine;

XDAPI xdEngine *CreateEngine();

#endif // X2D_ENGINE_H