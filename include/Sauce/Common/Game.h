/**
 * \def	LOG(str, ...) Console::Log(str, __VA_ARGS__);
 *
 * \brief	A macro that defines log.
 *
 * \todo TODO: Add timestap to all console messages
 *
 * \param	str	The string.
 * \param	...	Variable arguments providing additional information.
 */

#define LOG(str, ...) Console::Log(__FUNCTION__, __LINE__, str, __VA_ARGS__)

/**
 * \def	THROW(str, ...) throw Exception(SAUCE_RUNTIME_EXCEPTION, str, __VA_ARGS__);
 *
 * \brief	A macro that defines throw.
 *
 * \param	str	The string.
 * \param	...	Variable arguments providing additional information.
 */

#define THROW(str, ...) throw Exception(SAUCE_RUNTIME_EXCEPTION, str, __VA_ARGS__)

#ifndef SAUCE_ENGINE_H
#define SAUCE_ENGINE_H

#include <Sauce/Config.h>
#include <Sauce/Math.h>
#include <Sauce/Input.h>

#include <Sauce/Common/ResourceManager.h>
#include <Sauce/Common/IniParser.h>
#include <Sauce/Common/Event.h>

BEGIN_SAUCE_NAMESPACE

/*********************************************************************
**	Game console													**
**********************************************************************/

class FileSystem;
class FileReader;
class FileWriter;
class AudioManager;
class Graphics;

/**
 * \class	FileReader
 *
 * \brief	A file reader.
 */

class SAUCE_API Console
{
	friend class Game;
public:

	/**
	 * \fn	Console::Console();
	 *
	 * \brief	Default constructor.
	 */

	Console();
	~Console();

	/**
	 * \fn	void Console::log(const string &msg);
	 *
	 * \brief	Ouputs \p msg to the console
	 *
	 * \param	msg	The message.
	 */

	//void log(const string &msg);

	/**
	 * \fn	void Console::logf(const char *msg, ...);
	 *
	 * \brief	Logfs.
	 *
	 * \param	msg	The message.
	 * \param	...	Variable arguments providing additional information.
	 */

	void logf(const char *msg, ...);

	/**
	 * \fn	string Console::getLog() const;
	 *
	 * \brief	Gets the log.
	 *
	 * \return	The log.
	 */

	string getLog() const;

	/**
	 * \fn	void Console::clear();
	 *
	 * \brief	Clears this object to its blank/initial state.
	 */

	void clear();

	/**
	 * \fn	void Console::exportFile() const;
	 *
	 * \brief	Export file.
	 */

	void exportFile() const;

	/**
	 * \fn	string Console::readBuffer();
	 *
	 * \brief	Reads the buffer.
	 *
	 * \return	The buffer.
	 */

	string readBuffer();
	bool hasBuffer() const;
	void clearBuffer();

	static void Log(const char *function, const int line, const char *msg, ...);

private:

	 /**
	  * \fn	* fn void Console::call_log(const char *msg, va_list args);
	  *
	  * \brief	Call log.
	  *
	  * \param	msg 	The message.
	  * \param	args	The arguments.
	  */
	void call_log(const char *msg, va_list args);

	/** \brief	The log. */
	string m_log;

	/** \brief	The buffer. */
	string m_buffer;
	
	/** \brief	The engine. */
	Game *m_engine;
	
	/** \brief	The output. */
	FileWriter *m_output;

	static Console *s_this;
///< .
};

/*********************************************************************
**	Event handler													**
**********************************************************************/

class SAUCE_API Timer
{
public:

	 /**
	  * \fn	* fn Timer::Timer();
	  *
	  * \brief	Default constructor.
	  */

	Timer();
	
	// High-resolution timing
	void  start();

	/**
	 * \fn	void Timer::stop();
	 *
	 * \brief	Stops this object.
	 */

	void  stop();
	double getElapsedTime() const;

private:

