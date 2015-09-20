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
	double getElapsedTime() const;

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

	bool readBytes(char *buffer, const int count);
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

/* The unknown key */
#define XD_KEY_UNKNOWN            -1

/* Printable keys */
#define XD_KEY_SPACE              32
#define XD_KEY_APOSTROPHE         39  /* ' */
#define XD_KEY_COMMA              44  /* , */
#define XD_KEY_MINUS              45  /* - */
#define XD_KEY_PERIOD             46  /* . */
#define XD_KEY_SLASH              47  /* / */
#define XD_KEY_0                  48
#define XD_KEY_1                  49
#define XD_KEY_2                  50
#define XD_KEY_3                  51
#define XD_KEY_4                  52
#define XD_KEY_5                  53
#define XD_KEY_6                  54
#define XD_KEY_7                  55
#define XD_KEY_8                  56
#define XD_KEY_9                  57
#define XD_KEY_SEMICOLON          59  /* ; */
#define XD_KEY_EQUAL              61  /* = */
#define XD_KEY_A                  65
#define XD_KEY_B                  66
#define XD_KEY_C                  67
#define XD_KEY_D                  68
#define XD_KEY_E                  69
#define XD_KEY_F                  70
#define XD_KEY_G                  71
#define XD_KEY_H                  72
#define XD_KEY_I                  73
#define XD_KEY_J                  74
#define XD_KEY_K                  75
#define XD_KEY_L                  76
#define XD_KEY_M                  77
#define XD_KEY_N                  78
#define XD_KEY_O                  79
#define XD_KEY_P                  80
#define XD_KEY_Q                  81
#define XD_KEY_R                  82
#define XD_KEY_S                  83
#define XD_KEY_T                  84
#define XD_KEY_U                  85
#define XD_KEY_V                  86
#define XD_KEY_W                  87
#define XD_KEY_X                  88
#define XD_KEY_Y                  89
#define XD_KEY_Z                  90
#define XD_KEY_LEFT_BRACKET       91  /* [ */
#define XD_KEY_BACKSLASH          92  /* \ */
#define XD_KEY_RIGHT_BRACKET      93  /* ] */
#define XD_KEY_GRAVE_ACCENT       96  /* ` */
#define XD_KEY_WORLD_1            161 /* non-US #1 */
#define XD_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define XD_KEY_ESCAPE             256
#define XD_KEY_ENTER              257
#define XD_KEY_TAB                258
#define XD_KEY_BACKSPACE          259
#define XD_KEY_INSERT             260
#define XD_KEY_DELETE             261
#define XD_KEY_RIGHT              262
#define XD_KEY_LEFT               263
#define XD_KEY_DOWN               264
#define XD_KEY_UP                 265
#define XD_KEY_PAGE_UP            266
#define XD_KEY_PAGE_DOWN          267
#define XD_KEY_HOME               268
#define XD_KEY_END                269
#define XD_KEY_CAPS_LOCK          280
#define XD_KEY_SCROLL_LOCK        281
#define XD_KEY_NUM_LOCK           282
#define XD_KEY_PRINT_SCREEN       283
#define XD_KEY_PAUSE              284
#define XD_KEY_F1                 290
#define XD_KEY_F2                 291
#define XD_KEY_F3                 292
#define XD_KEY_F4                 293
#define XD_KEY_F5                 294
#define XD_KEY_F6                 295
#define XD_KEY_F7                 296
#define XD_KEY_F8                 297
#define XD_KEY_F9                 298
#define XD_KEY_F10                299
#define XD_KEY_F11                300
#define XD_KEY_F12                301
#define XD_KEY_F13                302
#define XD_KEY_F14                303
#define XD_KEY_F15                304
#define XD_KEY_F16                305
#define XD_KEY_F17                306
#define XD_KEY_F18                307
#define XD_KEY_F19                308
#define XD_KEY_F20                309
#define XD_KEY_F21                310
#define XD_KEY_F22                311
#define XD_KEY_F23                312
#define XD_KEY_F24                313
#define XD_KEY_F25                314
#define XD_KEY_KP_0               320
#define XD_KEY_KP_1               321
#define XD_KEY_KP_2               322
#define XD_KEY_KP_3               323
#define XD_KEY_KP_4               324
#define XD_KEY_KP_5               325
#define XD_KEY_KP_6               326
#define XD_KEY_KP_7               327
#define XD_KEY_KP_8               328
#define XD_KEY_KP_9               329
#define XD_KEY_KP_DECIMAL         330
#define XD_KEY_KP_DIVIDE          331
#define XD_KEY_KP_MULTIPLY        332
#define XD_KEY_KP_SUBTRACT        333
#define XD_KEY_KP_ADD             334
#define XD_KEY_KP_ENTER           335
#define XD_KEY_KP_EQUAL           336
#define XD_KEY_LEFT_SHIFT         340
#define XD_KEY_LEFT_CONTROL       341
#define XD_KEY_LEFT_ALT           342
#define XD_KEY_LEFT_SUPER         343
#define XD_KEY_RIGHT_SHIFT        344
#define XD_KEY_RIGHT_CONTROL      345
#define XD_KEY_RIGHT_ALT          346
#define XD_KEY_RIGHT_SUPER        347
#define XD_KEY_MENU               348
#define XD_KEY_LAST               XD_KEY_MENU

