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
stack<Scene*> Engine::s_sceneStack;
Scene *Engine::s_showScene = 0;
Scene *Engine::s_hideScene = 0;

// System dirs
string Engine::s_workDir;
string Engine::s_saveDir;

Engine::Engine()
{
}

Engine::~Engine()
{
	// Pop all scene objects
	while(s_sceneStack.size() > 0)
	{
		Scene *scene = s_sceneStack.top();
		if(scene) scene->hideEvent();
		s_sceneStack.pop();
	}

	m_endFunc();
	
	delete m_fileSystem;
	delete m_graphics;
	delete m_audio;
	delete m_timer;
	delete m_console;
}

void Engine::pushScene(Scene *scene)
{
	// Show and hide scenes
	s_showScene = scene;
	s_hideScene = s_sceneStack.size() > 0 ? s_sceneStack.top() : 0;

	// Add scene to stack
	s_sceneStack.push(scene);
}

void Engine::popScene()
{
	// Make sure there is a scene to pop
	if(s_sceneStack.size() == 0) return;

	// Hide and pop topmost scene
	s_hideScene = s_sceneStack.top();
	s_sceneStack.pop();

	// Show next scene
	s_showScene = s_sceneStack.size() > 0 ? s_sceneStack.top() : 0;
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
	SHGetKnownFolderPath(FOLDERID_SavedGames, 0, 0, &path);
	string str = utf8_encode(path);
	CoTaskMemFree(path);
	return str;
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

	m_mainFunc = config.main;
	m_drawFunc = config.draw;
	m_updateFunc = config.update;
	m_stepBeginFunc = config.stepBegin;
	m_stepEndFunc = config.stepEnd;
	m_endFunc = config.end;

	m_console->m_engine = this;

	Window::show();
	Graphics::init();
	
	try
	{
		// Print application message
		LOG("** x2D Game Engine **");
	
		m_console->s_initialized = true;

		m_mainFunc();
	
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

void Engine::draw()
{
	m_drawFunc(m_graphics->s_graphicsContext);
	m_graphics->swapBuffers();
}

void Engine::update()
{
	Input::checkBindings();
	m_updateFunc();
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
			// Process game events
			Window::processEvents();

			// Check if game is paused or out of focus
			if(s_paused || !Window::hasFocus())
			{
				continue;
			}

			// Step begin
			if(m_stepBeginFunc) m_stepBeginFunc();

			// Show new scene before hiding current scene to keep common resources alive
			if(s_showScene)
			{
				s_showScene->showEvent();
				s_showScene = 0;
			}
			
			if(s_hideScene)
			{
				s_hideScene->hideEvent();
				s_hideScene = 0;
			}

			// Calculate time delta
			const float currentTime = m_timer->getElapsedTime() * 0.001f;
			float deltaTime = currentTime - prevTime;
			prevTime = currentTime;
		
			// Avoid spiral of death
			if(deltaTime > 0.25f)
				deltaTime = 0.25f;
		
			// Apply time delta to acc
			acc += deltaTime;
			while(acc >= xd::Graphics::s_timeStep)
			{
				// Update the game
				update();
				acc -= xd::Graphics::s_timeStep;
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
				xd::Graphics::s_framesPerSecond = (float)int(fps/numFpsSamples);
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