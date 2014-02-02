//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/              
//		MacroByte (C)

#include "x2d/platform.h"
#include "x2d/app.h"

#include <GLFW/glfw3.h>

// Time functions
#include <ctime>

// Window class name
#define WINDOW_CLASSNAME "x2D"

// Window title
#define WINDOW_TITLE "x2D Game Engine"

int integerTypeId = -1;
int stringTypeId = -1;

//--------------------------------------------------------------------
// Window
//--------------------------------------------------------------------

X2DApp::X2DApp() :
	m_gfx(gfx),
	m_size(0),
	m_initEventsDone(false),
	m_engine(engine),
	m_fullscreen(false)
{
	// Read configuration file
	readConfig();

	// Show the window
	showWindow();

#if defined(X2D_WINDOWS) || defined(X2D_LINUX) || defined(X2D_OSX)
	glfwCreateWindow(m_size.x, m_size.y, m_title, );
#endif
}

//--------------------------------------------------------------------
// Game loop
//--------------------------------------------------------------------

// Close window
void Win32App::close()
{
	// If we're fullscreen
	if(m_fullscreen)
	{
		// Remove the full screen setting
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);	
	}

	// If we have a OpenGL context
	//m_gfx->destroyContext();

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
	m_engine->close();
}

// Event callback
void Win32App::processEvents()
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
void Win32App::exception(X2DRetCode error, const char* message)
{
	// Show exception in message box
	MessageBox(m_window, message, "An Exception Occured", MB_OK);
}

//--------------------------------------------------------------------
// Initialation
//--------------------------------------------------------------------
#define IDI_ICON 101
void Win32App::showWindow()
{
	// Register the window class
    WNDCLASSEX windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style		  = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
    windowClass.lpfnWndProc   = &Win32App::OnEvent;
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
	m_gfx->createContext(m_window);
	ShowWindow(m_window, SW_SHOW);
	SetForegroundWindow(m_window);
	SetFocus(m_window);

	// Create cache list of resolutions
	DEVMODE dm = { 0 };
	dm.dmSize = sizeof(dm);
	for(int i = 0; EnumDisplaySettings(NULL, i, &dm) != 0; i++) {
		vec2 res((float)dm.dmPelsWidth, (float)dm.dmPelsHeight);
		if(find(m_resolutions.begin(), m_resolutions.end(), res) == m_resolutions.end())
			m_resolutions.push_back(res);
	}	
}

void Win32App::initEvents()
{
	// Set optional event functions
	m_focusLostFunc			= getScriptFuncHandle("void focusLost()");
	m_focusGainedFunc		= getScriptFuncHandle("void focusGained()");
	m_windowResizedFunc		= getScriptFuncHandle("void windowResized(int width, int height)");
	m_leftMouseDownFunc		= getScriptFuncHandle("void leftMouseDown()");
	m_leftMouseUpFunc		= getScriptFuncHandle("void leftMouseUp()");
	m_rightMouseDownFunc	= getScriptFuncHandle("void rightMouseDown()");
	m_rightMouseUpFunc		= getScriptFuncHandle("void rightMouseUp()");
	m_middleMouseDownFunc	= getScriptFuncHandle("void middleMouseDown()");
	m_middleMouseUpFunc		= getScriptFuncHandle("void middleMouseUp()");
	m_mouseMoveFunc			= getScriptFuncHandle("void mouseMove(int x, int y)");
	m_mouseWheelFunc		= getScriptFuncHandle("void mouseWheel(int scrollDelta)");
	m_inputCharFunc			= getScriptFuncHandle("void inputText(string key)");

	integerTypeId = getObjectTypeId("int"); 
	stringTypeId = getObjectTypeId("string");

	m_initEventsDone = true;
}