	/** \brief	The frequency. */
	LARGE_INTEGER m_frequency;

	/** \brief	The start. */
	LARGE_INTEGER m_start;

	/** \brief	The end. */
	LARGE_INTEGER m_end;

	/** \brief	true to running. */
	bool m_running;
};

class SAUCE_API SimpleTimer
{
public:
	SimpleTimer();

	void start();
	float stop();

	float getElapsedTime() const;

private:
	clock_t m_startTick;
	bool m_running;
};

/*********************************************************************
**	Thread manager													**
**********************************************************************/

class SAUCE_API ThreadManager
{
public:

	/**
	 * \fn	virtual void ThreadManager::setupThread() = 0;
	 *
	 * \brief	Sets up the thread.
	 */

	virtual void setupThread() = 0;

	/**
	 * \fn	virtual void ThreadManager::cleanupThread() = 0;
	 *
	 * \brief	Cleanup thread.
	 */

	virtual void cleanupThread() = 0;
///< .
};

/*********************************************************************
**	File reader class												**
**********************************************************************/

class SAUCE_API FileReader
{
public:
	FileReader(const string &path);

	/**
	 * \fn	FileReader::~FileReader();
	 *
	 * \brief	Destructor.
	 */

	~FileReader();

	/**
	 * \fn	bool FileReader::isOpen();
	 *
	 * \brief	Query if this object is open.
	 *
	 * \return	true if open, false if not.
	 */

	bool isOpen();
	bool isEOF();
	void close();

	/**
	 * \fn	bool FileReader::readBytes(char *buffer, const int count);
	 *
	 * \brief	Reads the bytes.
	 *
	 * \param [in,out]	buffer	If non-null, the buffer.
	 * \param	count		  	Number of.
	 *
	 * \return	true if it succeeds, false if it fails.
	 */

	bool readBytes(char *buffer, const int count);
	string readLine();
	string readAll();

	FileReader &operator>>(int &i) { stream >> i; return *this; }

	/**
	 * \fn	FileReader FileReader::&operator>>(char &c)
	 *
	 * \brief	Bitwise right shift operator.
	 *
	 * \param [in,out]	c	The char to process.
	 *
	 * \return	The shifted result.
	 */

	FileReader &operator>>(char &c) { stream >> c; return *this; }

private:
	/** \brief	The stream. */
	/** \brief	The stream. */
	ifstream stream;
};

/*********************************************************************
**	File writer class												**
**********************************************************************/

class SAUCE_API FileWriter
{
public:
	FileWriter(const string &path);

	/**
	 * \fn	FileWriter::~FileWriter();
	 *
	 * \brief	Destructor.
	 */

	~FileWriter();

	bool isOpen();
	void close();

	void clear();
	void append(const char* data, const int length);

	/**
	 * \fn	void FileWriter::append(const string &str)
	 *
	 * \brief	Appends a str.
	 *
	 * \param	str	The string to append.
	 */

	void append(const string &str) { append(str.data(), str.size()); }
	void flush();

    /**
     * \fn	FileWriter FileWriter::&operator<<(basic_ostream<char, std::char_traits<char> >& (*fp)(basic_ostream<char, std::char_traits<char> >&))
     *
     * \brief	define an operator<< to take in std::endl.
     *
     * \param [in,out]	fp	If non-null, the fp.
     *
     * \return	The shifted result.
     */

    FileWriter &operator<<(basic_ostream<char, std::char_traits<char> >& (*fp)(basic_ostream<char, std::char_traits<char> >&)) { stream << fp; return *this; }

	/**
	 * \fn	bool FileWriter::operator!() const
	 *
	 * \brief	Logical negation operator.
	 *
	 * \return	The logical inverse of this value.
	 */

	bool operator!() const { return !stream; }
	FileWriter &operator<<(int i) { stream << i; return *this; }

