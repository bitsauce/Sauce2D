#ifndef WINAPP_H
#define WINAPP_H

// Windows headers
#include "x2d/platform.h"

// Abstact application class
#include "x2d/app.h"

// Vector
#include "x2d/math.h"

class OpenGLRender;
class X2DEngine;

// The window class:
// It wraps the handle of the window and initializes the OpenGL rendering context.
// It is also in charge of processing the different event messages.
class Win32App : public X2DApp
{
public:
	Win32App(X2DEngine *engine, OpenGLRender *gfx);
	
	void print(const string &msg, const X2DMessageType type);
	
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

	string getPlatformString() const { return "Windows"; }

	// Close window
	void close();
	
	// Window events
	void initEvents();
	void processEvents();

	// Display exception
	void exception(X2DRetCode error, const char* message);

	// Window functions
	void enableFullscreen();
	void disableFullscreen();
	class Array *resolutionList();
	void enableResize();
	void disableResize();
	bool hasFocus();

	void move(const int x, const int y);
	void pos(int &x, int &y, bool borders = true);
	void resize(const int width, const int height);
	void size(int &w, int &h);
	void minimize();
	void maximize();
	void restore();

	// Cursor functions
	void cursorPos(int &x, int &y) const;
	void setCursorPos(const int x, const int y);
	void setCursorLimits(const int x, const int y, const int w, const int h);

	// Keyboard
	bool getKeyState(const X2DVirtualKey key);

	// Time functions
	ulong getProgramTime();
	ulong getTickCount();
	void startHrTimer();
	float getHrTime();

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

private:

	// Engine handle
	X2DEngine *m_engine;

	// Gfx handle
	OpenGLRender *m_gfx;
	
	// The window handle
	HWND m_window;

	// The window device context
    HDC m_deviceContext;
	
	// The event message
    MSG m_message;

	// The previous update time
	DWORD m_previousTime;

	// Cached list of resolutions
	vector<vec2i> m_resolutions;

	// Window focus
	bool m_focus;

	// Window size
	vec2i m_size;

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

#endif // WINAPP_H