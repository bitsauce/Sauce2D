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
	SHGetKnownFolderPath(FOLDERID_Documents, 0, 0, &path);
	string str = utf8_encode(path);
	CoTaskMemFree(path);
	return str + "\\My Games";
}

//------------------------------------------------------------------------
// Run
//------------------------------------------------------------------------
int Engine::init(const Config &config)
{
	// Set platform string and program dir
	s_flags = config.flags;

	s_workDir = config.workDir;
	if(s_workDir.empty())
	{
		s_workDir = _getcwd(0, 0);
	}
	replace(s_workDir.begin(), s_workDir.end(), '\\', '/');
	util::toDirectoryPath(s_workDir);

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

	Window::show();
	Graphics::init();
	
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

void Engine::draw(const float alpha)
{
	m_drawFunc(m_graphics->s_graphicsContext, alpha);
	m_graphics->swapBuffers();
}

void Engine::update(const float dt)
{
	//Input::checkBindings();
	//m_updateFunc();
}

void Engine::exit()
{
	s_running = false;
}

int Engine::run()
{
	assert(s_initialized);

	try
	{
		// Fps sampling
		const int numFpsSamples = 8;
		float fpsSamples[numFpsSamples];
		int currFpsSample = 0;

		// Set running
		s_running = true;

		// Setup game loop
		m_timer->start();
		const float dt = 1.0f / 30.0f;
		float accumulator = 0.0f;
		float prevTime = m_timer->getElapsedTime() * 0.001f;

		// Lets make sure update is called once before draw
		if(m_updateFunc) m_updateFunc(dt);

		// Game loop
		while(s_running)
		{
			// Process game events
			Window::processEvents();

			// Check if game is paused or out of focus
			if(s_paused || (!isEnabled(XD_RUN_IN_BACKGROUND) && !Window::hasFocus()))
			{
				continue;
			}

			// Calculate time delta
			const float currentTime = m_timer->getElapsedTime() * 0.001f;
			float deltaTime = currentTime - prevTime;
			prevTime = currentTime;
		
			// Avoid spiral of death
			if(deltaTime > 0.25f)
			{
				deltaTime = 0.25f;
			}

			// Step begin
			if(m_stepBeginFunc) m_stepBeginFunc();

			// Apply time delta to accumulator
			accumulator += deltaTime;
			while(accumulator >= dt)
			{
				// Update the game
				Input::checkBindings();
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

			// Calculate fps
			if(deltaTime != 0.0f)
			{
				fpsSamples[currFpsSample++] = 1.0f/deltaTime;
			}

			if(currFpsSample >= numFpsSamples)
			{
				float fps = 0.0f;
				for(int i = 0; i < numFpsSamples; i++) fps += fpsSamples[i];
				Graphics::s_framesPerSecond = (float)int(fps/numFpsSamples);
				currFpsSample = 0;
			}

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