	/**
	 * \fn	FileWriter FileWriter::&operator<<(char c)
	 *
	 * \brief	Bitwise left shift operator.
	 *
	 * \param	c	The character.
	 *
	 * \return	The shifted result.
	 */

	FileWriter &operator<<(char c) { stream << c; return *this; }

private:
	/** \brief	The stream. */
	/** \brief	The stream. */
	ofstream stream;
};

/*********************************************************************
**	File writer class												**
**********************************************************************/

class SAUCE_API FileSystemIterator
{
public:
	FileSystemIterator(string path, const string &mask, const int flags);
	//~FileSystemIterator();

	/**
	 * \enum	Flag
	 *
	 * \brief	Values that represent flags.
	 */

	enum Flag
	{
		///< An enum constant representing the directories option
		DIRECTORIES = 1 << 1,
		///< An enum constant representing the files option
		FILES = 1 << 2
	};

	/**
	 * \fn	bool FileSystemIterator::hasNext() const;
	 *
	 * \brief	Query if this object has next.
	 *
	 * \return	true if next, false if not.
	 */

	bool hasNext() const;
	string &next();

	operator bool() const { return hasNext(); }

private:
	/** \brief	The files. */
	/** \brief	The files. */
	/** \brief	The files. */
	vector<string> m_files;
	/** \brief	The itr. */
	vector<string>::iterator m_itr;
};


/*********************************************************************
**	File system class												**
**********************************************************************/

/**
 * \class	SAUCE_API
 *
 * \brief	TODO: Needs re-doing with SDL in mind.
 */

class SAUCE_API FileSystem
{
	/**
	 * \class	Game
	 *
	 * \brief	A game.
	 */

	friend class Game;
public:

	/**
	 * \fn	bool FileSystem::readFile(string filePath, string &conent) const;
	 *
	 * \brief	File buffers.
	 *
	 * \param	filePath	  	Full pathname of the file.
	 * \param [in,out]	conent	The conent.
	 *
	 * \return	true if it succeeds, false if it fails.
	 */

	bool readFile(string filePath, string &conent) const;
	bool writeFile(string filePath, const string content) const;

	/**
	 * \fn	bool FileSystem::fileExists(string &filePath) const;
	 *
	 * \brief	OS specifics.
	 *
	 * \param [in,out]	filePath	Full pathname of the file.
	 *
	 * \return	true if it succeeds, false if it fails.
	 */

	bool fileExists(string &filePath) const;

	// Static functions
	static bool ReadFile(string path, string &content);
	static bool WriteFile(string path, const string &content);

	/**
	 * \fn	static bool FileSystem::MakeDir(string path);
	 *
	 * \brief	Makes a dir.
	 *
	 * \param	path	Full pathname of the file.
	 *
	 * \return	true if it succeeds, false if it fails.
	 */

	static bool MakeDir(string path);

protected:
	bool makeDir(const string &path);

private:
	/** \brief	this. */
	static FileSystem *s_this;
};

/*********************************************************************
**	Window class													**
**********************************************************************/

class Graphics;
class GraphicsContext;
class Pixmap;

class SAUCE_API Window
{
public:

	/**
	 * \fn	Window::Window(const string &title, const int x, const int y, const int w, const int h, const Uint32 flags);
	 *
	 * \brief	Constructor.
	 *
	 * \param	title	The title.
	 * \param	x	 	The int to process.
	 * \param	y	 	The int to process.
	 * \param	w	 	The int to process.
	 * \param	h	 	The int to process.
	 * \param	flags	The flags.
	 */

	Window(const string &title, const int x, const int y, const int w, const int h, const Uint32 flags);

	/**
	 * \fn	Window::~Window();
	 *
	 * \brief	Destructor.
	 */

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

	Vector2I getSize() const
	{
		Vector2I size;
		getSize(&size.x, &size.y);
		return size;
	}

	int getWidth() const
	{
		return getSize().x;
	}
	
	int getHeight() const
	{
		return getSize().y;
	}

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
	 * \brief Returns the GraphicsContext of the window.
	 */
	GraphicsContext *getGraphicsContext() const;

