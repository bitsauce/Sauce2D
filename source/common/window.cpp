//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/graphics.h>

BEGIN_XD_NAMESPACE

// Window class name
#define WINDOW_CLASSNAME "x2D"

// Window title
#define WINDOW_TITLE "x2D Game Engine"

//--------------------------------------------------------------------
// Window
//--------------------------------------------------------------------

HWND Window::s_window = 0;
HDC Window::s_deviceContext = 0;
MSG Window::s_message;
vector<Vector2i> Window::s_resolutions;
bool Window::s_focus = false;
Vector2i Window::s_size = Vector2i(0);
bool Window::s_fullscreen = false;
list<xd::WindowListener*> Window::s_windowListeners;
xd::GraphicsContext *Window::s_graphicsContext = nullptr;

void Window::close()
{
	// If we're fullscreen
	if(s_fullscreen)
	{
		// Remove the full screen setting
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);	
	}

	// If we have a OpenGL context
	xd::Graphics::destroyContext();

	// If we have a device context
	if(s_deviceContext)
	{
		// Release the device context
		ReleaseDC(s_window, s_deviceContext);
		s_deviceContext = NULL;
	}

	// Destroy our main window and unregister the window class
    DestroyWindow(s_window);
    UnregisterClass(TEXT(WINDOW_CLASSNAME), GetModuleHandle(NULL));

	// Close engine
	Engine::exit();
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
void Window::show()
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

    if(!RegisterClassEx(&windowClass))
		assert("Error in window class");

	// Set message
	s_message.message = (unsigned int)(~WM_QUIT);

	// Destroy window if it exsists
	if(s_window) {
		DestroyWindow(s_window);
	}

	// Setup window size
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int width = 800, height = 600;
	s_size.set(width, height);

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
	if(s_fullscreen)
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
		s_fullscreen = true;
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
    s_window = CreateWindowEx(exStyle,
							  TEXT(WINDOW_CLASSNAME), 
							  TEXT(WINDOW_TITLE),
							  WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style,
							  windowRect.left, windowRect.top,
							  windowRect.right - windowRect.left, 
							  windowRect.bottom - windowRect.top, 
							  NULL, NULL, 
							  GetModuleHandle(NULL), 
							  0);

	// Sanity check
	if(s_window == NULL)
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
	assert(s_window);

	// Describes the pixel format of the drawing surface
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;					// Version Number
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |	// Draws to a window
				PFD_SUPPORT_OPENGL |	// The format must support OpenGL
				PFD_DOUBLEBUFFER;		// Support for double buffering
	pfd.iPixelType = PFD_TYPE_RGBA;		// Uses an RGBA pixel format
	pfd.cColorBits = 24;				// 24 bits colors
	pfd.cAlphaBits = 8;					// 8 bits alpha

	// Get device context
	s_deviceContext = GetDC(s_window);
	if(!s_deviceContext)	
		assert("Unable to create rendering context");

	// Do Windows find a matching pixel format?
	int pixelFormat = ChoosePixelFormat(s_deviceContext, &pfd);
	if(!pixelFormat)				
		assert("Unable to create rendering context");

	// Set the new pixel format
	if(!SetPixelFormat(s_deviceContext, pixelFormat, &pfd))			
		assert("Unable to create rendering context");

	xd::Graphics::createContext();

	// Setup window
	ShowWindow(s_window, SW_SHOW);
	SetForegroundWindow(s_window);
	SetFocus(s_window);

	// Create cache list of resolutions
	DEVMODE dm = { 0 };
	dm.dmSize = sizeof(dm);
	for(int i = 0; EnumDisplaySettings(NULL, i, &dm) != 0; i++) {
		Vector2i res(dm.dmPelsWidth, dm.dmPelsHeight);
		if(find(s_resolutions.begin(), s_resolutions.end(), res) == s_resolutions.end()) {
			s_resolutions.push_back(res);
		}
	}	
}

