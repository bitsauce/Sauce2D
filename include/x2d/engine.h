#ifndef X2D_ENGINE_H
#define X2D_ENGINE_H

#include "config.h"
#include "math.h"
#include "resourcemanager.h"
#include "iniparser.h"
#include "input/input.h"

BEGIN_XD_NAMESPACE

#define LOG(str, ...) Console::Log(str, __VA_ARGS__)
#define THROW(str, ...) throw Exception(X2D_RUNTIME_EXCEPTION, str, __VA_ARGS__)

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
	Exception(RetCode code, const char * msg, ...);

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