#define XD_MOD_SHIFT           0x0001
#define XD_MOD_CONTROL         0x0002
#define XD_MOD_ALT             0x0004
#define XD_MOD_SUPER           0x0008

#define XD_MOUSE_BUTTON_1         0
#define XD_MOUSE_BUTTON_2         1
#define XD_MOUSE_BUTTON_3         2
#define XD_MOUSE_BUTTON_4         3
#define XD_MOUSE_BUTTON_5         4
#define XD_MOUSE_BUTTON_6         5
#define XD_MOUSE_BUTTON_7         6
#define XD_MOUSE_BUTTON_8         7
#define XD_MOUSE_BUTTON_LAST      XD_MOUSE_BUTTON_8
#define XD_MOUSE_BUTTON_LEFT      XD_MOUSE_BUTTON_1
#define XD_MOUSE_BUTTON_RIGHT     XD_MOUSE_BUTTON_2
#define XD_MOUSE_BUTTON_MIDDLE    XD_MOUSE_BUTTON_3

typedef int VirtualKey;

class XDAPI KeyListener
{
public:
	KeyListener();
	virtual ~KeyListener();

	virtual void keyPressEvent(const VirtualKey key) { }
	virtual void keyReleaseEvent(const VirtualKey key) { }
	virtual void charEvent(const uint c) { }
};

class XDAPI MouseListener
{
public:
	MouseListener();
	virtual ~MouseListener();

	virtual void mouseWheelEvent(const int dt) { }
};

class XDAPI Input
{
	friend class Window;
	friend class KeyListener;
	friend class MouseListener;
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
	static void unbindAll();
	static void checkBindings();

	// Keyboard listener
	static void charEvent(const uint c);
	static void keyPressed(const VirtualKey key);
	static void keyReleased(const VirtualKey key);

	// Mouse listener
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
	static list<KeyListener*> s_keyListeners;
	static void addKeyListener(KeyListener *object);
	static void removeKeyListener(KeyListener *object);

	// Mouse listener
	static list<MouseListener*> s_mouseListeners;
	static void addMouseListener(MouseListener *object);
	static void removeMouseListener(MouseListener *object);

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
class XDAPI WindowListener
{
	friend class Window;
protected:
	WindowListener();
	virtual ~WindowListener();

	virtual void resizeEvent(uint width, uint height) {}
	virtual void moveEvent(uint x, uint y) {}
	//virtual void 
};

class Graphics;
class GraphicsContext;

class XDAPI Window
{
	friend class Graphics;
	friend class WindowListener;
	friend class Engine;
	friend class Input;
private:
	static void init(int w, int h, bool fs);

	static void focusChanged(GLFWwindow*, int);
	static void sizeChanged(GLFWwindow*, int, int);
	static void keyCallback(GLFWwindow*, int, int, int, int);
	static void charCallback(GLFWwindow*, uint);
	static void mouseButtonCallback(GLFWwindow *, int, int, int);
	static void cursorMoveCallback(GLFWwindow*, double, double);
	static void scrollCallback(GLFWwindow*, double, double);

public:
	static void close();
	
	static void setFullScreen(const bool fullscreen);
	static bool getFullScreen();

	static void setResizable(const bool resizable);
	static bool getResizable();

	static bool hasFocus();
	
	static void setPosition(const Vector2i &pos);
	static Vector2i getPosition();

	static void setSize(const int width, const int height);
	static void setSize(const Vector2i &size) { setSize(size.x, size.y); }
	static void setWidth(const int width);
	static void setHeight(const int height);
	static Vector2i getSize();
	static int getWidth();
	static int getHeight();

	static void minimize();
	static void restore();
	//static void maximize();

private:
	// The window handle
	static GLFWwindow *s_window;

	// Cached list of resolutions
	//static vector<Vector2i> s_resolutions;

	// Fullscreen
	static bool s_focus;
	static bool s_fullScreen;

	// Windows graphics device
	static GraphicsContext *s_graphicsContext;

	// Window listeners
	static list<WindowListener*> s_windowListeners;
	static void addWindowListener(WindowListener *listener);
	static void removeWindowListener(WindowListener *listener);
	
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
	function<void()> stepBeginFunc, stepEndFunc, endFunc;
	function<void(GraphicsContext&)> mainFunc;
	function<void(GraphicsContext&, const double)> drawFunc;
	function<void(const double)> updateFunc;
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
	function<void()> m_stepBeginFunc, m_stepEndFunc, m_endFunc;
	function<void(GraphicsContext&)> m_mainFunc;
	function<void(GraphicsContext&, const double)> m_drawFunc;
	function<void(const double)> m_updateFunc;
};

XDAPI Engine *CreateEngine();

END_XD_NAMESPACE

#endif // X2D_ENGINE_H
