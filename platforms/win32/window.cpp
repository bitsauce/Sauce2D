//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "window.h"
#include "input.h"
#include <x2d/config.h>
#include <x2d/engine.h>
#include <x2d/math.h>
#include <x2d/scriptengine.h>
#include <x2d/scripts/funccall.h>
#include <x2d/ini.h>
#include <x2d/graphics.h>

#include "source/gfx/opengl/glgraphics.h"

// Time functions
#include <ctime>

// Window class name
#define WINDOW_CLASSNAME "x2D"

// Window title
#define WINDOW_TITLE "x2D Game Engine"

Window *g_window = 0;

int loadEvents(xdEngine *engine)
{
	g_window->initEvents(engine->getScriptEngine());
	return 0;
}

//--------------------------------------------------------------------
// Window
//--------------------------------------------------------------------

Window::Window(xdEngine *engine, Input *input, OpenGL *gfx) :
	m_engine(engine),
	m_graphics(gfx),
	m_input(input),
	m_size(0),
	m_initEventsDone(false),
	m_fullscreen(false)
{
	// Read configuration file
	readConfig();

	// Show the window
	showWindow();

	// Set global window for this source file
	g_window = this;
}

Window::~Window()
{
}

//--------------------------------------------------------------------
// Game loop
//--------------------------------------------------------------------

// Close window
void Window::close()
{
	// If we're fullscreen
	if(m_fullscreen)
	{
		// Remove the full screen setting
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);	
	}

	// If we have a OpenGL context
	m_graphics->destroyContext();

	// If we have a device context
	if(m_deviceContext)
	{
		// Release the device context
		ReleaseDC(m_window, m_deviceContext);
		m_deviceContext = NULL;
	}

	// Destroy our main window and unregister the window class
    DestroyWindow(m_window);
    UnregisterClass(TEXT(WINDOW_CLASSNAME), GetModuleHandle(NULL));

	// Close engine
	m_engine->exit();
}

// Event callback
void Window::processEvents()
{
	// Check for messages
	//
	// If the function returned with no timeout, it means that at 
	// least one message has been received, so process all of them.
	while(PeekMessage(&m_message, NULL, 0, 0, PM_REMOVE))
	{
		// If a message was waiting in the message queue, process it
		TranslateMessage(&m_message);
		DispatchMessage(&m_message);
	}
}

// Exception
/*void Window::exception(xdRetCode error, const char* message)
{
	// Show exception in message box
	MessageBox(m_window, message, "An Exception Occured", MB_OK);
}*/

