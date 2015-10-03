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

#include <direct.h> // for _getcw()
#include <ShlObj.h>

BEGIN_XD_NAMESPACE

#ifdef X2D_LINUX
#define MAX_PATH 256
#endif

Config::Config() :
	flags(0),
	workDir("")
{
}

//------------------------------------------------------------------------
// Engine
//------------------------------------------------------------------------

Engine *CreateEngine()
{
	return new Engine();
}

bool Engine::s_initialized = false;
bool Engine::s_paused = false;
bool Engine::s_running = false;
int Engine::s_flags = 0;

// System dirs
string Engine::s_workDir;
string Engine::s_saveDir;

Engine::Engine()
{
}

Engine::~Engine()
{
	if(m_endFunc) m_endFunc();

	delete m_fileSystem;
	delete m_graphics;
	delete m_audio;
	delete m_timer;
	delete m_console;
}

// Convert a wide Unicode string to an UTF8 string
string utf8_encode(const wstring &wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(), NULL, 0, NULL, NULL);
	string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

// Convert an UTF8 string to a wide Unicode String
wstring utf8_decode(const string &str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), NULL, 0);
	wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int) str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

string getSaveDir()
{
	wchar_t *path;
	SHGetKnownFolderPath(FOLDERID_Documents, 0, 0, &path);
	string str = utf8_encode(path);
	CoTaskMemFree(path);
	return str + "\\My Games";
}

void error_callback(int error, const char* description)
{
	LOG(description);
}

//------------------------------------------------------------------------
// Run
//------------------------------------------------------------------------
int Engine::init(const Config &config)
{
	// Set and process flags
	s_flags = config.flags;

	string workDir;
	for(uint i = 0; i < __argc; i++)
	{
		string arg = __argv[i];
		if(arg == "-export-log")
		{
			s_flags |= XD_EXPORT_LOG;
		}
		else if(arg == "-verbose")
		{
			s_flags |= XD_VERBOSE;
		}
	}

	// Set current directory to exe location
	{
		char* programPath = 0;
		_get_pgmptr(&programPath);
		string str = programPath;
		str = str.substr(0, str.find_last_of('\\'));
		SetCurrentDirectory(str.c_str());
	}

	// Set working directory
	s_workDir = config.workDir;
	if(s_workDir.empty())
	{
		s_workDir = _getcwd(0, 0);
	}
	replace(s_workDir.begin(), s_workDir.end(), '\\', '/');
	util::toDirectoryPath(s_workDir);

	// Set save directory
	s_saveDir = getSaveDir();
	replace(s_saveDir.begin(), s_saveDir.end(), '\\', '/');
	util::toDirectoryPath(s_saveDir);

	m_console = new Console();
	m_fileSystem = new FileSystem();
	if(isEnabled(XD_EXPORT_LOG))
	{
		m_console->m_output = new FileWriter(util::getAbsoluteFilePath(":/console.log"));
	}

	m_timer = new Timer();
	m_graphics = new Graphics();
	m_audio = new AudioManager();

	m_mainFunc = config.mainFunc;
	m_drawFunc = config.drawFunc;
	m_updateFunc = config.updateFunc;
	m_stepBeginFunc = config.stepBeginFunc;
	m_stepEndFunc = config.stepEndFunc;
	m_endFunc = config.endFunc;

	m_console->m_engine = this;

	glfwSetErrorCallback(error_callback);

	if(!glfwInit())
	{
		assert("GLFW could not initialize");
	}

	Window::init(800, 600, false);
	Graphics::init();
	Input::init();

	try
	{
		// Print application message
		LOG("** x2D Game Engine **");

		m_console->s_initialized = true;

		if(m_mainFunc) m_mainFunc(m_graphics->s_graphicsContext);

		LOG("x2D Engine Initialized");
		s_initialized = true;
	}
	catch(Exception e)
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

void Engine::exit()
{
	Window::close();
}

int Engine::run()
{
	assert(s_initialized);

	try
	{
		// Fps sampling
		const uint numFpsSamples = 8;
		double fpsSamples[numFpsSamples];
		for(uint i = 0; i < numFpsSamples; ++i) fpsSamples[i] = 0.0;
		uint currFpsSample = 0;

		// Set running
		s_running = true;

		// Setup game loop
		m_timer->start();
		const double dt = 1.0 / 30.0;
		double accumulator = 0.0;
		double prevTime = m_timer->getElapsedTime();

		// Lets make sure update is called once before draw
		if(m_updateFunc) m_updateFunc(dt);

		// Game loop
		while(!glfwWindowShouldClose(Window::s_window))
		{
			// Process game events
			glfwPollEvents();

			// Check if game is paused or out of focus
			if(s_paused || (!isEnabled(XD_RUN_IN_BACKGROUND) && !Window::hasFocus()))
			{
				continue;
			}

			// Calculate time delta
			const double currentTime = m_timer->getElapsedTime();
			double deltaTime = currentTime - prevTime;
			prevTime = currentTime;

			LOG("deltaTime: %f", deltaTime);

			// Avoid spiral of death
			if(deltaTime > 0.25)
			{
				deltaTime = 0.25;
			}

			// Step begin
			if(m_stepBeginFunc) m_stepBeginFunc();

			// Apply time delta to accumulator
			accumulator += deltaTime;
			while(accumulator >= dt)
			{
				// Update the game
				Input::updateBindings();
				if(m_updateFunc) m_updateFunc(dt);
				accumulator -= dt;
			}

			// Draw the game
			if(m_drawFunc)
			{
				const double alpha = accumulator / dt;
				m_drawFunc(m_graphics->s_graphicsContext, alpha);
				m_graphics->swapBuffers();
			}

			// Add fps sample
			if(deltaTime != 0.0f)
			{
				fpsSamples[currFpsSample++] = 1.0 / deltaTime;
				if(currFpsSample >= numFpsSamples)
				{
					currFpsSample = 0;
				}
			}

			// Get average fps
			double fps = 0.0;
			for(uint i = 0; i < numFpsSamples; i++) fps += fpsSamples[i];
			Graphics::s_framesPerSecond = fps / numFpsSamples;

			if(m_stepEndFunc) m_stepEndFunc();
		}
	}
	catch(Exception e)
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

	// Return OK
	return X2D_OK;
}

}