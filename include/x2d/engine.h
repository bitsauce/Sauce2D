#ifndef X2D_ENGINE_H
#define X2D_ENGINE_H

#include "config.h"
#include "math.h"
#include "resourcemanager.h"

#define LOG(str, ...) XConsole::Log(str, __VA_ARGS__)

#define NOT_IMPLEMENTED(func)			LOG("%s does not have a implementation.", #func);
#define NOT_IMPLEMENTED_RET(func, ret)	LOG("%s does not have a implementation.", #func); return ret;

/*********************************************************************
**	Game console													**
**********************************************************************/

class XFileSystem;
class XFileReader;
class XFileWriter;
class XDebugger;
class XAudioManager;
class XGraphics;

class XDAPI XConsole
{
	friend class XEngine;

	SINGLETON_DECL(XConsole);

public:
	XConsole();
	~XConsole();

	void log(const string &msg);
	void logf(const char *msg, ...);
	string getLog() const;

	void clear();
	void exportFile() const;

	string readBuffer();
	bool hasBuffer() const;
	void clearBuffer();

	static void Log(const char *msg, ...);

private:
	void call_log(const char *msg, va_list args);

	string m_log;
	string m_buffer;
	
	XEngine *m_engine;
	XFileWriter *m_output;
	
	static bool s_initialized;
};

/*********************************************************************
**	Event handler													**
**********************************************************************/
class XDAPI XTimer
{
public:
	XTimer();
	
	// High-resolution timing
	void  start();
	void  stop();
	float getElapsedTime() const;

	// System clock
	void getTicksPerSecond();
	long getTickCount() const;

private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_start;
	LARGE_INTEGER m_end;
	bool m_running;
	
	int m_ticksPerSec;
};

/*********************************************************************
**	Event handler													**
**********************************************************************/
class XDAPI XEventHandler
{
	virtual void processEvents() = 0;
};

/*********************************************************************
**	Thread manager													**
**********************************************************************/
class XThreadManager
{
public:
	virtual void setupThread() = 0;
	virtual void cleanupThread() = 0;
};

/*********************************************************************
**	File reader class												**
**********************************************************************/
class XDAPI XFileReader
{
public:
	XFileReader(const string &path);
	~XFileReader();

	bool isOpen();
	bool isEOF();
	void close();

	string readLine();
	string readAll();

private:
	ifstream stream;
};

/*********************************************************************
**	File writer class												**
**********************************************************************/
class XDAPI XFileWriter
{
public:
	XFileWriter(const string &path);
	~XFileWriter();

	bool isOpen();
	void close();

	void clear();
	void append(const char* data, const int length);
	void append(const string &str) { append(str.data(), str.size()); }
	void flush();

private:
	ofstream stream;
};

/*********************************************************************
**	File system class												**
**********************************************************************/
class XDAPI XFileSystem
{
	friend class XEngine;

	SINGLETON_DECL(XFileSystem)
public:
	// File buffers
	bool readFile(string filePath, string &conent) const;
	bool writeFile(string filePath, const string content) const;

	// OS specifics
	bool fileExists(string &filePath) const;
	// NOTE TO SELF: I might want to consider making a DirectoryIterator instead of using this function
	//virtual XScriptArray *listFiles(string &directory, const string &mask, const bool recursive) const		{ NOT_IMPLEMENTED_RET(listFiles, 0) }			// Optional
	//virtual XScriptArray *listFolders(string &directory, const string &mask, const bool recursive) const	{ NOT_IMPLEMENTED_RET(listFolders, 0) }			// Optional
	bool remove(string &path)																		{ NOT_IMPLEMENTED_RET(remove, false) }			// Optional

	// System windows
	string showSaveDialog(const string &file, const string &ext, const string &title, const string &folder) const	{ NOT_IMPLEMENTED_RET(showSaveDialog, "") }				// Optional
	string showOpenDialog(const string &file, const string &ext, const string &title, const string &folder) const	{ NOT_IMPLEMENTED_RET(showOpenDialog, "") }				// Optional

	// Static functions
	static bool ReadFile(string path, string &content);
	static bool WriteFile(string path, const string &content);
	static bool MakeDir(string path);

protected:
	bool makeDir(const string &path);
};

/*********************************************************************
**	Input class [static]											**
**********************************************************************/

// Mouse buttons
enum XMouseButton
{
	XD_MOUSE_LEFT,
	XD_MOUSE_MIDDLE,
	XD_MOUSE_RIGHT
};

// Virtual keys
enum XVirtualKey
{
	// Standard ASCII-Mapped keys
	XD_KEY_NULL = 0x00,
	XD_KEY_BACKSPACE = 0x08,
	XD_KEY_TAB = 0x09,
	XD_KEY_ENTER = 0x0D,
	XD_KEY_ESCAPE = 0x1B,
	XD_KEY_SPACE = 0x20,