//--------------------------------------------------------------------
// Initialation
//--------------------------------------------------------------------
#define IDI_ICON 101
void Window::showWindow()
{
	// Register the window class
    WNDCLASSEX windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style		  = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
    windowClass.lpfnWndProc   = &Window::OnEvent;
    windowClass.cbClsExtra    = 0;
    windowClass.cbWndExtra    = 0;
    windowClass.hInstance     = GetModuleHandle(NULL);
    windowClass.hIcon         = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IDI_ICON));
	windowClass.hIconSm       = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IDI_ICON));
    windowClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // Window start up color
    windowClass.lpszMenuName  = NULL;
    windowClass.lpszClassName = WINDOW_CLASSNAME;

    if(!::RegisterClassEx(&windowClass))
		assert("Error in window class");

	// Set message
	m_message.message = (unsigned int)(~WM_QUIT);

	// Destroy window if it exsists
	if(m_window)
		DestroyWindow(m_window);

	// Setup window size
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int width = m_config.width, height = m_config.height;
	m_size.set(width, height);

	// Centered window rect
	RECT windowRect;
	windowRect.top		= (screenHeight/2)-(height/2);
	windowRect.bottom	= (screenHeight/2)+(height/2);
	windowRect.left		= (screenWidth/2)-(width/2);
	windowRect.right	= (screenWidth/2)+(width/2);

	// Windows style
	DWORD exStyle = 0;
	DWORD style = 0;		

	// Check fullscreen
	if(m_config.fullscreen)
	{
		// Apply fullscreen
		DEVMODE screenSettings;
		memset(&screenSettings, 0, sizeof(screenSettings));	
		screenSettings.dmSize		= sizeof(screenSettings);		
		screenSettings.dmPelsWidth	= width;			
		screenSettings.dmPelsHeight	= height;		
		screenSettings.dmBitsPerPel	= 32;		
		screenSettings.dmFields	    = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		// Change the display settings to fullscreen. On error, throw 
		// an exception.
		if(ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			assert("Unable to swith to fullscreen mode");

		// Set fullscreen style
		exStyle = WS_EX_APPWINDOW;	
		style = WS_POPUP;		

		// Hide the cursor.
		ShowCursor(false);
		m_fullscreen = true;
	}
	else
	{
		// Set windowed style
		exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		style = WS_OVERLAPPEDWINDOW;	
	}

	// Adjust the window to the true requested size
	AdjustWindowRectEx(&windowRect, style, FALSE, exStyle);

	// Create the main window
    m_window = CreateWindowEx(exStyle,
							  TEXT(WINDOW_CLASSNAME), 
							  TEXT(WINDOW_TITLE),
							  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style,
							  windowRect.left, windowRect.top,
							  windowRect.right - windowRect.left, 
							  windowRect.bottom - windowRect.top, 
							  NULL, NULL, 
							  GetModuleHandle(NULL), 
							  this);

	// Sanity check
	if(m_window == NULL)
	{
		// Failed to create window
		// Note: You might have gotten here because
		//       the application encountered an
		//       exception in ProcessEvent()
		DWORD error = GetLastError();
		char errorBuffer[1024];
		wsprintf(errorBuffer, "Error creating window. Error code, decimal %d, hexadecimal %X.", error, error);

		// Display the message
		MessageBox(NULL, errorBuffer, "Error", MB_ICONHAND);
	}
	assert(m_window);

	// Setup and show window
	m_graphics->createContext(m_window);
	ShowWindow(m_window, SW_SHOW);
	SetForegroundWindow(m_window);
	SetFocus(m_window);

	// Create cache list of resolutions
	DEVMODE dm = { 0 };
	dm.dmSize = sizeof(dm);
	for(int i = 0; EnumDisplaySettings(NULL, i, &dm) != 0; i++) {
		Vector2i res(dm.dmPelsWidth, dm.dmPelsHeight);
		if(find(m_resolutions.begin(), m_resolutions.end(), res) == m_resolutions.end())
			m_resolutions.push_back(res);
	}	
}

void Window::initEvents(xdScriptEngine *scriptEngine)
{
	// Set optional event functions
	m_focusLostFunc			=	scriptEngine->getModule()->GetFunctionByDecl("void focusLost()");
	m_focusGainedFunc		=	scriptEngine->getModule()->GetFunctionByDecl("void focusGained()");
	m_windowResizedFunc		=	scriptEngine->getModule()->GetFunctionByDecl("void windowResized(int, int)");
	m_leftMouseDownFunc		=	scriptEngine->getModule()->GetFunctionByDecl("void leftMouseDown()");
	m_leftMouseUpFunc		=	scriptEngine->getModule()->GetFunctionByDecl("void leftMouseUp()");
	m_rightMouseDownFunc	=	scriptEngine->getModule()->GetFunctionByDecl("void rightMouseDown()");
	m_rightMouseUpFunc		=	scriptEngine->getModule()->GetFunctionByDecl("void rightMouseUp()");
	m_middleMouseDownFunc	=	scriptEngine->getModule()->GetFunctionByDecl("void middleMouseDown()");
	m_middleMouseUpFunc		=	scriptEngine->getModule()->GetFunctionByDecl("void middleMouseUp()");
	m_mouseMoveFunc			=	scriptEngine->getModule()->GetFunctionByDecl("void mouseMove(int, int)");
	m_mouseWheelFunc		=	scriptEngine->getModule()->GetFunctionByDecl("void mouseWheel(int)");
	m_inputCharFunc			=	scriptEngine->getModule()->GetFunctionByDecl("void inputText(string key)");
	m_shutdownFunc			= 	scriptEngine->getModule()->GetFunctionByDecl("void shutdown()");

	m_initEventsDone = true;
}

void Window::readConfig()
{
#ifdef DEPRICATE
	// Check if file exists
	string file = m_engine->getWorkingPath("config.ini");
	if(!isFile(file))
		return;

	// Read config data
	IniFile cfg(file);
	string widthStr = cfg.value("window/width");
	if(!widthStr.empty()) m_config.width = stoi(widthStr);
	string heightStr = cfg.value("window/height");
	if(!heightStr.empty()) m_config.height = stoi(heightStr);
	m_config.fullscreen = cfg.value("window/fullscreen") == "1" ? true : false;
#endif
}

