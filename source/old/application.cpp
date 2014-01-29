//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)

#ifdef WIN32

#include "platform/platform.h"
#include "aurorax2d.h"
#include "engine.h"

//------------------------------------------------------------------------
// X2DApp
//------------------------------------------------------------------------

X2DApp::X2DApp() :
	m_engine(NULL),
	m_onException(NULL),
	m_onOutput(NULL),
	m_focus(true),
	m_flags(0),
	m_state(X2D::StateUninitialized),
	m_fps(0),
	m_height(0),
	m_width(0)
{
}

X2DApp::~X2DApp()
{
	delete m_engine;
}

// Init app
int X2DApp::init()
{
	// Make sure we're not initializing twice
	assert(!m_engine);

	// Create application and init
	m_engine = new X2DEngine(this);
	int r = m_engine->init();

	// Check error
	if(r >= 0)
	{
		// Check if the application is still running
		if(m_state != X2D::StateClosed)
			m_state = X2D::StateInitialized; // Application initialized
	}else
		close();

	// Return
	return r;
}

// Draw app
void X2DApp::draw()
{
	gameEngine->GameGfx->draw();
}

// Update app
void X2DApp::update()
{
	gameEngine->GameLevel->update();
}

// Close app
void X2DApp::close()
{
	m_state = X2D::StateClosed;
}

//------------------------------------------------------------------------
//	Callback
//------------------------------------------------------------------------
// Exception callback
void X2DEngine::exception(X2D::ErrorCode error, const char* message)
{
	GameApp->raiseException(error, message);
}

void X2DApp::raiseException(X2D::ErrorCode error, const char* message)
{
	// Call function
	if(m_onException)
		m_onException(this, error, message);
}

void X2DApp::setExceptionCallback(void(*func)(X2DApp*, X2D::ErrorCode, const char*))
{
	// Make sure the function is undefined
	assert(!m_onException);
	
	// Set function
	m_onException = func;
}

// Output callback
void X2DEngine::output(const char* message)
{
	GameApp->writeOutput(message);
}

void X2DApp::writeOutput(const char* message)
{
	// Call function
	if(m_onOutput)
		m_onOutput(this, message);
}

void X2DApp::setOutputCallback(void(*func)(X2DApp*, const char*))
{
	// Make sure the function is undefined
	assert(!m_onOutput);
	
	// Set function
	m_onOutput = func;
}

//------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------
void X2DApp::leftButtonDown()
{
	MouseEvent e(X2D::MousePressEvent);
	e.scrollDelta = 0;
	e.button = LeftMouseButton;
	if(inputViewport)
	{
		e.gamePos = inputViewport->translateToGame(g_canvasCursor);
		e.viewPos = inputViewport->translateFromCanvas(g_canvasCursor);
	}else{
		e.gamePos.set(0.0f, 0.0f);
		e.viewPos.set(0.0f, 0.0f);
	}

	gameEngine->GameLevel->postEvent(&e);
}

void X2DApp::leftButtonUp()
{
	MouseEvent e(X2D::MouseReleaseEvent);
	e.scrollDelta = 0;
	e.button = LeftMouseButton;
	if(inputViewport)
	{
		e.gamePos = inputViewport->translateToGame(g_canvasCursor);
		e.viewPos = inputViewport->translateFromCanvas(g_canvasCursor);
	}else{
		e.gamePos.set(0.0f, 0.0f);
		e.viewPos.set(0.0f, 0.0f);
	}

	gameEngine->GameLevel->postEvent(&e);
}

void X2DApp::rightButtonDown()
{
	MouseEvent e(X2D::MousePressEvent);
	e.scrollDelta = 0;
	e.button = RightMouseButton;
	if(inputViewport)
	{
		e.gamePos = inputViewport->translateToGame(g_canvasCursor);
		e.viewPos = inputViewport->translateFromCanvas(g_canvasCursor);
	}else{
		e.gamePos.set(0.0f, 0.0f);
		e.viewPos.set(0.0f, 0.0f);
	}

	gameEngine->GameLevel->postEvent(&e);
}

void X2DApp::rightButtonUp()
{
	MouseEvent e(X2D::MouseReleaseEvent);
	e.scrollDelta = 0;
	e.button = RightMouseButton;
	if(inputViewport)
	{
		e.gamePos = inputViewport->translateToGame(g_canvasCursor);
		e.viewPos = inputViewport->translateFromCanvas(g_canvasCursor);
	}else{
		e.gamePos.set(0.0f, 0.0f);
		e.viewPos.set(0.0f, 0.0f);
	}

	gameEngine->GameLevel->postEvent(&e);
}

void X2DApp::mouseMove(int x, int y)
{
	// Make sure app is initialized
	if(m_state != X2D::StateInitialized)
		return;
	
	MouseEvent e(X2D::MouseMoveEvent);
	e.button = NoMouseButton;

	// Check for y-flip
	//if(!GameYAxisCenterTopLeft)
	y = m_height - y;
	
	// Translate to view relative positions
	//x *= (float)inputViewport->size().width/(float)inputViewport->windowSize().width;
	//y *= (float)inputViewport->size().height/(float)inputViewport->windowSize().height;

	// Store previous pos
	Point2 prevPos = g_cursor;
	Point2 uiprevPos = g_canvasCursor;
	setCursor(x, y);
	
	if(inputViewport)
	{
		e.gamePos = inputViewport->translateToGame(g_canvasCursor);
		e.viewPos = inputViewport->translateFromCanvas(g_canvasCursor);
	}else{
		e.gamePos.set(0.0f, 0.0f);
		e.viewPos.set(0.0f, 0.0f);
	}

	gameEngine->GameLevel->postEvent(&e);
}

