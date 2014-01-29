/**
  *	\brief The Application class is responsible for performing all OS specific commands.
  */
class Application
{
public:
	/**
	  * Make a windowed application go fullscreen
	  * \warning This function will fail if the current window resolution is not suported as a fullscreen resolution on the graphics card. Use appGetResolutionList() to get a list of supported fullscreen resolutions
	  * \note Only for Windows, Mac and Linux
	  */
	void appEnableFullscreen() {}

	/**
	  * Make a fullscreen application go windowed
	  * \note Only for Windows, Mac and Linux
	  */
	void appDisableFullscreen() {}

	/**
	  * Enables manual resizing of the window
	  * \note Only for Windows, Mac and Linux
	  * \note Window resizing is enabled by default
	  */
	void appEnableResize() {}

	/**
	  * Disables manual resizing of the window
	  * \note Only for Windows, Mac and Linux
	  * \note Window resizing is enabled by default
	  */
	void appDisableResize() {}

	/**
	  * Returns a list of the supported desktop resolutions for this graphics card
	  * \note Only for Windows, Mac and Linux
	  */
	array<vec2> appGetResolutionList() {}

	/**
	  * Moves the application window to a desktop relative position
	  * \note Only for Windows, Mac and Linux
	  *	\param x The desktop relative x-coordinate
	  *	\param y The desktop relative y-coordinate
	  */
	void appMove(const int x, const int y) {}

	/**
	  * Returns the position of the application window in desktop coordinates
	  * \note Only for Windows, Mac and Linux
	  *	\param x Out x-coordinate
	  *	\param y Out y-coordinate
	  *	\param borders Include window borders. This has some issues.
	  */
	void appGetPos(int &x, int &y, bool borders = true) {}

	/**
	  * Resizes the application window
	  * \note Only for Windows, Mac and Linux
	  *	\param w Width of the new window in pixels (excluding borders)
	  *	\param h Height of the new window in pixels (excluding borders)
	  */
	void appResize(const int w, const int h) {}

	/**
	  * Returns the current application window size (excluding borders)
	  * \note Only for Windows, Mac and Linux
	  *	\param w Out width in pixels
	  *	\param h Out height in pixels
	  */
	void appGetSize(int &w, int &h) {}

	/**
	  * Minimizes the application window
	  * \note Only for Windows, Mac and Linux
	  */
	void appMinimize() {}

	/**
	  * Maximizes the application window
	  * \note Only for Windows, Mac and Linux
	  */
	void appMaximize() {}

	/**
	  * Restores the application window from the taskbar
	  * \note Only for Windows, Mac and Linux
	  */
	void appRestore() {}

	/** 
	  *	Moves the cursor to a desktop relative position
	  * \note Only for Windows, Mac and Linux
	  *	\param x The desktop relative x-coordinate
	  *	\param y The desktop relative y-coordinate
	  */
	void appSetCursorPos(const int x, const int y) {}

	/**
	  * Returns the desktop relative cursor position
	  * \note Only for Windows, Mac and Linux
	  * \param x Out x-coordinate
	  * \param y Out y-coordinate
	  */
	void appGetCursorPos(int &x, int &y) {}

	/**
	  * Limits the cursor movement to the specified desktop relative rectangle
	  * \note Only for Windows, Mac and Linux
	  * \param x The x-coordinate of the rectangle
	  * \param y The y-coordinate of the rectangle
	  *	\param w The width of the rectangle
	  *	\param h The height of the rectangle
	  */
	void appSetCursorLimits(const int x, const int y, const int w, const int h) {}

	/**
	  * Shows the system cursor, if hidden
	  * \note Only for Windows, Mac and Linux
	  */
	void appShowCursor() {}

	/**
	  * Hides the system cursor, if visible
	  * \note Only for Windows, Mac and Linux
	  */
	void appHideCursor() {}

	/**
	  * Returns the working directory (where the executable is located)
	  * \note Only for Windows, Mac and Linux
	  */
	string appGetWorkDir() {}
	
	/**
	  * Returns a working directory relative path. The same as appGetWorkDir() + path
	  * \note Only for Windows, Mac and Linux
	  * \param path A working directory relative path
	  */
	string appGetWorkPath(const string &path) {}

	/**
	  * Returns the program run-time in ticks
	  */
	uint64 appGetProgramTime() {}
	
	/**
	  * Returns the system run-time in ticks
	  * \note On windows this will overflow after 48 days
	  */
	uint64 appGetTickCount() {}
	
	/**
	  * Restart high-resoulution timer
	  */
	void appRestartHrTimer() {}
	
	/**
	  * Returns the high-resoultion time
	  */
	float appGetHrTime() {}
};