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

GLFWwindow *Window::s_window = 0;
//vector<Vector2i> Window::s_resolutions;
bool Window::s_focus = true;
bool Window::s_fullScreen = false;
list<WindowListener*> Window::s_windowListeners;
GraphicsContext *Window::s_graphicsContext = nullptr;

//--------------------------------------------------------------------
// Window
//--------------------------------------------------------------------

WindowListener::WindowListener()
{
	Window::addWindowListener(this);
}

WindowListener::~WindowListener()
{
	Window::removeWindowListener(this);
}

//--------------------------------------------------------------------
// Initialation
//--------------------------------------------------------------------

#define IDI_ICON 101

void Window::init(int w, int h, bool fs)
{
	if(!s_window)
	{
		// Create window
		s_window = glfwCreateWindow(w, h, WINDOW_TITLE, fs ? glfwGetPrimaryMonitor() : NULL, NULL);
	}
	else
	{
		// Change fullscreen mode
		GLFWwindow *window = glfwCreateWindow(w, h, WINDOW_TITLE, fs ? glfwGetPrimaryMonitor() : NULL, s_window);
		glfwDestroyWindow(s_window);
		s_window = window;
		s_graphicsContext->resizeViewport(w, h);
	}

	if(!s_window)
	{
		glfwTerminate();
		assert("Window could not initialize");
	}

	// Set callbacks
	glfwSetFramebufferSizeCallback(s_window, sizeChanged);
	glfwSetWindowFocusCallback(s_window, focusChanged);
	glfwSetKeyCallback(s_window, keyCallback);
	glfwSetCharCallback(s_window, charCallback);
	glfwSetMouseButtonCallback(s_window, mouseButtonCallback);
	glfwSetCursorPosCallback(s_window, cursorMoveCallback);
	glfwSetScrollCallback(s_window, scrollCallback);

	glfwMakeContextCurrent(s_window);
	Graphics::setVsync(1);
	glfwShowWindow(s_window);

	s_fullScreen = fs;
	s_focus = true;
}

void Window::close()
{
	glfwDestroyWindow(s_window);
}

// Exception
/*
void Window::exception(xdRetCode error, const char* message)
{
	// Show exception in message box
	MessageBox(m_window, message, "An Exception Occured", MB_OK);
}
*/

//--------------------------------------------------------------------
// Window functions
//--------------------------------------------------------------------
void Window::setFullScreen(const bool fullScreen)
{

	//int count;
	//const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

	if(s_fullScreen != fullScreen)
	{
		// Get window size
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		// Destroy window
		Window::init(mode->width, mode->height, fullScreen);
	}

	s_fullScreen = fullScreen;
}

bool Window::getFullScreen()
{
	return s_fullScreen;
} 

void Window::setResizable(const bool resizable)
{
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwShowWindow(s_window);
}

void Window::focusChanged(GLFWwindow*, int focus)
{
	s_focus = focus == 1;
}

bool Window::hasFocus()
{
	return s_focus;
}

Vector2i Window::getPosition()
{
	int x = -1, y = -1;
	glfwGetWindowPos(s_window, &x, &y);
	return Vector2i(x, y);
}

void Window::setSize(const int width, const int height)
{
	glfwSetWindowSize(s_window, width, height);
}

void Window::setWidth(const int width)
{
	setSize(width, getSize().y);
}

void Window::setHeight(const int height)
{
	setSize(getSize().x, height);
}

//void Window::maximize()
//{
//}

void Window::minimize()
{
	glfwIconifyWindow(s_window);
}

void Window::setPosition(const Vector2i &pos)
{
	glfwSetWindowPos(s_window, pos.x, pos.y);
}

void Window::restore()
{
	glfwRestoreWindow(s_window);
}

Vector2i Window::getSize()
{
	int w = -1, h = -1;
	glfwGetWindowSize(s_window, &w, &h);
	return Vector2i(w, h);
}

int Window::getWidth()
{
	int w = -1, h = -1;
	glfwGetWindowSize(s_window, &w, &h);
	return w;
}

int Window::getHeight()
{
	int w = -1, h = -1;
	glfwGetWindowSize(s_window, &w, &h);
	return h;
}

void Window::addWindowListener(WindowListener *listener)
{
	s_windowListeners.push_back(listener);
}

void Window::removeWindowListener(WindowListener *listener)
{
	s_windowListeners.remove(listener);
}

void Window::sizeChanged(GLFWwindow *, const int width, const int height)
{
	// Resize viewport
	if(s_graphicsContext)
	{
		s_graphicsContext->resizeViewport(width, height);
	}

	// Call rezie events
	for(list<WindowListener*>::iterator itr = s_windowListeners.begin(); itr != s_windowListeners.end(); ++itr)
	{
		(*itr)->resizeEvent(width, height);
	}
}

void Window::keyCallback(GLFWwindow*, int key, int scancode, int action, int mods)
{
	switch(action)
	{
		case GLFW_PRESS:
			KeyListener::callKeyPressed(key);
			break;
		case GLFW_RELEASE:
			KeyListener::callKeyReleased(key);
			break;
		case GLFW_REPEAT: break;
	}
}

void Window::charCallback(GLFWwindow *, uint c)
{
	KeyListener::callCharEvent(c);
}

void Window::mouseButtonCallback(GLFWwindow*, int button, int action, int mods)
{
	switch(action)
	{
		case GLFW_PRESS:
			KeyListener::callKeyPressed(button);
			break;
		case GLFW_RELEASE:
			KeyListener::callKeyReleased(button);
			break;
		case GLFW_REPEAT: break;
	}
}

void Window::cursorMoveCallback(GLFWwindow*, double x, double y)
{
	Input::s_position.set((float) x, (float) y);
}

void Window::scrollCallback(GLFWwindow*, double x, double y)
{
	MouseListener::callMouseWheelEvent(y);
}

END_XD_NAMESPACE