//--------------------------------------------------------------------
// Window functions
//--------------------------------------------------------------------
void Window::enableFullscreen()
{
	if(!s_fullscreen)
	{
		// Check if resolution is supported
		if(find(s_resolutions.begin(), s_resolutions.end(), s_size) == s_resolutions.end())
		{
			// Apply largest resolution
			Vector2 res = s_resolutions.back();
			LOG("Resolution %ix%i is not supported by this graphics card. Using largest resolution %ix%i by default.", uint(s_size.x), uint(s_size.y), uint(res.x), uint(res.y));
			s_size = res;
		}

		// Apply fullscreen
		DEVMODE screenSettings;
		memset(&screenSettings, 0, sizeof(screenSettings));
		screenSettings.dmSize		= sizeof(screenSettings);
		screenSettings.dmPelsWidth	= (uint)s_size.x;
		screenSettings.dmPelsHeight	= (uint)s_size.y;
		screenSettings.dmBitsPerPel	= 32;
		screenSettings.dmFields	    = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		// Change the display settings to fullscreen. On error, throw an exception.
		if(ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			assert("Unable to swith to fullscreen mode");

		DWORD exStyle = WS_EX_APPWINDOW;
		DWORD style = WS_POPUP;

		// Set styles
		SetWindowLong(s_window, GWL_STYLE, style);
		SetWindowLong(s_window, GWL_EXSTYLE, exStyle);

		// Hide the cursor
		//ShowCursor(false);

		// Set window pos and size
		SetWindowPos(s_window, HWND_TOP, 0, 0, (uint)s_size.x, (uint)s_size.y, SWP_SHOWWINDOW);

		// Set fullscreen
		s_fullscreen = true;
	}
}

void Window::disableFullscreen()
{
	if(s_fullscreen)
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
		windowRect.top		= (long)(screenHeight/2)-(long)(s_size.y/2);
		windowRect.bottom	= (long)(screenHeight/2)+(long)(s_size.y/2);
		windowRect.left		= (long)(screenWidth/2)-(long)(s_size.x/2);
		windowRect.right	= (long)(screenWidth/2)+(long)(s_size.x/2);
	
		// Adjust the window to the true requested size
		AdjustWindowRectEx(&windowRect, style, false, exStyle);

		// Set styles
		SetWindowLong(s_window, GWL_STYLE, style);
		SetWindowLong(s_window, GWL_EXSTYLE, exStyle);

		// Set window pos and size
		SetWindowPos(s_window, HWND_TOP, windowRect.left, windowRect.top,
					windowRect.right - windowRect.left,  windowRect.bottom - windowRect.top,
					SWP_SHOWWINDOW);

		// Set fullscreen
		s_fullscreen = false;
	}
}

bool Window::isFullscreen()
{
	return s_fullscreen;
} 
/*XScriptArray *Window::getResolutionList() const
{
    // Create the array object
    XScriptArray *arr = CreateArray("Vector2i", 0);

	// Copy from cached list
	arr->Resize(m_resolutions.size());
	for(uint i = 0; i < m_resolutions.size(); i++)
		((Vector2i*)arr->At(i))->set(m_resolutions[i].x, m_resolutions[i].y);
	
	// Return list
	return arr;
}*/

void Window::enableResize()
{
	// Set style
	SetWindowLong(s_window, GWL_STYLE, WS_OVERLAPPEDWINDOW);
	ShowWindow(s_window, SW_SHOW);
}

void Window::disableResize()
{
	// Set style
	SetWindowLong(s_window, GWL_STYLE,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
		WS_MINIMIZEBOX);
	ShowWindow(s_window, SW_SHOW);
}

bool Window::hasFocus()
{
	return s_focus;
}