	/**
	 * \fn	bool Window::handleEvent(SDL_Event &event, Game *game);
	 *
	 * \brief	Handle window input events.
	 *
	 * \param [in,out]	event	If non-null, the event.
	 * \param [in,out]	game 	If non-null, the game.
	 *
	 * \return	true if the window was closed, else it returns false.
	 */

	bool handleEvent(SDL_Event &event, Game *game);

	/**
	 * \fn	Uint32 Window::getWindowID() const;
	 *
	 * \brief	Gets window identifier.
	 *
	 * \return	The window identifier.
	 */

	Uint32 getID() const;

	/**
	 * \brief Returns the SDL_Window handle of this window.
	 */
	SDL_Window *getSDLHandle() const;

private:
	/** \brief	SDL window object. */
	SDL_Window *m_window;

	/** \brief	Graphics context. */
	GraphicsContext *m_graphicsContext;
};

/*********************************************************************
**	Exception class													**
**********************************************************************/

class SAUCE_API Exception
{
public:

	/**
	 * \fn	Exception::Exception(RetCode code, const char * msg, ...);
	 *
	 * \brief	Constructor.
	 *
	 * \param	code	The code.
	 * \param	msg 	The message.
	 * \param	... 	Variable arguments providing additional information.
	 */

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
	/** \brief	The message. */
	string m_message;

	/** \brief	The error code. */
	RetCode m_errorCode;
};

#define PROPAGATE_EVENT(eventFunc) \
	list<SceneObject*> children(m_children); \
	for(SceneObject *child : children) \
	{ \
		child->eventFunc(e); \
	}

/**
* \class	SceneObject
*
* \brief	An game object.
*/
class SAUCE_API SceneObject
{
public:
	/**
	 * \fn	virtual void EventObject::onEvent(Event *event)
	 *
	 * \brief	Executes the event action.
	 *
	 * \param [in,out]	event	If non-null, the event.
	 */

	virtual void onEvent(Event *e)
	{
		PROPAGATE_EVENT(onEvent);
	}

	/**
	* \fn	virtual void Game::onStart(GameEvent*)
	*
	* \brief	An event called after the game has initialized.
	*
	* \param [in,out]	parameter1	If non-null, the first parameter.
	*/

	virtual void onStart(GameEvent *e)
	{
		PROPAGATE_EVENT(onStart);
	}

	/**
	* \fn	virtual void Game::onEnd(GameEvent*)
	*
	* \brief	An event called as the game is ending.
	*
	* \param [in,out]	parameter1	If non-null, the first parameter.
	*/

	virtual void onEnd(GameEvent *e)
	{
		PROPAGATE_EVENT(onEnd);
	}

	/**
	* \fn	virtual void Game::onTick(TickEvent*)
	*
	* \brief	An event called when the game wants to update the game state.
	*
	* \param [in,out]	parameter1	If non-null, the first parameter.
	*/

	virtual void onTick(TickEvent *e)
	{
		PROPAGATE_EVENT(onTick);
	}

	/**
	 * \fn	virtual void SceneObject::onDraw(DrawEvent *e)
	 *
	 * \brief	An event called when the game wants to draw the current game state.
	 *
	 * \param [in,out]	e	The event parameters.
	 */

	virtual void onDraw(DrawEvent *e)
	{
		PROPAGATE_EVENT(onDraw);
	}

	/**
	* \fn	virtual void Game::onStepBegin(StepEvent*)
	*
	* \brief	An event called at the start of the game loop.
	*
	* \param [in,out]	parameter1	If non-null, the first parameter.
	*/

	virtual void onStepBegin(StepEvent *e)
	{
		PROPAGATE_EVENT(onStepBegin);
	}

	/**
	* \fn	virtual void Game::onStepEnd(StepEvent*)
	*
	* \brief	An event called at the end of the game loop.
	*
	* \param [in,out]	parameter1	If non-null, the first parameter.
	*/

