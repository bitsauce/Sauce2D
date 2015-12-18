#ifndef X2D_ENGINE_H
#define X2D_ENGINE_H

#include <x2d/config.h>
#include <x2d/math.h>
#include <x2d/resourcemanager.h>
#include <x2d/iniparser.h>
#include <x2d/input/input.h>

BEGIN_CG_NAMESPACE

#define LOG(str, ...) Console::Log(str, __VA_ARGS__)
#define THROW(str, ...) throw Exception(X2D_RUNTIME_EXCEPTION, str, __VA_ARGS__)

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
	friend class Game;
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
	
	Game *m_engine;
	FileWriter *m_output;

	static Console *s_this;
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
// TODO: Needs re-doing with SDL in mind
class XDAPI FileSystem
{
	friend class Game;
public:
	// File buffers
	bool readFile(string filePath, string &conent) const;
	bool writeFile(string filePath, const string content) const;

	// OS specifics
	bool fileExists(string &filePath) const;

	// Static functions
	static bool ReadFile(string path, string &content);
	static bool WriteFile(string path, const string &content);
	static bool MakeDir(string path);

protected:
	bool makeDir(const string &path);

private:
	static FileSystem *s_this;
};

/*********************************************************************
**	Window class													**
**********************************************************************/
class Graphics;
class GraphicsContext;
class Pixmap;

class XDAPI Window
{
public:
	Window(const string &title, const int x, const int y, const int w, const int h, const Uint32 flags);
	~Window();
	
	/** 
	 * \brief Set the window to fullscreen using the given \p displayMode.
	 * If \p displayMode is 0 or invalid, then setFullScreenMode will use the
	 * current desktop display mode.
	 * TODO: Multi-monitor support
	 */
	void setFullScreenMode(SDL_DisplayMode *displayMode);

	/**
	 * \brief Sets the window to windowed mode.
	 */
	void setWindowed();

	/**
	 * \brief Gets the current window flags.
	 */
	Uint32 getFlags() const;

	/**
	 * \brief Returns true if all the flags in \p flags is set.
	 */
	bool checkFlags(const Uint32 flags) const;

	/**
	 * \brief Moves the window to a given position in desktop coordinates.
	 */
	void setPosition(const int x, const int y);

	/**
	 * \brief Gets the position of window in desktop coordinates.
	 */
	void getPosition(int *x, int *y) const;

	/**
	 * \brief Resizes the window to a given size in desktop coordinates.
	 */
	void setSize(const int width, const int height);

	/**
	 * \brief Gets the size of window in desktop coordinates.
	 */
	void getSize(int *x, int *y) const;

	/**
	 * \brief Sets the title of the window.
	 */
	void setTitle(const string &title);

	/**
	 * \brief Gets the title of the window.
	 */
	string getTitle() const;

	/**
	 * \brief Sets the icon of the window.
	 */
	void setIcon(const Pixmap &icon);

	/**
	 * \brief Sets the minimum size of the window in desktop coordinates.
	 */
	void setMinimumSize(const int width, const int height);

	/**
	 * \brief Sets the maximum size of the window in desktop coordinates.
	 */
	void setMaximumSize(const int width, const int height);

	/**
	 * \brief If true applies borders to the window. If false removes borders from the window.
	 */
	void setBordered(const bool bordered);

	/**
	 * \brief If true the window will try to grab user input.
	 */
	void setGrab(const bool grabbed);

	/**
	 * \brief Set the brightness of the window?
	 */
	void setBrightness(const float brightness);

	/**
	 * \brief Set gamma ramp of the window?
	 */
	void setGammaRamp(Uint16 *red, Uint16 *green, Uint16 *blue);

	/**
	 * \brief Hides the window?
	 */
	void hide();

	/**
	 * \brief Shows the window?
	 */
	void show();

	/**
	 * \brief Get display index of window?
	 */
	int getDisplayIndex() const;

	/**
	 * \brief Get display mode?
	 */
	void getDisplayMode(SDL_DisplayMode *mode) const;

	/**
	 * \brief Maximizes the window.
	 */
	void maximize();

	/**
	 * \brief Minimizes the window.
	 */
	void minimize();

	/**
	 * \brief Restores the window if minimized.
	 */
	void restore();

	/**
	 * \brief Sets the vsync mode. 0 disables vsync, 1 enables vsync and -1 enables adaptive vsync.
	 */
	void setVSync(const int mode);


	/**
	 * \brief 
	 */
	GraphicsContext *getGraphicsContext() const;

	/**
	 * \brief Returns the SDL_Window handle of this window.
	 */
	SDL_Window *getSDLHandle() const;

private:
	// SDL window object
	SDL_Window *m_window;

	// Graphics context
	GraphicsContext *m_graphicsContext;
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
**	Game class														**
**********************************************************************/
class XDAPI Game
{
public:
	Game(const string &name, const string &organization = "CrossGame");
	~Game();

	/**
	 * \brief An event called after the game has initialized.
	 */
	virtual void onStart(GraphicsContext&) { }

	/**
	 * \brief An event called as the game is ending.
	 */
	virtual void onEnd() { }

	/**
	 * \brief An event called when the game wants to update the game state.
	 */
	virtual void onUpdate(const float delta) { }

	/**
	 * \brief An event called when the game wants to draw the current game state.
	 */
	virtual void onDraw(GraphicsContext&, const float alpha) { }

	/**
	 * \brief An event called at the start of the game loop.
	 */
	virtual void onStepBegin() { }
	
	/**
	 * \brief An event called at the end of the game loop.
	 */
	virtual void onStepEnd() { }
	
	virtual void onSizeChanged(int width, int height) { }

	virtual void onKeyDown(int vk, int mod, int scancode) { }

	virtual void onKeyUp(int vk, int mod, int scancode) { }

	virtual void onMouseMove(float x, float y) { }

	virtual void onMouseDown(int btn) { }

	virtual void onMouseUp(int btn) { }

	virtual void onMouseWheel(int x, int y) { }

public:
	// Run game
	int run();
	
	// End game
	void end();

	// Set pause state
	void setPaused(const bool paused);

	// Check game config
	bool isEnabled(const EngineFlag flag);

	// Get working dir
	string getRootDirectory() const { return m_rootDir; }

	// Get save dir
	string getPrefDirectory() const { return m_saveDir; }

	Window *getMainWindow() const { return m_window; }

private:

	// Game name and organization
	const string m_name;
	const string m_organization;
	
	// Initialized?
	bool m_initialized;

	// Paused?
	bool m_paused;

	// Running?
	bool m_running;

	// Fps
	double m_framesPerSecond;
	
	Window *m_window;
	FileSystem		*m_fileSystem;
	//AudioManager	*m_audio;
	Timer			*m_timer;
	Console			*m_console;

	// Engine running flags
	uint m_flags;

	// Engine working directory
	string m_rootDir, m_saveDir;
};

END_CG_NAMESPACE

#endif // X2D_ENGINE_H