	XD_KEY_0 = 0x30,
	XD_KEY_1,
	XD_KEY_2,
	XD_KEY_3,
	XD_KEY_4,
	XD_KEY_5,
	XD_KEY_6,
	XD_KEY_7,
	XD_KEY_8,
	XD_KEY_9,

	// Alphabetic keys
	XD_KEY_A = 0x41,
	XD_KEY_B,
	XD_KEY_C,
	XD_KEY_D,
	XD_KEY_E,
	XD_KEY_F,
	XD_KEY_G,
	XD_KEY_H,
	XD_KEY_I,
	XD_KEY_J,
	XD_KEY_K,
	XD_KEY_L,
	XD_KEY_M,
	XD_KEY_N,
	XD_KEY_O,
	XD_KEY_P,
	XD_KEY_Q,
	XD_KEY_R,
	XD_KEY_S,
	XD_KEY_T,
	XD_KEY_U,
	XD_KEY_V,
	XD_KEY_W,
	XD_KEY_X,
	XD_KEY_Y,
	XD_KEY_Z,

	// Mouse keys
	XD_LMB = 0x80,
	XD_RMB,
	XD_WHEEL,

	// Arrow keys
	XD_KEY_Left,
	XD_KEY_Up,
	XD_KEY_Right,
	XD_KEY_Down,

	// Special keys
	XD_KEY_Shift,
	XD_KEY_LeftShift,
	XD_KEY_RightShift,
	XD_KEY_Control,
	XD_KEY_LeftControl,
	XD_KEY_RightControl,
	XD_KEY_Menu, // Windows button
	XD_KEY_LeftMenu,
	XD_KEY_RightMenu,
	XD_KEY_Space,
	XD_KEY_PageUp,
	XD_KEY_PageDown,
	XD_KEY_End,
	XD_KEY_Home,
	XD_KEY_Snapshot,
	XD_KEY_Insert,
	XD_KEY_Delete,

	// Numpad keys
	XD_KEY_Numpad0,
	XD_KEY_Numpad1,
	XD_KEY_Numpad2,
	XD_KEY_Numpad3,
	XD_KEY_Numpad4,
	XD_KEY_Numpad5,
	XD_KEY_Numpad6,
	XD_KEY_Numpad7,
	XD_KEY_Numpad8,
	XD_KEY_Numpad9,
	XD_KEY_Plus,
	XD_KEY_Hyphen,

	// Other keys
	XD_KEY_Caret,
	XD_KEY_Apostrophe,
	XD_KEY_QuotationMark,
	XD_KEY_Backslash,
	XD_KEY_Slash,
	XD_KEY_Asterisk,
	XD_KEY_Pipe,
	XD_KEY_Colon,
	XD_KEY_Semicolon,
	XD_KEY_Tilde,
	XD_KEY_Comma,
	XD_KEY_Period,
	XD_KEY_Greater,
	XD_KEY_Less,
	XD_KEY_Equals
};

class XDAPI XInput
{
	friend class XWindow;

public:
	// Desktop cursor functions
	static void     setCursorPos(const Vector2i &pos);
	static Vector2i getCursorPos();
	static void     setCursorLimits(const Recti &area);

	// Key state function
	static bool getKeyState(const XVirtualKey key);

	// General position
	static Vector2 getPosition();

	// Key binding
	static void bind(const XVirtualKey key, delegate<void()> *function);
	static void unbind(const XVirtualKey key);
	static void resetBindings();
	static void updateBindings();
	static void checkBindings();

	// Keyboard listener
	//static void addKeyboardListener(XKeyboardListener *object);
	static void charEvent(uint utf8char);
	static void keyPressed(const XVirtualKey key);
	static void keyReleased(const XVirtualKey key);

	// Mouse listener
	//static void addMouseListener(XMouseListener *object);
	//static void mouseClick(const XMouseButton btn); // This doesn't make any sence unless we specify a click-rectangle
	static void mouseScroll(const int dt);

	// Overloads
	static void setCursorPos(const int x, const int y) { setCursorPos(Vector2i(x, y)); }
	static void setCursorLimits(const int x, const int y, const int w, const int h) { setCursorLimits(Recti(x, y, w, h)); }

private:

	// Key bind
	struct KeyBind
	{
		bool pressed;
		delegate<void()> function;
	};

	// Keyboard listener
	static map<XVirtualKey, KeyBind> s_keyBindings;
	static vector<delegate<void()>> s_keyListeners;

	// Mouse listener
	static map<XMouseButton, bool> s_mousePressed;
	static vector<delegate<void()>> s_mouseListeners;

	// Cursor position
	static Vector2 s_position;
};

extern XVirtualKey fromWinKey(uchar key);
extern uchar toWinKey(XVirtualKey key);

/*********************************************************************
**	Scene interface													**
**********************************************************************/