	virtual void onStepEnd(StepEvent *e)
	{
		PROPAGATE_EVENT(onStepEnd);
	}

	/**
	* \fn	virtual void Game::onWindowSizeChanged(WindowEvent*)
	*
	* \brief	Executes the size changed action.
	*
	* \param [in,out]	parameter1	If non-null, the first parameter.
	*/

	virtual void onWindowSizeChanged(WindowEvent *e)
	{
		PROPAGATE_EVENT(onWindowSizeChanged);
	}

	/**
	 * \fn	virtual void EventObject::onKeyEvent(KeyEvent *e)
	 *
	 * \brief	Executes the key up action.
	 *
	 * \param [in,out]	e	If non-null, the KeyEvent to process.
	 */

	virtual void onKeyEvent(KeyEvent *e)
	{
		PROPAGATE_EVENT(onKeyEvent);

		switch(e->getType())
		{
			case EVENT_KEY_DOWN: onKeyDown(e); break;
			case EVENT_KEY_UP: onKeyUp(e); break;
			case EVENT_KEY_REPEAT: onKeyRepeat(e); break;
		}
	}

	virtual void onTextInput(TextEvent *e)
	{
		PROPAGATE_EVENT(onTextInput);
	}

	/**
	 * \fn	virtual void EventObject::onKeyDown(KeyEvent *e)
	 *
	 * \brief	Executes the key down action.
	 *
	 * \param [in,out]	e	If non-null, the KeyEvent to process.
	 */

	virtual void onKeyDown(KeyEvent *e)
	{
		PROPAGATE_EVENT(onKeyDown);
	}

	/**
	 * \fn	virtual void EventObject::onKeyUp(KeyEvent *e)
	 *
	 * \brief	Executes the key up action.
	 *
	 * \param [in,out]	e	If non-null, the KeyEvent to process.
	 */

	virtual void onKeyUp(KeyEvent *e)
	{
		PROPAGATE_EVENT(onKeyUp);
	}

	/**
	 * \fn	virtual void EventObject::onKeyRepeat(KeyEvent *e)
	 *
	 * \brief	Executes the key down action.
	 *
	 * \param [in,out]	e	If non-null, the KeyEvent to process.
	 */

	virtual void onKeyRepeat(KeyEvent *e)
	{
		PROPAGATE_EVENT(onKeyRepeat);
	}

	/**
	 * \fn	virtual void SceneObject::onMouseEvent(MouseEvent *e)
	 *
	 * \brief	Executes the mouse event action.
	 *
	 * \param [in,out]	e	If non-null, the MouseEvent to process.
	 */

	virtual void onMouseEvent(MouseEvent *e)
	{
		PROPAGATE_EVENT(onMouseEvent);

		switch(e->getType())
		{
			case EVENT_MOUSE_UP: onMouseUp(e); break;
			case EVENT_MOUSE_DOWN: onMouseDown(e); break;
			case EVENT_MOUSE_MOVE: onMouseMove(e); break;
			case EVENT_MOUSE_WHEEL: onMouseWheel(e); break;
		}
	}

	/**
	* \fn	virtual void Game::onMouseMove(MouseEvent *e)
	*
	* \brief	Executes the mouse move action.
	*
	* \param [in,out]	e	If non-null, the MouseEvent to process.
	*/

	virtual void onMouseMove(MouseEvent *e)
	{
		PROPAGATE_EVENT(onMouseMove);
	}

	/**
	* \fn	virtual void Game::onMouseDown(MouseEvent *e)
	*
	* \brief	Executes the mouse down action.
	*
	* \param [in,out]	e	If non-null, the MouseEvent to process.
	*/

	virtual void onMouseDown(MouseEvent *e)
	{
		PROPAGATE_EVENT(onMouseDown);
	}

