//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <x2d/engine.h>
#include <x2d/graphics.h>

BEGIN_CG_NAMESPACE

//--------------------------------------------------------------------
// Window
//--------------------------------------------------------------------

Window::Window(const string &title, const int x, const int y, const int w, const int h, const Uint32 flags)
{
	// Request opengl 3.1 context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create our window
	m_window = SDL_CreateWindow(title.c_str(), x, y, w, h, SDL_WINDOW_OPENGL | flags);
	if(!m_window)
	{
		THROW("Window could not initialize");
	}

	// Create graphics manager of this window
	m_graphicsContext = new GraphicsContext(this);

	// Set callbacks
	/*glfwSetFramebufferSizeCallback(s_window, sizeChanged);
	glfwSetWindowFocusCallback(s_window, focusChanged);
	glfwSetKeyCallback(s_window, keyCallback);
	glfwSetCharCallback(s_window, charCallback);
	glfwSetMouseButtonCallback(s_window, mouseButtonCallback);
	glfwSetCursorPosCallback(s_window, cursorMoveCallback);
	glfwSetScrollCallback(s_window, scrollCallback);

	glfwMakeContextCurrent(s_window);
	Graphics::setVsync(1);
	glfwShowWindow(s_window);*/
}

Window::~Window()
{
	delete m_graphicsContext;
	SDL_DestroyWindow(m_window);
}

//--------------------------------------------------------------------
// Window functions
//--------------------------------------------------------------------
void Window::setFullScreenMode(SDL_DisplayMode *displayMode)
{
	// Use desktop display mode if not given
	if(displayMode == 0)
	{
		SDL_GetDesktopDisplayMode(0, displayMode);
	}

	// Set display mode and fullscreen flag
	SDL_SetWindowDisplayMode(m_window, displayMode);
	SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
}

void Window::setWindowed()
{
	SDL_SetWindowFullscreen(m_window, 0);
}

Uint32 Window::getFlags() const
{
	return SDL_GetWindowFlags(m_window);
}

bool Window::checkFlags(const Uint32 flags) const
{
	return (getFlags() & flags) == flags;
}

void Window::setPosition(const int x, const int y)
{
	SDL_SetWindowPosition(m_window, x, y);
}

void Window::getPosition(int *x, int *y) const
{
	SDL_GetWindowPosition(m_window, x, y);
}

void Window::setSize(const int width, const int height)
{
	SDL_SetWindowSize(m_window, width, height);
}

void Window::getSize(int *width, int *height) const
{
	SDL_GetWindowSize(m_window, width, height);
}

void Window::setTitle(const string &title)
{
	SDL_SetWindowTitle(m_window, title.c_str());
}

string Window::getTitle() const
{
	return SDL_GetWindowTitle(m_window);
}

void Window::setIcon(const Pixmap &icon)
{
	Uint32 rmask, gmask, bmask, amask;

	/* SDL interprets each pixel as a 32-bit number, so our masks must depend
	on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void*)icon.getData(), icon.getWidth(), icon.getHeight(), 32, icon.getWidth() * 4, rmask, gmask, bmask, amask);
	SDL_SetWindowIcon(m_window, surface);
	SDL_FreeSurface(surface);
}

void Window::setMinimumSize(const int width, const int height)
{
	SDL_SetWindowMinimumSize(m_window, width, height);
}

void Window::setMaximumSize(const int width, const int height)
{
	SDL_SetWindowMaximumSize(m_window, width, height);
}

void Window::setBordered(const bool bordered)
{
	SDL_SetWindowBordered(m_window, bordered ? SDL_TRUE : SDL_FALSE);
}

void Window::setGrab(const bool grabbed)
{
	SDL_SetWindowGrab(m_window, grabbed ? SDL_TRUE : SDL_FALSE);
}

void Window::setBrightness(const float brightness)
{
	SDL_SetWindowBrightness(m_window, brightness);
}

void Window::setGammaRamp(Uint16 *red, Uint16 *green, Uint16 *blue)
{
	SDL_SetWindowGammaRamp(m_window, red, green, blue);
}

void Window::show()
{
	SDL_ShowWindow(m_window);
}

void Window::hide()
{
	SDL_HideWindow(m_window);
}

int Window::getDisplayIndex() const
{
	return SDL_GetWindowDisplayIndex(m_window);
}

void Window::getDisplayMode(SDL_DisplayMode *mode) const
{
	SDL_GetWindowDisplayMode(m_window, mode);
}

void Window::minimize()
{
	SDL_MinimizeWindow(m_window);
}

void Window::restore()
{
	SDL_RestoreWindow(m_window);
}

void Window::maximize()
{
	SDL_MaximizeWindow(m_window);
}

GraphicsContext *Window::getGraphicsContext() const
{
	return m_graphicsContext;
}

SDL_Window *Window::getSDLHandle() const
{
	return m_window;
}

void Window::setVSync(const int mode)
{
	SDL_GL_MakeCurrent(m_window, m_graphicsContext->getSDLHandle());
	SDL_GL_SetSwapInterval(mode);
}

END_CG_NAMESPACE