//--------------------------------------------------------------------
// Window functions
//--------------------------------------------------------------------
void Window::enableFullscreen()
{
	if(!m_fullscreen)
	{
		// Check if resolution is supported
		if(find(m_resolutions.begin(), m_resolutions.end(), m_size) == m_resolutions.end())
		{
			// Apply largest resolution
			Vector2 res = m_resolutions.back();
			LOG("Resolution %ix%i is not supported by this graphics card. Using largest resolution %ix%i by default.", uint(m_size.x), uint(m_size.y), uint(res.x), uint(res.y));
			m_size = res;
		}

		// Apply fullscreen
		DEVMODE screenSettings;
		memset(&screenSettings, 0, sizeof(screenSettings));
		screenSettings.dmSize		= sizeof(screenSettings);
		screenSettings.dmPelsWidth	= (uint)m_size.x;
		screenSettings.dmPelsHeight	= (uint)m_size.y;
		screenSettings.dmBitsPerPel	= 32;
		screenSettings.dmFields	    = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		// Change the display settings to fullscreen. On error, throw an exception.
		if(ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			assert("Unable to swith to fullscreen mode");

		DWORD exStyle = WS_EX_APPWINDOW;
		DWORD style = WS_POPUP;

		// Set styles
		SetWindowLong(m_window, GWL_STYLE, style);
		SetWindowLong(m_window, GWL_EXSTYLE, exStyle);

		// Hide the cursor
		//ShowCursor(false);

		// Set window pos and size
		SetWindowPos(m_window, HWND_TOP, 0, 0, (uint)m_size.x, (uint)m_size.y, SWP_SHOWWINDOW);

		// Set fullscreen
		m_fullscreen = true;
	}
}

void Window::disableFullscreen()
{
	if(m_fullscreen)
	{
		// Remove the full screen setting
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);

		// Setup window size
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		DWORD style = WS_OVERLAPPEDWINDOW;

		// Centered window rect
		RECT windowRect;
		windowRect.top		= (long)(screenHeight/2)-(long)(m_size.y/2);
		windowRect.bottom	= (long)(screenHeight/2)+(long)(m_size.y/2);
		windowRect.left		= (long)(screenWidth/2)-(long)(m_size.x/2);
		windowRect.right	= (long)(screenWidth/2)+(long)(m_size.x/2);
	
		// Adjust the window to the true requested size
		AdjustWindowRectEx(&windowRect, style, false, exStyle);

		// Set styles
		SetWindowLong(m_window, GWL_STYLE, style);
		SetWindowLong(m_window, GWL_EXSTYLE, exStyle);

		// Set window pos and size
		SetWindowPos(m_window, HWND_TOP, windowRect.left, windowRect.top,
					windowRect.right - windowRect.left,  windowRect.bottom - windowRect.top,
					SWP_SHOWWINDOW);

		// Set fullscreen
		m_fullscreen = false;
	}
}

bool Window::isFullscreen() const
{
	return m_fullscreen;
}

Array *Window::getResolutionList() const
{
    // Create the array object
    Array *arr = CreateArray("Vector2i", 0);

	// Copy from cached list
	arr->Resize(m_resolutions.size());
	for(uint i = 0; i < m_resolutions.size(); i++)
		((Vector2i*)arr->At(i))->set(m_resolutions[i].x, m_resolutions[i].y);
	
	// Return list
	return arr;
}

void Window::enableResize()
{
	// Set style
	SetWindowLong(m_window, GWL_STYLE, WS_OVERLAPPEDWINDOW);
	ShowWindow(m_window, SW_SHOW);
}

void Window::disableResize()
{
	// Set style
	SetWindowLong(m_window, GWL_STYLE,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
		WS_MINIMIZEBOX);
	ShowWindow(m_window, SW_SHOW);
}

bool Window::hasFocus()
{
	return m_focus;
}

Vector2i Window::getPosition() const
{
	RECT rect;
	GetWindowRect(m_window, &rect);
	return Vector2i(rect.left, rect.top);
	/*if(!borders) {
		x += GetSystemMetrics(SM_CXSIZEFRAME)*2;
		y += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME)*2;
	}*/
}

