#ifndef WIN_WINDOW_H
#define WIN_WINDOW_H

#include <x2d/x2d.h>

class OpenGL;
class Input;

extern int loadEvents(asIScriptEngine*);

// The window class:
// It wraps the handle of the window and initializes the OpenGL rendering context.
// It is also in charge of processing the different event messages.
class Window : public XWindow
{
public:
	Window(XEngine *engine, Input *input, OpenGL *graphics);
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
	void initEvents(asIScriptEngine*);
	void processEvents();

	// Display exception
	//void exception(xdRetCode error, const char* message);

	// Window functions
	void enableFullscreen();
	void disableFullscreen();
	bool isFullscreen() const;
	XScriptArray *getResolutionList() const;

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
	XEngine *m_engine;

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
	asIScriptFunction *m_focusLostFunc;
	asIScriptFunction *m_focusGainedFunc;
	asIScriptFunction *m_windowResizedFunc;
	asIScriptFunction *m_leftMouseDownFunc;
	asIScriptFunction *m_leftMouseUpFunc;
	asIScriptFunction *m_rightMouseDownFunc;
	asIScriptFunction *m_rightMouseUpFunc;
	asIScriptFunction *m_middleMouseDownFunc;
	asIScriptFunction *m_middleMouseUpFunc;
	asIScriptFunction *m_mouseMoveFunc;
	asIScriptFunction *m_mouseWheelFunc;
	asIScriptFunction *m_inputCharFunc;
	asIScriptFunction *m_shutdownFunc;

public:
	
	// Window procedure callback
    static LRESULT CALLBACK OnEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
};

#endif // WIN_WINDOW_H