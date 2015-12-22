#ifndef CGF_ENGINE_H
#define CGF_ENGINE_H

#include <CGF/Config.h>
#include <CGF/Math.h>
#include <CGF/Input.h>

#include <CGF/Common/ResourceManager.h>
#include <CGF/Common/IniParser.h>

BEGIN_CGF_NAMESPACE

#define LOG(str, ...) Console::Log(str, __VA_ARGS__)
#define THROW(str, ...) throw Exception(CGF_RUNTIME_EXCEPTION, str, __VA_ARGS__)

/*********************************************************************
**	Game console													**
**********************************************************************/

class FileSystem;
class FileReader;
class FileWriter;
class AudioManager;
class Graphics;

class CGF_API Console
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
class CGF_API Timer
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
class CGF_API EventHandler
{
	virtual void processEvents() = 0;
};

/*********************************************************************
**	Thread manager													**
**********************************************************************/
class CGF_API ThreadManager
{
public:
	virtual void setupThread() = 0;
	virtual void cleanupThread() = 0;
};

/*********************************************************************
**	File reader class												**
**********************************************************************/
class CGF_API FileReader
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
class CGF_API FileWriter
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
class CGF_API FileSystemIterator
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
class CGF_API FileSystem
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

class CGF_API Window
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
class CGF_API Exception
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
class CGF_API Game
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

	// Get binary path
	string getBinaryPath() const { return m_binaryPath; }

	// Get save dir
	string getPrefPath() const { return m_prefPath; }

	// Get main window
	Window *getMainWindow() const { return m_window; }

	static Game *GetInstance() { return s_game; }

private:

	// Game name and organization
	const string m_name, m_organization;
	
	// Initialized?
	bool m_initialized;

	// Paused?
	bool m_paused;

	// Running?
	bool m_running;

	// Fps
	double m_framesPerSecond;
	
	Window          *m_window;
	FileSystem		*m_fileSystem;
	//AudioManager	*m_audio;
	Timer			*m_timer;
	Console			*m_console;

	// Engine running flags
	uint m_flags;

	// Engine working directory
	string m_binaryPath, m_prefPath;

	// Static game
	static Game *s_game;
};

END_CGF_NAMESPACE

#endif // CGF_ENGINE_H