void Window::setSize(const Vector2i &size)
{
	// Make sure the application is initialized
	/*if(!m_initEventsDone)
	{
		ERR("Window::resize(): Application has to be initialized first!");
		return;
	}*/

	// Set size
	m_size = size;

	// If we are fullscreen, refresh fullscreen mode
	if(m_fullscreen)
	{
		// Check if resolution is supported
		if(find(m_resolutions.begin(), m_resolutions.end(), m_size) == m_resolutions.end())
		{
			// Apply largest resolution
			Vector2i res = m_resolutions.back();
			LOG("Resolution '%ix%i' is not supported as a fullscreen resolution by the graphics card. Using resolution %ix%i instead.",
				m_size.x, m_size.y, res.x, res.y);
			m_size = res;
		}

		// Apply fullscreen
		DEVMODE screenSettings;
		memset(&screenSettings, 0, sizeof(screenSettings));
		screenSettings.dmSize		= sizeof(screenSettings);
		screenSettings.dmPelsWidth	= (uint)m_size.x;
		screenSettings.dmPelsHeight	= (uint)m_size.y;
		screenSettings.dmBitsPerPel	= 32;
		screenSettings.dmFields	    = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		// Change the display settings to fullscreen. On error, throw an exception.
		if(ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			assert("Unable to swith to fullscreen mode");

		DWORD exStyle = WS_EX_APPWINDOW;
		DWORD style = WS_POPUP;

		// Set styles
		SetWindowLong(m_window, GWL_STYLE, style);
		SetWindowLong(m_window, GWL_EXSTYLE, exStyle);

		// Set window pos and size
		SetWindowPos(m_window, HWND_TOP, 0, 0, (uint)m_size.x, (uint)m_size.y, SWP_SHOWWINDOW);
	}else{

		// Setup window size
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Centered window rect
		RECT windowRect;
		windowRect.top		= (long)(screenHeight/2)-(long)(m_size.y/2);
		windowRect.bottom	= (long)(screenHeight/2)+(long)(m_size.y/2);
		windowRect.left		= (long)(screenWidth/2)-(long)(m_size.x/2);
		windowRect.right	= (long)(screenWidth/2)+(long)(m_size.x/2);
	
		// Adjust the window to the true requested size
		AdjustWindowRectEx(&windowRect, GetWindowLong(m_window, GWL_STYLE), false, GetWindowLong(m_window, GWL_EXSTYLE));

		// Resize window
		SetWindowPos(m_window, HWND_TOP, windowRect.left, windowRect.top,
					windowRect.right - windowRect.left,
					windowRect.bottom - windowRect.top,
					SWP_SHOWWINDOW);
	}
}

void Window::maximize()
{
}

void Window::minimize()
{
}

void Window::setPosition(const Vector2i&)
{
}

void Window::restore()
{
}

Vector2i Window::getSize() const
{
	return m_size;
}

//--------------------------------------------------------------------
// Events
//--------------------------------------------------------------------
LRESULT Window::OnEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// On create
	if(Message == WM_NCCREATE)
	{
        // Get the creation parameters.
		CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);

        // Set as the "user data" parameter of the window
        SetWindowLongPtr(Handle, GWLP_USERDATA, reinterpret_cast<long>(pCreateStruct->lpCreateParams));
	}else{
		// Get the Window instance corresponding to the window handle
		Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(Handle, GWLP_USERDATA));

		// If we have a window, call processEvent()
		if(window) {
			window->processEvents(Message, wParam, lParam);
		}
	}

	// Return default
    return DefWindowProc(Handle, Message, wParam, lParam);
}