void X2DApp::mouseScroll(const int delta)
{
	MouseEvent e(X2D::MouseReleaseEvent);
	e.scrollDelta = delta;
	e.button = NoMouseButton;
	if(inputViewport)
	{
		e.gamePos = inputViewport->translateToGame(g_canvasCursor);
		e.viewPos = inputViewport->translateFromCanvas(g_canvasCursor);
	}else{
		e.gamePos.set(0.0f, 0.0f);
		e.viewPos.set(0.0f, 0.0f);
	}

	gameEngine->GameLevel->postEvent(&e);
}

void X2DApp::keyPressed(const uint key, const uchar code)
{
	KeyEvent e(X2D::KeyPressEvent);
	e.key = key;
	e.code = code;
	
	gameEngine->GameLevel->postEvent(&e);
}

bool X2DApp::hasFocus()
{
	return m_focus;
}

void X2DApp::activateFocus()
{
	m_focus = true;
}

void X2DApp::deactivateFocus()
{
	m_focus = false;
}

void X2DApp::removeFlags(int flags)
{
	m_flags &= ~flags;
}

void X2DApp::addFlags(int flags)
{
	m_flags |= flags;
}

void X2DApp::setFlags(int flags)
{
	m_flags = flags;
}

int X2DApp::getFlags()
{
	return m_flags;
}

X2D::State X2DApp::state()
{
	return m_state;
}

void X2DApp::setFps(const int fps)
{
	m_fps = fps;
}

int X2DApp::fps() const
{
	return m_fps;
}

void X2DApp::resize(const unsigned int width, const unsigned int height)
{
	// Set size
	int prevWidth = m_width;
	int prevHeight = m_height;
	m_width = width;
	m_height = height;

	// Make sure app is initialized
	if(m_state != X2D::StateInitialized || !inputViewport)
		return;
	CanvasEvent e;
	e.prevCanvas.set(prevWidth, prevHeight);
	e.canvas.set(width, height);

	// Resize viewport to canvas size
	Viewport *viewport = inputViewport;
	if(viewport->aspectRatioMode() == KeepAspectRatio)
	{
		int x = (width - viewport->size().width) / 2;
		int y = (height - viewport->size().height) / 2;
		viewport->setPos(x, y);
	}else
		viewport->setSize(width, height);

	// Check for the aspect ratio mode which are supposed to not strech the image
	if(viewport->aspectRatioMode() == NoAspectRatio)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	// Post event
	X2DEngine::GameLevel->postEvent(&e);
}

void X2DApp::size(unsigned int &width, unsigned int &height)
{
	width = m_width;
	height = m_height;
}

unsigned int X2DApp::height()
{
	return m_height;
}

unsigned int X2DApp::width()
{
	return m_width;
}

void X2DApp::setWorkDirectory(const char* path)
{
	// Create a string
	string pathStr(path);

	// Make sure the path is of
	// native format
	toNativePath(pathStr);

	// Check if path is valid
	//assert(isValidPath(path));

	// Add tailing slash
	if(pathStr[pathStr.size()-1] != '/')
		pathStr += '/';
	
	// Set path
	g_workDir = pathStr;
}

const char* X2DApp::workDirectory()
{
	return g_workDir.c_str();
}

void X2DApp::cursor(int &x, int &y)
{
	x = m_cursorX;
	y = m_cursorY;
}

void X2DApp::setCursor(const int x, const int y)
{
	// Set cursor coordinates
	m_cursorX = g_canvasCursor.x = x;
	m_cursorY = g_canvasCursor.y = y;

	// Translate cursor position to in game
	if(inputViewport)
	{
		g_cursor.x = inputViewport->pos().x + m_cursorX;
		g_cursor.y = inputViewport->pos().y + m_cursorY;
	}
}

//------------------------------------------------------------------------
// X2DDebugger
//------------------------------------------------------------------------

X2DDebugger::X2DDebugger()
{
	// Create debugger
	X2DEngine::GameDebugger = new Debugger();
}

X2DDebugger::~X2DDebugger()
{
	X2DEngine::GameDebugger = NULL;
}

//------------------------------------------------------------------------
// Commands
//------------------------------------------------------------------------

void X2DDebugger::resume()
{
	// Resume debugging
	X2DEngine::GameDebugger->m_command = X2D::NoCommand;
}

void X2DDebugger::stepOver()
{
	// Step over next statement
	X2DEngine::GameDebugger->m_command = X2D::StepOverCommand;
}

void X2DDebugger::stepInto()
{
	// Step into next statement
	X2DEngine::GameDebugger->m_command = X2D::StepIntoCommand;
}

void X2DDebugger::stepOut()
{
	// Step out of current statemen
	X2DEngine::GameDebugger->m_command = X2D::StepOutCommand;
}

void X2DDebugger::interupt()
{
	// Interupt debugging
	X2DEngine::GameDebugger->m_command = X2D::InteruptCommand;
}

//------------------------------------------------------------------------
// Breakpoints
//------------------------------------------------------------------------
void X2DDebugger::addBreakpoint(const char* file, const int line)
{
	// Add breakpoint
	X2DEngine::GameDebugger->addBreakpoint(file, line);
}

void X2DDebugger::removeBreakpoint(const char* file, const int line)
{
	// Remove breakpoint
	X2DEngine::GameDebugger->removeBreakpoint(file, line);
}

void X2DDebugger::setDataAvailableCallback(void(*callback)(const char*, X2D::DebugDataType))
{
	// Set callback
	X2DEngine::GameDebugger->setDataAvailableCallback(callback);
}

void X2DDebugger::setBreakpointHitCallback(void(*callback)(const char*, const int))
{
	// Set callback
	X2DEngine::GameDebugger->setBreakpointHitCallback(callback);
}

#endif