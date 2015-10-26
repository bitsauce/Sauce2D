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

#include <boxer/boxer.h>

BEGIN_XD_NAMESPACE

#ifdef X2D_LINUX
#define MAX_PATH 256
#endif

Game::Game() :
	m_flags(0),
	m_workDir(""),
	m_saveDir(""),
	m_inputConfig("")
{
}

void Game::setFlags(const uint flags)
{
	m_flags = flags;
}

uint Game::getFlags() const
{
	return m_flags;
}

void Game::setWorkDir(const string & workDir)
{
	m_workDir = workDir;
}

string Game::getWorkDir() const
{
	return m_workDir;
}

void Game::setSaveDir(const string & saveDir)
{
	m_saveDir = saveDir;
}

string Game::getSaveDir() const
{
	return m_saveDir;
}

void Game::setInputConfig(const string & inputConfig)
{
	m_inputConfig = inputConfig;
}

string Game::getInputConfig() const
{
	return m_inputConfig;
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
Game * Engine::s_game = 0;

Engine::Engine()
{
}

Engine::~Engine()
{
	s_game->end();

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

Exception::Exception(RetCode code, const char * msg, ...) :
	m_errorCode(code)
{
	va_list args;
	va_start(args, msg);

	int size = _scprintf(msg, args) + 1;
	m_message.resize(size);
	vsprintf_s(&m_message[0], size, msg, args);

	va_end(args);
}

//------------------------------------------------------------------------
// Run
//------------------------------------------------------------------------
int Engine::init(Game * game)
{
	// Set game
	s_game = game;

	// Set and process flags
	uint flags = game->getFlags();
	for(int i = 0; i < __argc; i++)
	{
		string arg = __argv[i];
		if(arg == "-export-log")
		{
			flags |= XD_EXPORT_LOG;
		}
		else if(arg == "-verbose")
		{
			flags |= XD_VERBOSE;
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
	string workDir = game->getWorkDir();
	if(workDir.empty())
	{
		workDir = _getcwd(0, 0);
	}
	replace(workDir.begin(), workDir.end(), '\\', '/');
	util::toDirectoryPath(workDir);
	game->setWorkDir(workDir);

	// Set save directory
	string saveDir = game->getSaveDir();
	if(saveDir.empty())
	{
		saveDir = getSaveDir();
	}
	replace(saveDir.begin(), saveDir.end(), '\\', '/');
	util::toDirectoryPath(saveDir);
	game->setSaveDir(saveDir);

	m_console = new Console();
	m_fileSystem = new FileSystem();
	if(isEnabled(XD_EXPORT_LOG))
	{
		m_console->m_output = new FileWriter(util::getAbsoluteFilePath(":/Console.log"));
	}

	m_timer = new Timer();
	m_graphics = new Graphics();
	m_audio = new AudioManager();

	m_console->m_engine = this;

	glfwSetErrorCallback(error_callback);

	if(!glfwInit())
	{
		assert("GLFW could not initialize");
	}

	Window::init(800, 600, false);
	Graphics::init();
	Input::init(game->getInputConfig());

	try
	{
		// Print application message
		LOG("** x2D Game Engine **");

		// Start game
		s_game->start(m_graphics->s_graphicsContext);

		// Set initialized
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

bool Engine::isEnabled(const EngineFlag flag)
{
	return (s_game->getFlags() & flag) != 0;
}

int Engine::run()
{
	// Make sure we've run init
	if(!s_initialized)
	{
		return X2D_NOT_INITIALIZED;
	}

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
		s_game->update(dt);

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

			// Avoid spiral of death
			if(deltaTime > 0.25)
			{
				deltaTime = 0.25;
			}

			// Step begin
			s_game->stepBegin();

			// Apply time delta to accumulator
			accumulator += deltaTime;
			while(accumulator >= dt)
			{
				// Update the game
				Input::updateBindings();
				s_game->update(dt);
				accumulator -= dt;
			}

			// Draw the game
			const double alpha = accumulator / dt;
			s_game->draw(m_graphics->s_graphicsContext, alpha);
			m_graphics->swapBuffers();

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

			// Step end
			s_game->stepEnd();
		}
	}
	catch(Exception e)
	{
		boxer::show(e.message().c_str(), "Exception");
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

END_XD_NAMESPACE