void Win32App::readConfig()
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
void Win32App::enableFullscreen()
{
	if(!m_fullscreen)
	{
		// Check if resolution is supported
		if(find(m_resolutions.begin(), m_resolutions.end(), m_size) == m_resolutions.end())
		{
			// Apply largest resolution
			vec2 res = m_resolutions.back();
			iosystem::warn("Resolution %ix%i is not supported by this graphics card. Using largest resolution %ix%i by default.", uint(m_size.x), uint(m_size.y), uint(res.x), uint(res.y));
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
		ShowCursor(false);

		// Set window pos and size
		SetWindowPos(m_window, HWND_TOP, 0, 0, (uint)m_size.x, (uint)m_size.y, SWP_SHOWWINDOW);

		// Set fullscreen
		m_fullscreen = true;
	}
}

void Win32App::disableFullscreen()
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

Array *Win32App::resolutionList()
{
    // Create the array object
    Array *arr = CreateArray("vec2i", 0);

	// Copy from cached list
	arr->Resize(m_resolutions.size());
	for(uint i = 0; i < m_resolutions.size(); i++)
		((vec2i*)arr->At(i))->set(m_resolutions[i].x, m_resolutions[i].y);
	
	// Return list
	return arr;
}

void Win32App::enableResize()
{
	// Set style
	SetWindowLong(m_window, GWL_STYLE, WS_OVERLAPPEDWINDOW);
	ShowWindow(m_window, SW_SHOW);
}

void Win32App::disableResize()
{
	// Set style
	SetWindowLong(m_window, GWL_STYLE,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
		WS_MINIMIZEBOX);
	ShowWindow(m_window, SW_SHOW);
}

bool Win32App::hasFocus()
{
	return m_focus;
}

void Win32App::pos(int &x, int &y, bool borders)
{
	RECT rect;
	GetWindowRect(m_window, &rect);
	x = rect.left;
	y = rect.top;
	if(!borders) {
		x += GetSystemMetrics(SM_CXSIZEFRAME)*2;
		y += GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYSIZEFRAME)*2;
	}
}

