//     _____                        ______             _            
//    / ____|                      |  ____|           (_)           
//   | (___   __ _ _   _  ___ ___  | |__   _ __   __ _ _ _ __   ___ 
//    \___ \ / _` | | | |/ __/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \
//    ____) | (_| | |_| | (_|  __/ | |____| | | | (_| | | | | |  __/
//   |_____/ \__,_|\__,_|\___\___| |______|_| |_|\__, |_|_| |_|\___|
//                                                __/ |             
//                                               |___/              
// Made by Marcus "Bitsauce" Loo Vergara
// 2011-2018 (C)

#include <Sauce/Common.h>
#include <Sauce/graphics.h>

BEGIN_SAUCE_NAMESPACE

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

Uint32 Window::getID() const
{
	return SDL_GetWindowID(m_window);
}

bool Window::handleEvent(SDL_Event &event, Game *game)
{
	switch(event.window.event)
	{
		case SDL_WINDOWEVENT_CLOSE:
		{
			return true;
		}
		break;

		case SDL_WINDOWEVENT_SIZE_CHANGED:
		{
			// Resize viewport
			int width = event.window.data1, height = event.window.data2;
			m_graphicsContext->resizeViewport(width, height);

			// Call event
			{
				WindowEvent e(WindowEvent::SIZE_CHANGED, this, width, height);
				game->onEvent(&e);
			}

			{
				Vector2I position = game->getInputManager()->getPosition();
				MouseEvent e(MouseEvent::MOVE, game->getInputManager(), position.x, position.y, SAUCE_MOUSE_BUTTON_NONE, 0, 0);
				game->onEvent(&e);
			}
		}
		break;
	}
	return false;
}

END_SAUCE_NAMESPACE