	/**
	* \fn	virtual void Game::onMouseUp(MouseEvent *e)
	*
	* \brief	Executes the mouse up action.
	*
	* \param [in,out]	e	If non-null, the MouseEvent to process.
	*/

	virtual void onMouseUp(MouseEvent *e)
	{
		PROPAGATE_EVENT(onMouseUp);
	}

	/**
	* \fn	virtual void Game::onMouseWheel(MouseEvent *e)
	*
	* \brief	Executes the mouse wheel action.
	*
	* \param [in,out]	e	If non-null, the MouseEvent to process.
	*/

	virtual void onMouseWheel(MouseEvent *e)
	{
		PROPAGATE_EVENT(onMouseWheel);
	}

	/**
	 * \fn	void SceneObject::addChildFirst(SceneObject *child)
	 *
	 * \brief	Adds a child first.
	 *
	 * \param [in,out]	child	If non-null, the child.
	 */

	void addChildFirst(SceneObject *child)
	{
		if(!child) return;
		m_children.push_front(child);
		child->m_parent = this;
	}

	/**
	 * \fn	void SceneObject::addChildLast(SceneObject *child)
	 *
	 * \brief	Adds a child last.
	 *
	 * \param [in,out]	child	If non-null, the child.
	 */

	void addChildLast(SceneObject *child)
	{
		if(!child) return;
		m_children.push_back(child);
		child->m_parent = this;
	}

	/**
	 * \fn	void SceneObject::removeChild(SceneObject *child)
	 *
	 * \brief	Removes child.
	 *
	 * \param [in,out]	child	If non-null, the child.
	 */

	void removeChild(SceneObject *child)
	{
		if(!child) return;
		m_children.remove(child);
	}

	/**
	 * \fn	void removeChildFront()
	 *
	 * \brief	Removes the first child.
	 */

	void removeChildFront()
	{
		if(m_children.empty()) return;
		m_children.pop_front();
	}

	/**
	 * \fn	void removeChildLast()
	 *
	 * \brief	Removes the last child.
	 */

	void removeChildLast()
	{
		if(m_children.empty()) return;
		m_children.pop_back();
	}

	/**
	 * \fn	list<SceneObject*> getChildren() const
	 *
	 * \brief	Gets the children of this item.
	 *
	 * \return	null if it fails, else the children.
	 */

	list<SceneObject*> getChildren() const
	{
		return m_children;
	}

	/**
	 * \fn	SceneObject *getParent() const
	 *
	 * \brief	Gets the parent of this item.
	 *
	 * \return	null if it fails, else the parent.
	 */

	SceneObject *getParent() const
	{
		return m_parent;
	}

	/**
	 * \fn	void SceneObject::setUserData(void *data)
	 *
	 * \brief	Sets user data.
	 *
	 * \param [in,out]	data	If non-null, the data.
	 */

	void setUserData(void *data)
	{
		m_userData = data;
	}

	/**
	 * \fn	void SceneObject::*getUserData()
	 *
	 * \brief	Gets user data.
	 *
	 * \return	null if it fails, else the user data.
	 */

	void *getUserData()
	{
		return m_userData;
	}

private:
	// TODO: Consider using shared_ptr for SceneObject*
	list<SceneObject*> m_children;
	SceneObject *m_parent;
	bool m_eventPropagated;
	void *m_userData;
};

class SAUCE_API Scene
{
	friend class Game;
public:
	SceneObject *getRoot() const
	{
		return m_root;
	}

private:
	Scene(SceneObject *root) :
		m_root(root)
	{
	}

	SceneObject *m_root;
};

class SpriteBatch;

class SAUCE_API Game : public SceneObject
{
public:
	Game(const string &name, const string &organization = "CrossGame");
	~Game();

public:
	// Run game
	int run();
	
	// End game
	void end();

	// Set pause state
	void setPaused(const bool paused);

	// Check game config
	void setFlags(const uint flags);
	uint getFlags() const;
	bool isEnabled(const EngineFlag flag);