void Win32App::resize(const int width, const int height)
{
	// Make sure the application is initialized
	if(!m_initEventsDone)
	{
		iosystem::error("Win32App::resize(): Application has to be initialized first!");
		return;
	}

	// Set size
	m_size.set(width, height);

	// If we are fullscreen, refresh fullscreen mode
	if(m_fullscreen)
	{
		// Check if resolution is supported
		if(find(m_resolutions.begin(), m_resolutions.end(), m_size) == m_resolutions.end())
		{
			// Apply largest resolution
			vec2i res = m_resolutions.back();
			iosystem::warn("Resolution '%ix%i' is not supported as a fullscreen resolution by the graphics card. Using resolution %ix%i instead.",
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

void Win32App::maximize()
{
}

void Win32App::minimize()
{
}

void Win32App::move(int,int)
{
}

void Win32App::restore()
{
}

void Win32App::size(int& w,int& h)
{
	w = m_size.x;
	h = m_size.y;
}

void Win32App::cursorPos(int &x, int &y) const
{
	POINT p;
	GetCursorPos(&p);
	x = p.x;
	y = p.y;
}

void Win32App::setCursorPos(const int x, const int y)
{
	SetCursorPos(x, y);
}

void Win32App::setCursorLimits(const int x, const int y, const int w, const int h)
{
	RECT rect;
	rect.top    = (long)(y);
	rect.bottom	= (long)(y+h);
	rect.left   = (long)(x);
	rect.right  = (long)(x+w);
	ClipCursor(&rect);
}

bool Win32App::getKeyState(const X2DVirtualKey key)
{
	int vk = 0;
	switch(key)
	{
	case X2D_LeftMouse:	vk = VK_LBUTTON; break;
	case X2D_RightMouse: vk = VK_RBUTTON; break;
	case X2D_MiddleMouse: vk = VK_MBUTTON; break;
	case X2D_Key0: vk = '0'; break;
	case X2D_Key1: vk = '1'; break;
	case X2D_Key2: vk = '2'; break;
	case X2D_Key3: vk = '3'; break;
	case X2D_Key4: vk = '4'; break;
	case X2D_Key5: vk = '5'; break;
	case X2D_Key6: vk = '6'; break;
	case X2D_Key7: vk = '7'; break;
	case X2D_Key8: vk = '8'; break;
	case X2D_Key9: vk = '9'; break;
	case X2D_KeyA: vk = 'A'; break;
	case X2D_KeyB: vk = 'B'; break;
	case X2D_KeyC: vk = 'C'; break;
	case X2D_KeyD: vk = 'D'; break;
	case X2D_KeyE: vk = 'E'; break;
	case X2D_KeyF: vk = 'F'; break;
	case X2D_KeyG: vk = 'G'; break;
	case X2D_KeyH: vk = 'H'; break;
	case X2D_KeyI: vk = 'I'; break;
	case X2D_KeyJ: vk = 'J'; break;
	case X2D_KeyK: vk = 'K'; break;
	case X2D_KeyL: vk = 'L'; break;
	case X2D_KeyM: vk = 'M'; break;
	case X2D_KeyN: vk = 'N'; break;
	case X2D_KeyO: vk = 'O'; break;
	case X2D_KeyP: vk = 'P'; break;
	case X2D_KeyQ: vk = 'Q'; break;
	case X2D_KeyR: vk = 'R'; break;
	case X2D_KeyS: vk = 'S'; break;
	case X2D_KeyT: vk = 'T'; break;
	case X2D_KeyU: vk = 'U'; break;
	case X2D_KeyV: vk = 'V'; break;
	case X2D_KeyW: vk = 'W'; break;
	case X2D_KeyX: vk = 'X'; break;
	case X2D_KeyY: vk = 'Y'; break;
	case X2D_KeyZ: vk = 'X'; break;
	case X2D_KeyRight: vk = VK_RIGHT; break;
	case X2D_KeyDown: vk = VK_DOWN; break;
	case X2D_KeyLeft: vk = VK_LEFT; break;
	case X2D_KeyUp: vk = VK_UP; break;
	case X2D_KeyBackspace: vk = VK_BACK; break;
	case X2D_KeyEscape: vk = VK_ESCAPE; break;
	case X2D_KeyTab: vk = VK_TAB; break;
	case X2D_KeyEnter: vk = VK_RETURN; break;
	case X2D_KeyShift: vk = VK_SHIFT; break;
	case X2D_KeyLeftShift: vk = VK_LSHIFT; break;
	case X2D_KeyRightShift: vk = VK_RSHIFT; break;
	case X2D_KeyControl: vk = VK_CONTROL; break;
	case X2D_KeyLeftControl: vk = VK_LCONTROL; break;
	case X2D_KeyRightControl: vk = VK_RCONTROL; break;
	case X2D_KeyMenu: vk = VK_MENU; break;
	case X2D_KeyLeftMenu: vk = VK_LMENU; break;
	case X2D_KeyRightMenu: vk = VK_RMENU; break;
	case X2D_KeySpace: vk = VK_SPACE; break;
	case X2D_KeyPageUp: vk = VK_NEXT; break;
	case X2D_KeyPageDown: vk = VK_PRIOR; break;
	case X2D_KeyEnd: vk = VK_END; break;
	case X2D_KeyHome: vk = VK_HOME; break;
	case X2D_KeySnapshot: vk = VK_SNAPSHOT; break;
	case X2D_KeyInsert: vk = VK_INSERT; break;
	case X2D_KeyDelete: vk = VK_DELETE; break;
	case X2D_KeyNumpad0: vk = VK_NUMPAD0; break;
	case X2D_KeyNumpad1: vk = VK_NUMPAD1; break;
	case X2D_KeyNumpad2: vk = VK_NUMPAD2; break;
	case X2D_KeyNumpad3: vk = VK_NUMPAD3; break;
	case X2D_KeyNumpad4: vk = VK_NUMPAD4; break;
	case X2D_KeyNumpad5: vk = VK_NUMPAD5; break;
	case X2D_KeyNumpad6: vk = VK_NUMPAD6; break;
	case X2D_KeyNumpad7: vk = VK_NUMPAD7; break;
	case X2D_KeyNumpad8: vk = VK_NUMPAD8; break;
	case X2D_KeyNumpad9: vk = VK_NUMPAD9; break;
	/*case X2D_KeyPluss: vk = ; break;
	case X2D_KeyHyphen: vk = ; break;
	case X2D_KeyCaret: vk = ; break;
	case X2D_KeyApostrophe: vk = ; break;
	case X2D_KeyQuotationMark: vk = ; break;
	case X2D_KeyBackslash: vk = ; break;
	case X2D_KeySlash: vk = ; break;
	case X2D_KeyAsterisk: vk = ; break;
	case X2D_KeyPipe: vk = ; break;
	case X2D_KeyColon: vk = ; break;
	case X2D_KeySemicolon: vk = ; break;
	case X2D_KeyTilde: vk = ; break;
	case X2D_KeyComma: vk = ; break;
	case X2D_KeyPeriod: vk = ; break;
	case X2D_KeyGreater: vk = ; break;
	case X2D_KeyLess: vk = ; break;
	case X2D_KeyEquals: vk = VK_; break;*/
	}
	return Win32App::hasFocus() && (GetKeyState(vk) & 0x80) != 0;
}

// Vsync
void Win32App::enableVsync()
{
	wglSwapIntervalEXT(1);
}

void Win32App::disableVsync()
{
	wglSwapIntervalEXT(0);
}

//--------------------------------------------------------------------
// Events
//--------------------------------------------------------------------
LRESULT Win32App::OnEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam)
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
		Win32App* win32App = reinterpret_cast<Win32App*>(GetWindowLongPtr(Handle, GWLP_USERDATA));

		// If we have a window, call processEvent()
		if(win32App) win32App->processEvents(Message, wParam, lParam);
	}

	// Return default
    return DefWindowProc(Handle, Message, wParam, lParam);
}

void Win32App::processEvents(UINT Message, WPARAM wParam, LPARAM lParam)
{
	// Make sure app is initialized
	if(!m_initEventsDone)
		return;

	// Switch message
	switch(Message)
    {
        case WM_CLOSE:
		{
			// Quit when we close the main window
            PostQuitMessage(0);
			close();
		}
		break;

		case WM_SIZE:
		{
			// Resize viewport
			m_size.set(LOWORD(lParam), HIWORD(lParam));
			if(m_windowResizedFunc) {
				startScriptFuncCall(m_windowResizedFunc);
				addScriptFuncArg(&m_size.x, 4);
				addScriptFuncArg(&m_size.y, 4);
				endScriptFuncCall();
			}
		}
		break;

		case WM_SETFOCUS:
		{
			m_focus = true;
			if(m_focusGainedFunc) {
				startScriptFuncCall(m_focusGainedFunc);
				endScriptFuncCall();
			}
		}
		break;
			
		case WM_KILLFOCUS:
		{
			m_focus = false;
			if(m_focusLostFunc) {
				startScriptFuncCall(m_focusLostFunc);
				endScriptFuncCall();
			}
		}
		break;

		case WM_LBUTTONDOWN:
		{
			if(m_leftMouseDownFunc) {
				startScriptFuncCall(m_leftMouseDownFunc);
				endScriptFuncCall();
			}
		}
		break;

		case WM_LBUTTONUP:
		{
			if(m_leftMouseUpFunc) {
				startScriptFuncCall(m_leftMouseUpFunc);
				endScriptFuncCall();
			}
		}
		break;

		case WM_RBUTTONDOWN:
		{
			if(m_rightMouseDownFunc) {
				startScriptFuncCall(m_rightMouseDownFunc);
				endScriptFuncCall();
			}
		}
		break;

		case WM_RBUTTONUP:
		{
			if(m_rightMouseUpFunc) {
				startScriptFuncCall(m_rightMouseUpFunc);
				endScriptFuncCall();
			}
		}
		break;
		
		case WM_MBUTTONDOWN:
		{
			if(m_middleMouseDownFunc) {
				startScriptFuncCall(m_middleMouseDownFunc);
				endScriptFuncCall();
			}
		}
		break;

		case WM_MBUTTONUP:
		{
			if(m_middleMouseUpFunc) {
				startScriptFuncCall(m_middleMouseUpFunc);
				endScriptFuncCall();
			}
		}
		break;

		case WM_MOUSEMOVE:
		{
			// Set cursor pos
			if(m_mouseMoveFunc) {
				int x = GET_X_LPARAM(lParam);
				int y = GET_Y_LPARAM(lParam);
				startScriptFuncCall(m_mouseMoveFunc);
				addScriptFuncArg(&x, 4);
				addScriptFuncArg(&y, 4);
				endScriptFuncCall();
			}
		}
		break;

		case WM_MOUSEWHEEL:
		{
			if(m_mouseWheelFunc) {
				int scrollDelta = (short)HIWORD(wParam);
				startScriptFuncCall(m_mouseWheelFunc);
				addScriptFuncArg(&scrollDelta, 4);
				endScriptFuncCall();
			}
		}
		break;

		case WM_CHAR:
		{
			if(m_inputCharFunc) {
				string key;
				key += (char)wParam;
				
				startScriptFuncCall(m_inputCharFunc);
				addScriptFuncArg(&key, stringTypeId);
				endScriptFuncCall();
			}
		}
		break;
    }
}

//--------------------------------------------------------------------
// System Time
//--------------------------------------------------------------------
ulong Win32App::getTickCount() { return GetTickCount(); } // Tick count
ulong Win32App::getProgramTime() { return clock(); } // Program running time

// High-resolution timer
__int64 freq = 1;
__int64 counterStart;
void Win32App::startHrTimer()
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
    QueryPerformanceCounter((LARGE_INTEGER*)&counterStart);
}

float Win32App::getHrTime()
{
    __int64 counter = 0;
    QueryPerformanceCounter((LARGE_INTEGER*)&counter);
    return float(counter-counterStart)/float(freq);
}

// Depricated, altought good reference code
/*void Win32App::updateViewports()
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
			vec2 prevSize = viewport->gameSize();
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