class XScene
{
	friend class XEngine;
private:
	virtual void showEvent() {}
	virtual void hideEvent() {}

	virtual void drawEvent() {}
	virtual void updateEvent() {}

	virtual void resizeEvent() {}
	
	//virtual void keyboardEvent() {}
	//virtual void mouseEvent() {}
};

/*********************************************************************
**	Profiler														**
**********************************************************************/
class XDAPI XProfiler
{
	friend class XDebugger;
	friend class XEngine;

	struct Node
	{
		// Constructors
		Node(const string &name) :
			name(name),
			duration(0.0f),
			calls(0)
		{
		}

		// Parent-child relations
		Node *parent;
		map<string, Node*> children;

		// Time measurement
		float startTime;
		float duration;
		int calls;

		// Node data
		string name;
	};

public:
	XProfiler();
	virtual ~XProfiler();

	void deleteTree(Node *node);

	void enable();
	void disable();
	bool isEnabled() const;

	void push(const string &name);
	void pop();

	void sendStats(Node *node);
	void stepBegin();

private:
	Node *m_root;
	Node *m_currentNode;
	XDebugger *m_debugger;
	XTimer *m_timer;
	uint m_samples;
	bool m_enabled;
	bool m_toggle;
};

/*********************************************************************
**	Window class													**
**********************************************************************/
class XDAPI XWindow
{
	friend class XGraphics;
public:
	static void processEvents();
	static void processEvents(UINT Message, WPARAM wParam, LPARAM lParam);
	static void close();
	static void show();
	
	static void enableFullscreen();
	static void disableFullscreen();
	static bool isFullscreen();
	//XScriptArray *getResolutionList() const;

	static void enableResize();
	static void disableResize();
	static bool hasFocus();
	
	// Window actions
	static void setPosition(const Vector2i &pos);
	static Vector2i getPosition();
	static void setSize(const Vector2i &size);
	static Vector2i getSize();
	static void minimize();
	static void maximize();
	static void restore();

private:
	// The window handle
	static HWND s_window;

	// The window device context
	static HDC s_deviceContext;
	
	// The event message
	static MSG s_message;

	// Cached list of resolutions
	static vector<Vector2i> s_resolutions;

	// Window focus
	static bool s_focus;

	// Window size
	static Vector2i s_size;

	// Window fullscreen state
	static bool s_fullscreen;
	
	// Window procedure callback
	static LRESULT CALLBACK OnEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
};

/*********************************************************************
**	Exception class													**
**********************************************************************/
class XException
{
public:
	XException(XRetCode code, string msg) :
		m_errorCode(code),
		m_message(msg)
	{
	}

	XRetCode errorCode() const
	{
		return m_errorCode;
	}

	string message() const
	{
		return m_message;
	}

private:
	string m_message;
	XRetCode m_errorCode;
};

/*********************************************************************
**	Engine config													**
**********************************************************************/
typedef void (*VoidFunc)();

struct XDAPI XConfig
{
	XConfig();

	bool isValid() const
	{
		return draw && update && main;
	}

	void operator=(const XConfig &other)
	{
		flags = other.flags;
		workDir = other.workDir;
	}

	// Engine running flags (optional)
	int				flags;

	// Engine working directory (required)
	const char*		workDir;

	// Game loop functions
	VoidFunc		draw, update, main, end;
};

/*********************************************************************
**	Game engine														**
**********************************************************************/
class XDAPI XEngine
{
public:
	XEngine();
	~XEngine();

	// Initialize the engine
	int init(const XConfig &config);

	// Run game
	int run();
	
	// Exit game
	static void exit();
	static void pause() { s_paused = true; }
	static void resume() { s_paused = false; }

	// Exception
	//virtual void exception(const xdRetCode) = 0;

	// Scene
	static void pushScene(XScene *scene);
	static void popScene();

	// Exceptions
	void exception(XRetCode errorCode, const char* message);

	// Static functions
	static bool isEnabled(const XEngineFlag flag) { return (s_flags & flag) != 0; }
	static string getWorkingDirectory() { return s_workDir; }
	static string getSaveDirectory() { return s_saveDir; }

private:
	
	// State
	static bool s_initialized;
	static bool s_paused;
	static bool s_running;
	static int s_flags;

	// System dirs
	static string s_workDir;
	static string s_saveDir;
	
	XFileSystem*	m_fileSystem;
	XGraphics*		m_graphics;
	XAudioManager*	m_audio;
	XTimer*			m_timer;
	XConsole*		m_console;
	XMath*			m_math;

	// Game loop
	void draw();
	void update();

	// Scene stack
	static stack<XScene*> s_sceneStack;

	// Game loop
	VoidFunc m_mainFunc, m_drawFunc, m_updateFunc, m_endFunc;
};

XDAPI XEngine *CreateEngine();

#endif // X2D_ENGINE_H
