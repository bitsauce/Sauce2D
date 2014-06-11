#ifndef WIN_WINDOW_H
#define WIN_WINDOW_H

#include <x2d/config.h>
#include <x2d/util.h>
#include <x2d/input.h>
#include <x2d/window.h>
#include <x2d/math.h>

class OpenGL;
class Input;
class xdEngine;

// The window class:
// It wraps the handle of the window and initializes the OpenGL rendering context.
// It is also in charge of processing the different event messages.
class Window : public xdWindow
{
public:
	Window(xdEngine *engine, Input *input, OpenGL *graphics);
	~Window();
	
	// Application config
	struct Config
	{
		// Metrics
		int width;
		int height;
		int x;
		int y;

		// Work dir
		string workDir;

		// Icon
		string iconFile;

		// Title
		string title;

		// Flags
		bool centered;
		bool fullscreen;
		int windowFlags;

		Config() :
			width(800),
			height(600),
			x(0),
			y(0),
			workDir(""),
			iconFile(""),
			title(""),
			centered(true),
			fullscreen(false),
			windowFlags(0)
		{
		}
	};

	// Close window
	void close();
	
	// Window events
	void initEvents();
	void processEvents();

	// Display exception
	//void exception(xdRetCode error, const char* message);

	// Window functions
	void enableFullscreen();
	void disableFullscreen();
	class Array *resolutionList() const;

	void enableResize();
	void disableResize();
	bool hasFocus();

	void setPosition(const Vector2i &pos);
	Vector2i getPosition() const;
	void setSize(const Vector2i &size);
	Vector2i getSize() const;
	void minimize();
	void maximize();
	void restore();

	// Vsync
	void enableVsync();
	void disableVsync();

private:

	// Show window
	void showWindow();

	// Configure window
	void readConfig();

	// Processes window events
	void processEvents(UINT Message, WPARAM wParam, LPARAM lParam);

	// Engine handle
	xdEngine *m_engine;

	// Gfx handle
	OpenGL *m_graphics;
	Input *m_input;
	
	// The window handle
	HWND m_window;

	// The window device context
    HDC m_deviceContext;
	
	// The event message
    MSG m_message;

	// The previous update time
	DWORD m_previousTime;

	// Cached list of resolutions
	vector<Vector2i> m_resolutions;

	// Window focus
	bool m_focus;

	// Window size
	Vector2i m_size;

	// Window fullscreen state
	bool m_fullscreen;

	// Window config
	Config m_config;

	// Window events init
	bool m_initEventsDone;

	// Event functions
	void *m_focusLostFunc;
	void *m_focusGainedFunc;
	void *m_windowResizedFunc;
	void *m_leftMouseDownFunc;
	void *m_leftMouseUpFunc;
	void *m_rightMouseDownFunc;
	void *m_rightMouseUpFunc;
	void *m_middleMouseDownFunc;
	void *m_middleMouseUpFunc;
	void *m_mouseMoveFunc;
	void *m_mouseWheelFunc;
	void *m_inputCharFunc;

public:
	
	// Window procedure callback
    static LRESULT CALLBACK OnEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
};

#endif // WIN_WINDOW_H