	// Get binary path
	string getBinaryPath() const { return m_binaryPath; }

	// Get save dir
	string getPrefPath() const { return m_prefPath; }

	/**
	 * \fn	Window Game::*getWindow(const Sint32 id = -1) const;
	 *
	 * \brief	Get a game window.
	 *
	 * \param	id	The identifier. If less than 0, the functon will return the first window created.
	 *
	 * \return	null a window with \p id does not exist, else it returns the window.
	 */

	Window *getWindow(const Sint32 id = -1) const;

	/**
	 * \fn	float Game::getFPS() const
	 *
	 * \brief	Gets the FPS.
	 *
	 * \return	The FPS.
	 */

	float getFPS() const
	{
		return m_framesPerSecond;
	}

	InputManager *getInputManager()
	{
		return m_inputManager;
	}

	ResourceManager *getResourceManager()
	{
		return m_resourceManager;
	}

	Scene *getScene()
	{
		return m_scene;
	}

	/**
	* \fn	virtual void EventObject::onEvent(Event *event)
	*
	* \brief	Executes the event action.
	*
	* \param [in,out]	event	If non-null, the event.
	*/

	virtual void onEvent(Event *e)
	{
		SceneObject::onEvent(e);

		// Find and call the specific event function
		switch(e->getType())
		{
			case EVENT_START: onStart(static_cast<GameEvent*>(e)); break;
			case EVENT_END: onEnd(static_cast<GameEvent*>(e)); break;
			case EVENT_TICK: onTick(static_cast<TickEvent*>(e)); break;
			case EVENT_DRAW: onDraw(static_cast<DrawEvent*>(e)); break;
			case EVENT_STEP_BEGIN: onStepBegin(static_cast<StepEvent*>(e)); break;
			case EVENT_STEP_END: onStepEnd(static_cast<StepEvent*>(e)); break;
			case EVENT_KEY_DOWN:
			case EVENT_KEY_UP:
			case EVENT_KEY_REPEAT: onKeyEvent(static_cast<KeyEvent*>(e)); break;
			case EVENT_TEXT_INPUT: onTextInput(static_cast<TextEvent*>(e));
			case EVENT_MOUSE_UP:
			case EVENT_MOUSE_DOWN:
			case EVENT_MOUSE_MOVE:
			case EVENT_MOUSE_WHEEL: onMouseEvent(static_cast<MouseEvent*>(e)); break;
			case EVENT_WINDOW_SIZE_CHANGED: onWindowSizeChanged(static_cast<WindowEvent*>(e)); break;
		}
	}

	static Game *GetInstance()
	{
		return s_game;
	}

private:

	/**
	 * \property	const string m_name, m_organization
	 *
	 * \brief	Game name and organization.
	 *
	 * \return	The m organization.
	 */

	const string m_name, m_organization;
	
	/** \brief	Initialized? */
	bool m_initialized;

	/** \brief	Paused? */
	bool m_paused;

	/** \brief	Running? */
	bool m_running;

	/** \brief	Fps. */
	double m_framesPerSecond;
	
	/** \brief	Game windows. */
	list<Window*> m_windows;

	/** \brief	The file system. */
	FileSystem		*m_fileSystem;
	
	//AudioManager	*m_audio;
	
	ResourceManager *m_resourceManager;
	
	InputManager *m_inputManager;

	Scene *m_scene;

	//SpriteBatch *m_spriteBatch;
	
	/** \brief	The timer. */
	Timer			*m_timer;
	
	/** \brief	The console. */
	Console			*m_console;

	/** \brief	Engine running flags. */
	uint m_flags;

	/**
	 * \property	string m_binaryPath, m_prefPath
	 *
	 * \brief	Engine working directory.
	 *
	 * \return	The full pathname of the preference file.
	 */

	string m_binaryPath, m_prefPath;

	// Static game
	static Game *s_game;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_ENGINE_H