void Window::processEvents(UINT Message, WPARAM wParam, LPARAM lParam)
{
	// Make sure app is initialized
	if(!m_initEventsDone)
		return;

	// Switch message
	switch(Message)
    {
        case WM_CLOSE:
		{
			if(m_shutdownFunc)
			{
				FunctionCall *func = CreateFuncCall();
				func->Prepare(m_shutdownFunc);
				func->Execute();
				delete func;
			}

			// Quit when we close the main window
            PostQuitMessage(0);
			close();
		}
		break;

		case WM_SIZE:
		{
			// Resize viewport
			m_size.set(LOWORD(lParam), HIWORD(lParam));
			if(m_windowResizedFunc)
			{
				FunctionCall *func = CreateFuncCall();
				func->Prepare(m_windowResizedFunc);
				func->SetArgument(0, &m_size.x, 4);
				func->SetArgument(1, &m_size.y, 4);
				func->Execute();
				delete func;
			}

			m_graphics->setOrthoProjection(0.0f, m_size.x, m_size.y, 0.0f, -1.0f, 1.0f);
			m_graphics->setViewport(Recti(0, 0, m_size.x, m_size.y));
		}
		break;

		case WM_SETFOCUS:
		{
			m_focus = true;
			if(m_focusGainedFunc) {
				//startScriptFuncCall(m_focusGainedFunc);
				//endScriptFuncCall();
			}
		}
		break;
			
		case WM_KILLFOCUS:
		{
			m_focus = false;
			if(m_focusLostFunc) {
				//startScriptFuncCall(m_focusLostFunc);
				//endScriptFuncCall();
			}
		}
		break;

		case WM_LBUTTONDOWN:
		{
			if(m_leftMouseDownFunc) {
				//startScriptFuncCall(m_leftMouseDownFunc);
				//endScriptFuncCall();
			}
		}
		break;

		case WM_LBUTTONUP:
		{
			if(m_leftMouseUpFunc) {
				//startScriptFuncCall(m_leftMouseUpFunc);
				//endScriptFuncCall();
			}
		}
		break;

		case WM_RBUTTONDOWN:
		{
			if(m_rightMouseDownFunc) {
				//startScriptFuncCall(m_rightMouseDownFunc);
				//endScriptFuncCall();
			}
		}
		break;

		case WM_RBUTTONUP:
		{
			if(m_rightMouseUpFunc) {
				//startScriptFuncCall(m_rightMouseUpFunc);
				//endScriptFuncCall();
			}
		}
		break;
		
		case WM_MBUTTONDOWN:
		{
			if(m_middleMouseDownFunc) {
				//startScriptFuncCall(m_middleMouseDownFunc);
				//endScriptFuncCall();
			}
		}
		break;

		case WM_MBUTTONUP:
		{
			if(m_middleMouseUpFunc) {
				//startScriptFuncCall(m_middleMouseUpFunc);
				//endScriptFuncCall();
			}
		}
		break;

		case WM_MOUSEMOVE:
		{
			// Set cursor pos
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);
			if(m_mouseMoveFunc) {
				//startScriptFuncCall(m_mouseMoveFunc);
				//addScriptFuncArg(&x, 4);
				//addScriptFuncArg(&y, 4);
				//endScriptFuncCall();
			}
			m_input->m_position.set(x, y);
		}
		break;

		case WM_MOUSEWHEEL:
		{
			if(m_mouseWheelFunc) {
				int scrollDelta = (short)HIWORD(wParam);
				//startScriptFuncCall(m_mouseWheelFunc);
				//addScriptFuncArg(&scrollDelta, 4);
				//endScriptFuncCall();
			}
		}
		break;

		case WM_CHAR:
		{
			if(m_inputCharFunc) {
				string key;
				key += (char)wParam;
				
				//startScriptFuncCall(m_inputCharFunc);
				//addScriptFuncArg(&key, stringTypeId);
				//endScriptFuncCall();
			}
		}
		break;
    }
}


// Depricated, altought good reference code
/*void Window::updateViewports()
{
	// Update viewports
	Viewport *viewport = m_engine.viewport;
	if(m_size.x <= 0 || m_size.y <= 0)
		return;
	if(viewport->m_resizeWithCanvas)
	{
		// Resize viewport to window
		viewport->setSize(m_size);
		if(!viewport->m_keepImage)
		{
			// Resize image to viewport
			Vector2 prevSize = viewport->gameSize();
			viewport->setGameSize(m_size);

			// Call resize event
			CanvasEvent e;
			e.canvas = m_size;
			e.prevCanvas = prevSize;
			m_engine.level->postEvent(&e);
		}
	}else if(viewport->m_resizeWithAspectRatio)
	{
		// Resize viewport to aspect ratio
		if(m_size.x/m_size.y < viewport->m_aspectRatio)
		{
			viewport->setSize((uint)m_size.x, (uint)(m_size.x/viewport->m_aspectRatio));
		}else{
			viewport->setSize((uint)(m_size.y*viewport->m_aspectRatio), (uint)m_size.y);
		}
		viewport->setPos((uint)((m_size.x/2)-(viewport->size().x/2)), (uint)((m_size.y/2)-(viewport->size().y/2)));
	}
}*/