Vector2i Window::getPosition()
{
	RECT rect;
	GetWindowRect(s_window, &rect);
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
	s_size = size;

	// If we are fullscreen, refresh fullscreen mode
	if(s_fullscreen)
	{
		// Check if resolution is supported
		if(find(s_resolutions.begin(), s_resolutions.end(), s_size) == s_resolutions.end())
		{
			// Apply largest resolution
			Vector2i res = s_resolutions.back();
			LOG("Resolution '%ix%i' is not supported as a fullscreen resolution by the graphics card. Using resolution %ix%i instead.",
				s_size.x, s_size.y, res.x, res.y);
			s_size = res;
		}

		// Apply fullscreen
		DEVMODE screenSettings;
		memset(&screenSettings, 0, sizeof(screenSettings));
		screenSettings.dmSize		= sizeof(screenSettings);
		screenSettings.dmPelsWidth	= (uint)s_size.x;
		screenSettings.dmPelsHeight	= (uint)s_size.y;
		screenSettings.dmBitsPerPel	= 32;
		screenSettings.dmFields	    = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		// Change the display settings to fullscreen. On error, throw an exception.
		if(ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			assert("Unable to swith to fullscreen mode");

		DWORD exStyle = WS_EX_APPWINDOW;
		DWORD style = WS_POPUP;

		// Set styles
		SetWindowLong(s_window, GWL_STYLE, style);
		SetWindowLong(s_window, GWL_EXSTYLE, exStyle);

		// Set window pos and size
		SetWindowPos(s_window, HWND_TOP, 0, 0, (uint)s_size.x, (uint)s_size.y, SWP_SHOWWINDOW);
	}
	else
	{
		// Setup window size
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Centered window rect
		RECT windowRect;
		windowRect.top		= (long)(screenHeight/2)-(long)(s_size.y/2);
		windowRect.bottom	= (long)(screenHeight/2)+(long)(s_size.y/2);
		windowRect.left		= (long)(screenWidth/2)-(long)(s_size.x/2);
		windowRect.right	= (long)(screenWidth/2)+(long)(s_size.x/2);
	
		// Adjust the window to the true requested size
		AdjustWindowRectEx(&windowRect, GetWindowLong(s_window, GWL_STYLE), false, GetWindowLong(s_window, GWL_EXSTYLE));

		// Resize window
		SetWindowPos(s_window, HWND_TOP, windowRect.left, windowRect.top,
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

void Window::setPosition(const Vector2i &)
{
}

void Window::restore()
{
}

Vector2i Window::getSize()
{
	return s_size;
}

void Window::addWindowListener(xd::WindowListener *listener)
{
	s_windowListeners.push_back(listener);
}

void Window::removeWindowListener(xd::WindowListener *listener)
{
	s_windowListeners.remove(listener);
}

//--------------------------------------------------------------------
// Event loop
//--------------------------------------------------------------------
void Window::processEvents()
{
	// Check for messages
	//
	// If the function returned with no timeout, it means that at 
	// least one message has been received, so process all of them.
	while(PeekMessage(&s_message, NULL, 0, 0, PM_REMOVE))
	{
		// If a message was waiting in the message queue, process it
		TranslateMessage(&s_message);
		DispatchMessage(&s_message);
	}
}

LRESULT Window::OnEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// On create
	if(Message == WM_NCCREATE)
	{
        // Get the creation parameters.
		CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);

        // Set as the "user data" parameter of the window
        SetWindowLongPtr(Handle, GWLP_USERDATA, reinterpret_cast<long>(pCreateStruct->lpCreateParams));
	}
	else
	{
		processEvents(Message, wParam, lParam);
	}

	// Return default
    return DefWindowProc(Handle, Message, wParam, lParam);
}

void Window::processEvents(UINT Message, WPARAM wParam, LPARAM lParam)
{
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
			s_size.set(LOWORD(lParam), HIWORD(lParam));
			s_graphicsContext->resizeViewport(s_size.x, s_size.y);
			for(list<xd::WindowListener*>::iterator itr = s_windowListeners.begin(); itr != s_windowListeners.end(); ++itr)
			{
				(*itr)->resizeEvent(s_size.x, s_size.y);
			}
		}
		break;

		case WM_MOVE:
		{
			/*for(list<xd::WindowListener*>::iterator itr = s_windowListeners.begin(); itr != s_windowListeners.end(); ++itr)
			{
				(*itr)->moveEvent(s_size.x, s_size.y);
			}*/
		}
		break;

		case WM_SETFOCUS:
		{
			s_focus = true;
		}
		break;
			
		case WM_KILLFOCUS:
		{
			s_focus = false;
		}
		break;

		case WM_LBUTTONDOWN:
		{
		}
		break;

		case WM_LBUTTONUP:
		{
		}
		break;

		case WM_RBUTTONDOWN:
		{
		}
		break;

		case WM_RBUTTONUP:
		{
		}
		break;
		
		case WM_MBUTTONDOWN:
		{
		}
		break;

		case WM_MBUTTONUP:
		{
		}
		break;

		case WM_MOUSEMOVE:
		{
			// Set cursor pos
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);
			Input::s_position.set((float)x, (float)y);
		}
		break;

		case WM_MOUSEWHEEL:
		{
			int scrollDelta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA; // should be float so that finer mousewheels dont bug out
			Input::mouseScroll(scrollDelta);
		}
		break;

		case WM_KEYDOWN:
			Input::keyPressed(fromWinKey((uchar)wParam));
		break;

		case WM_KEYUP:
			Input::keyReleased(fromWinKey((uchar)wParam));
		break;

		case WM_CHAR:
			Input::charEvent((wchar_t)wParam);
		break;
    }
}

// Depricated, although good reference code
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

END_XD_NAMESPACE