#ifndef X2D_ENGINE_H
#define X2D_ENGINE_H

#include "config.h"
#include "math.h"
#include "resourcemanager.h"
#include "iniparser.h"

BEGIN_XD_NAMESPACE

#define LOG(str, ...) Console::Log(str, __VA_ARGS__)

#define NOT_IMPLEMENTED(func)			LOG("%s does not have a implementation.", #func);
#define NOT_IMPLEMENTED_RET(func, ret)	LOG("%s does not have a implementation.", #func); return ret;

/*********************************************************************
**	Game console													**
**********************************************************************/

class FileSystem;
class FileReader;
class FileWriter;
class AudioManager;
class Graphics;

class XDAPI Console
{
	friend class Engine;

	SINGLETON_DECL(Console);

public:
	Console();
	~Console();

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
	
	Engine *m_engine;
	FileWriter *m_output;
	
	static bool s_initialized;
};

/*********************************************************************
**	Event handler													**
**********************************************************************/
class XDAPI Timer
{
public:
	Timer();
	
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
class XDAPI EventHandler
{
	virtual void processEvents() = 0;
};

/*********************************************************************
**	Thread manager													**
**********************************************************************/
class XDAPI ThreadManager
{
public:
	virtual void setupThread() = 0;
	virtual void cleanupThread() = 0;
};

/*********************************************************************
**	File reader class												**
**********************************************************************/
class XDAPI FileReader
{
public:
	FileReader(const string &path);
	~FileReader();

	bool isOpen();
	bool isEOF();
	void close();

	string readLine();
	string readAll();

	FileReader &operator>>(int &i) { stream >> i; return *this; }
	FileReader &operator>>(char &c) { stream >> c; return *this; }

private:
	ifstream stream;
};

/*********************************************************************
**	File writer class												**
**********************************************************************/
class XDAPI FileWriter
{
public:
	FileWriter(const string &path);
	~FileWriter();

	bool isOpen();
	void close();

	void clear();
	void append(const char* data, const int length);
	void append(const string &str) { append(str.data(), str.size()); }
	void flush();
	
    // define an operator<< to take in std::endl
    FileWriter &operator<<(basic_ostream<char, std::char_traits<char> >& (*fp)(basic_ostream<char, std::char_traits<char> >&)) { stream << fp; return *this; }

	bool operator!() const { return !stream; }
	FileWriter &operator<<(int i) { stream << i; return *this; }
	FileWriter &operator<<(char c) { stream << c; return *this; }

private:
	ofstream stream;
};

/*********************************************************************
**	File writer class												**
**********************************************************************/
class XDAPI FileSystemIterator
{
public:
	FileSystemIterator(string path, const string &mask, const int flags);
	//~FileSystemIterator();

	enum Flag
	{
		DIRECTORIES = 1 << 1,
		FILES = 1 << 2
	};
	
	bool hasNext() const;
	string &next();

	operator bool() const { return hasNext(); }

private:
	vector<string> m_files;
	vector<string>::iterator m_itr;
};


/*********************************************************************
**	File system class												**
**********************************************************************/
class XDAPI FileSystem
{
	friend class Engine;

	SINGLETON_DECL(FileSystem)
public:
	// File buffers
	bool readFile(string filePath, string &conent) const;
	bool writeFile(string filePath, const string content) const;

	// OS specifics
	bool fileExists(string &filePath) const;
	// NOTE TO SELF: I might want to consider making a DirectoryIterator instead of using this function
	//virtual XScriptArray *listFiles(string &directory, const string &mask, const bool recursive) const	{ NOT_IMPLEMENTED_RET(listFiles, 0) }			// Optional
	//virtual XScriptArray *listFolders(string &directory, const string &mask, const bool recursive) const	{ NOT_IMPLEMENTED_RET(listFolders, 0) }			// Optional
	bool remove(string &path)																				{ NOT_IMPLEMENTED_RET(remove, false) }			// Optional

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
enum MouseButton
{
	XD_MOUSE_LEFT,
	XD_MOUSE_MIDDLE,
	XD_MOUSE_RIGHT
};

// Virtual keys
enum VirtualKey
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
	XD_KEY_LEFT,
	XD_KEY_UP,
	XD_KEY_RIGHT,
	XD_KEY_DOWN,

	// Special keys
	XD_KEY_SHIFT,
	XD_KEY_LSHIFT,
	XD_KEY_RSHIFT,
	XD_KEY_CONTROL,
	XD_KEY_LCONTROL,
	XD_KEY_RCONTROL,
	XD_KEY_MENU, // Windows button
	XD_KEY_LMENU,
	XD_KEY_RMENU,
	//XD_KEY_SPACE,
	XD_KEY_PAGEUP,
	XD_KEY_PAGEDOWN,
	XD_KEY_END,
	XD_KEY_HOME,
	XD_KEY_SNAPSHOT,
	XD_KEY_INSERT,
	XD_KEY_DELETE,

	// Function keys
	XD_KEY_F1,
	XD_KEY_F2,
	XD_KEY_F3,
	XD_KEY_F4,
	XD_KEY_F5,
	XD_KEY_F6,
	XD_KEY_F7,
	XD_KEY_F8,
	XD_KEY_F9,
	XD_KEY_F10,
	XD_KEY_F11,
	XD_KEY_F12,

