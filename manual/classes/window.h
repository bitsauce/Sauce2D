/**
  *	\brief Responsible for window related operations.
  */
class Window
{
public:
	/**
	  * Make a windowed application go fullscreen
	  * \warning This function will fail if the current window resolution is not suported as a fullscreen resolution on the graphics card. Use appGetResolutionList() to get a list of supported fullscreen resolutions
	  * \note Only for Windows, Mac and Linux
	  */
	void enableFullscreen() {}

	/**
	  * Make a fullscreen application go windowed
	  * \note Only for Windows, Mac and Linux
	  */
	void disableFullscreen() {}

	/**
	  * Returns true if the window is in fullscreen mode
	  * \note Only for Windows, Mac and Linux
	  */
	bool isFullscreen() {}

	/**
	  * Enables manual resizing of the window
	  * \note Only for Windows, Mac and Linux
	  * \note Window resizing is enabled by default
	  */
	void enableResize() {}

	/**
	  * Disables manual resizing of the window
	  * \note Only for Windows, Mac and Linux
	  * \note Window resizing is enabled by default
	  */
	void disableResize() {}

	/**
	  * Returns a list of the supported desktop resolutions for this graphics card
	  * \note Only for Windows, Mac and Linux
	  */
	array<Vector2i> getResolutionList() const {}

	/**
	  * Moves the window to a desktop-relative position
	  * \note Only for Windows, Mac and Linux
	  *	\param x Desktop-relative x-coordinate
	  *	\param y Desktop-relative y-coordinate
	  */
	void setPosition(const int x, const int y) {}

	/**
	  * \overload
	  *	\param position Desktop-relative position
	  */
	void setPosition(const Vector2i &position) {}

	/**
	  * Returns the position of the window in desktop coordinates
	  * \note Only for Windows, Mac and Linux
	  */
	Vector2i getPosition() const {}

	/**
	  * Resizes the window
	  * \note Only for Windows, Mac and Linux
	  *	\param w Width of the new window in pixels (excluding borders)
	  *	\param h Height of the new window in pixels (excluding borders)
	  */
	void setSize(const int w, const int h) {}

	/**
	  * \overload
	  *	\param size New window size
	  */
	void setSize(const Vector2i &size) {}

	/**
	  * Returns the current window size (excluding borders)
	  * \note Only for Windows, Mac and Linux
	  */
	Vector2i getSize() const {}

	/**
	  * Minimizes the window
	  * \note Only for Windows, Mac and Linux
	  */
	void minimize() {}

	/**
	  * Maximizes the window
	  * \note Only for Windows, Mac and Linux
	  */
	void maximize() {}

	/**
	  * Restores the window from the taskbar
	  * \note Only for Windows, Mac and Linux
	  */
	void restore() {}
};