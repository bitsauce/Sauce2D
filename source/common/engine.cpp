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
#include <x2d/audio.h>

#ifdef X2D_LINUX
#define MAX_PATH 256
#endif

XConfig::XConfig() :
	flags(0),
	workDir(""),
	draw(0),
	update(0),
	main(0)
{
}

//------------------------------------------------------------------------
// Engine
//------------------------------------------------------------------------

XEngine *CreateEngine()
{
	return new XEngine();
}

bool XEngine::s_initialized = false;
bool XEngine::s_paused = false;
bool XEngine::s_running = false;
int XEngine::s_flags = 0;
stack<XScene*> XEngine::s_sceneStack;

// System dirs
string XEngine::s_workDir;
string XEngine::s_saveDir;

XEngine::XEngine()
{
}

XEngine::~XEngine()
{
	// Pop all scene objects
	while(s_sceneStack.size() > 0)
	{
		popScene();
	}
	
	delete m_fileSystem;
	delete m_graphics;
	delete m_audio;
	delete m_timer;
	delete m_math;
	delete m_assetManager;
	delete m_console;
}

void XEngine::pushScene(XScene *scene)
{
	// Hide previous scene
	XScene *prevScene = s_sceneStack.size() > 0 ? s_sceneStack.top() : 0;
	if(prevScene)
	{
		prevScene->hideEvent();
	}

	// Add scene to stack
	s_sceneStack.push(scene);

	// Show new scene
	if(scene)
	{
		scene->showEvent();
	}
}

void XEngine::popScene()
{
	// Make sure there is a scene to pop
	if(s_sceneStack.size() == 0) return;

	// Hide and pop topmost scene
	XScene *scene = s_sceneStack.top();
	scene->hideEvent();
	s_sceneStack.pop();

	// Show next scene
	XScene *nextScene = s_sceneStack.size() > 0 ? s_sceneStack.top() : 0;
	if(nextScene)
	{
		nextScene->showEvent();
	}
}

#include <direct.h> // for _getcw()
#include <ShlObj.h>

// Convert a wide Unicode string to an UTF8 string
string utf8_encode(const wstring &wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    string strTo( size_needed, 0 );
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// Convert an UTF8 string to a wide Unicode String
wstring utf8_decode(const string &str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    wstring wstrTo( size_needed, 0 );
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

string getSaveDir()
{
	wchar_t *path;
	SHGetKnownFolderPath(FOLDERID_SavedGames, 0, 0, &path);
	string str = utf8_encode(path);
	CoTaskMemFree(path);
	return str;
}

//------------------------------------------------------------------------
// Run
//------------------------------------------------------------------------
int XEngine::init(const XConfig &config)
{
	// Make sure the config is valid
	if(!config.isValid())
	{
		return X2D_INVALID_CONFIG;
	}

	// Set platform string and program dir
	s_flags = config.flags;

	s_workDir = config.workDir;
	replace(s_workDir.begin(), s_workDir.end(), '\\', '/');
	util::toDirectoryPath(s_workDir);

	s_saveDir = getSaveDir();
	replace(s_saveDir.begin(), s_saveDir.end(), '\\', '/');
	util::toDirectoryPath(s_saveDir);
	
	m_console = new XConsole();
	m_fileSystem = new XFileSystem();
	if(isEnabled(XD_EXPORT_LOG))
	{
		m_console->m_output = new XFileWriter(util::getAbsoluteFilePath(":/console.log"));
	}

	m_timer = new XTimer();
	m_graphics = new XGraphics();
	m_audio = new XAudioManager();
	m_math = new XMath();
	m_assetManager = new XAssetManager;

	m_mainFunc = config.main;
	m_drawFunc = config.draw;
	m_updateFunc = config.update;
	m_endFunc = config.end;

	m_console->m_engine = this;

	XWindow::show();
	XGraphics::init();

	if(!m_math)
	{
		m_math = new XMath;
	}
	
	try
	{
		// Print application message
		LOG("** x2D Game Engine **");
	
		m_console->s_initialized = true;

		// Load plugins
		/*
		if(config.loadPluginsFunc != 0 && config.loadPluginsFunc(scriptEngine) < 0)
		{
			LOG("Loading plugins...");
			return X2D_PLUGIN_LOAD_ERROR;
		}*/

		// Load events
		/*if(config.loadEventsFunc != 0 && config.loadEventsFunc(scriptEngine) < 0)
		{
			return X2D_PLUGIN_LOAD_ERROR;
		}*/

		m_mainFunc();
	
		LOG("x2D Engine Initialized");
		s_initialized = true;
	}
	catch(XException e)
	{
		LOG("An exception occured: %s", e.message().c_str());
		return e.errorCode();
	}
	catch(...)
	{
		LOG("Unknown exception occured.");
		return X2D_UNKNOWN_EXCEPTION;
	}
	return X2D_OK;
}

void XEngine::draw()
{
	if(s_sceneStack.size() > 0)
	{
		s_sceneStack.top()->drawEvent(); // TODO: The current scene can be cached to avoid redundant top() calls
	}
	else
	{
		m_drawFunc();
	}
	m_graphics->swapBuffers();
}

void XEngine::update()
{
	XInput::checkBindings();
	if(s_sceneStack.size() > 0)
	{
		s_sceneStack.top()->updateEvent();
	}
	else
	{
		m_updateFunc();
	}
}

void XEngine::exit()
{
	s_running = false;
}

int XEngine::run()
{
	assert(s_initialized);

	try
	{
		// Setup game loop
		m_timer->start();
		float prevTime = m_timer->getElapsedTime() * 0.001f;
		float acc = 0.0f;

		// Fps sampling
		const int numFpsSamples = 8;
		float fpsSamples[numFpsSamples];
		int currFpsSample = 0;

		s_running = true;

		// Game loop
		while(s_running)
		{
			// Step begin
			/*if(m_debugger)
			{
				m_debugger->getProfiler()->stepBegin();
			}*/

			// Process game events
			XWindow::processEvents();

			// Check if game is paused or out of focus
			if(s_paused || !XWindow::hasFocus())
				continue;

			// Calculate time delta
			const float currentTime = m_timer->getElapsedTime() * 0.001f;
			float deltaTime = currentTime - prevTime;
			prevTime = currentTime;
		
			// Avoid spiral of death
			if(deltaTime > 0.25f)
				deltaTime = 0.25f;
		
			// Apply time delta to acc
			acc += deltaTime;
			while(acc >= XGraphics::s_timeStep)
			{
				// Update the game
				update();
				acc -= XGraphics::s_timeStep;
			}

			// Draw the game
			draw();

			// Calculate fps
			if(deltaTime != 0.0f)
			{
				fpsSamples[currFpsSample++] = 1.0f/deltaTime;
			}

			if(currFpsSample >= numFpsSamples)
			{
				float fps = 0.0f;
				for(int i = 0; i < numFpsSamples; i++) fps += fpsSamples[i];
				XGraphics::s_framesPerSecond = (float)int(fps/numFpsSamples);
				currFpsSample = 0;
			}
		}
	}
	catch(XException e)
	{
		LOG("An exception occured: %s", e.message().c_str());
		return e.errorCode();
	}
	catch(...)
	{
		LOG("Unknown exception occured.");
		return X2D_UNKNOWN_EXCEPTION;
	}

	LOG("Exiting x2D Engine...");

	m_endFunc();

	// Return OK
	return X2D_OK;
}