	// Numpad keys
	XD_KEY_NUMPAD0,
	XD_KEY_NUMPAD1,
	XD_KEY_NUMPAD2,
	XD_KEY_NUMPAD3,
	XD_KEY_NUMPAD4,
	XD_KEY_NUMPAD5,
	XD_KEY_NUMPAD6,
	XD_KEY_NUMPAD7,
	XD_KEY_NUMPAD8,
	XD_KEY_NUMPAD9,
	XD_KEY_PLUS,
	XD_KEY_MINUS,

	// Other keys
	XD_KEY_CARET,
	XD_KEY_APOSTROPHE,
	XD_KEY_QUOTATION_MARK,
	XD_KEY_BACKSLASH,
	XD_KEY_SLASH,
	XD_KEY_ASTERISK,
	XD_KEY_PIPE,
	XD_KEY_COLON,
	XD_KEY_SEMICOLON,
	XD_KEY_TILDE,
	XD_KEY_COMMA,
	XD_KEY_PERIOD,
	XD_KEY_GREATER,
	XD_KEY_LESS,
	XD_KEY_EQUALS
};

class XDAPI KeyboardListener
{
public:
	virtual void keyPressEvent(const VirtualKey key) { }
	virtual void keyReleaseEvent(const VirtualKey key) { }
	virtual void charEvent(const wchar_t c) { }
};

class XDAPI MouseListener
{
public:
	virtual void mouseWheelEvent(const int dt) { }
};

class XDAPI Input
{
	friend class Window;
public:
	// Desktop cursor functions
	static void     setCursorPos(const Vector2i &pos);
	static Vector2i getCursorPos();
	static void     setCursorLimits(const Recti &area);

	// Key state function
	static bool getKeyState(const VirtualKey key);

	// General position
	static Vector2 getPosition();

	// Key binding
	static void bind(const VirtualKey key, function<void()> function);
	static void unbind(const VirtualKey key);
	static void resetBindings();
	static void updateBindings();
	static void checkBindings();

	// Keyboard listener
	static void addKeyboardListener(KeyboardListener *object);
	static void charEvent(const wchar_t c);
	static void keyPressed(const VirtualKey key);
	static void keyReleased(const VirtualKey key);

	// Mouse listener
	static void addMouseListener(MouseListener *object);
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
		function<void()> function;
	};

	// Keyboard listener
	static map<VirtualKey, KeyBind> s_keyBindings;
	static vector<KeyboardListener*> s_keyListeners;

	// Mouse listener
	static map<MouseButton, bool> s_mousePressed;
	static vector<MouseListener*> s_mouseListeners;

	// Cursor position
	static Vector2 s_position;
};

extern VirtualKey fromWinKey(uchar key);
extern uchar toWinKey(VirtualKey key);

/*********************************************************************
**	Profiler														**
**********************************************************************/
class XDAPI Profiler
{
	friend class Engine;

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
	Profiler();
	virtual ~Profiler();

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
	Timer *m_timer;
	uint m_samples;
	bool m_enabled;
	bool m_toggle;
};

/*********************************************************************
**	Window class													**
**********************************************************************/
class WindowListener
{
	friend class Window;
protected:
	virtual void resizeEvent(uint width, uint height) {}
	virtual void moveEvent(uint x, uint y) {}
	//virtual void 
};

class Graphics;
class GraphicsContext;

class XDAPI Window
{
	friend class Graphics;
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

	static void addWindowListener(WindowListener *listener);
	static void removeWindowListener(WindowListener *listener);

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

	// Windows graphics device
	static GraphicsContext *s_graphicsContext;

	// Window listeners
	static list<WindowListener*> s_windowListeners;
	
	// Window procedure callback
	static LRESULT CALLBACK OnEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
};

/*********************************************************************
**	Exception class													**
**********************************************************************/
class XDAPI Exception
{
public:
	Exception(RetCode code, string msg) :
		m_errorCode(code),
		m_message(msg)
	{
	}

	RetCode errorCode() const
	{
		return m_errorCode;
	}

	string message() const
	{
		return m_message;
	}

private:
	string m_message;
	RetCode m_errorCode;
};

/*********************************************************************
**	Engine config													**
**********************************************************************/
struct XDAPI Config
{
	Config();

	// Engine running flags (optional)
	int				flags;

	// Engine working directory (required)
	const char*		workDir;

	// Game loop functions
	function<void()> update, main, stepBegin, stepEnd, end;
	function<void(GraphicsContext&)> draw;
};

/*********************************************************************
**	Game engine														**
**********************************************************************/
class XDAPI Engine
{
public:
	Engine();
	~Engine();

	// Initialize the engine
	int init(const Config &config);

	// Run game
	int run();
	
	// Exit game
	static void exit();
	static void pause() { s_paused = true; }
	static void resume() { s_paused = false; }

	// Exceptions
	void exception(RetCode errorCode, const char* message);

	// Static functions
	static bool isEnabled(const EngineFlag flag) { return (s_flags & flag) != 0; }
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
	
	FileSystem		*m_fileSystem;
	Graphics		*m_graphics;
	AudioManager	*m_audio;
	Timer			*m_timer;
	Console			*m_console;

	// Game loop
	void draw();
	void update();

	// Game loop
	function<void()> m_mainFunc, m_updateFunc, m_stepBeginFunc, m_stepEndFunc, m_endFunc;
	function<void(GraphicsContext&)> m_drawFunc;
};

XDAPI Engine *CreateEngine();

END_XD_NAMESPACE

#endif // X2D_ENGINE_H
