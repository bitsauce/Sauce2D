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

XEngine *XEngine::s_this = 0;

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

XEngine::XEngine() :
	m_debugger(0),
	m_running(false),
	m_paused(false),
	m_initialized(false)
{
	s_this = this;
}

XEngine::~XEngine()
{
	// Pop all scene objects
	/*while(m_sceneStack.size() > 0)
	{
		popScene();
	}*/
	
	delete m_input;
	delete m_fileSystem;
	delete m_graphics;
	delete m_audio;
	delete m_timer;
	delete m_window;
	delete m_math;
	delete m_assetManager;
	delete m_console;
}

string XEngine::getWorkingDirectory() const
{
	return m_workDir;
}

string XEngine::getSaveDirectory() const
{
	return m_saveDir;
}

/*void XEngine::pushScene(Scene *scene)
{
	// Get previous scene
	asIScriptObject *prevScene = m_sceneStack.size() > 0 ? m_sceneStack.top() : 0;

	if(prevScene)
	{
		// Call hide on previous scene
		asIScriptFunction *hideFunc = prevScene->GetObjectType()->GetMethodByDecl("void hide()");
		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(hideFunc); assert(r >= 0);
		r = ctx->SetObject(prevScene); assert(r >= 0);
		r = ctx->Execute(); AS_CHECK_RUNTIME_ERROR
		r = ctx->Release();
	}

	// Add scene to stack
	m_sceneStack.push(object);

	// Clear garbage
	int r = XScriptEngine::GetAngelScript()->GarbageCollect(asGC_FULL_CYCLE | asGC_DESTROY_GARBAGE); assert(r >= 0);

	if(object)
	{
		// Cache draw and update functions
		asIObjectType *objectType = object->GetObjectType();
		m_sceneDrawFunc = objectType->GetMethodByDecl("void draw()");
		m_sceneUpdateFunc = objectType->GetMethodByDecl("void update()");

		// Call show func
		asIScriptFunction *showFunc = objectType->GetMethodByDecl("void show()");
		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(showFunc); assert(r >= 0);
		r = ctx->SetObject(object); assert(r >= 0);
		r = ctx->Execute(); AS_CHECK_RUNTIME_ERROR
		r = ctx->Release();
	}
}

void XEngine::popScene()
{
	if(m_sceneStack.size() > 0)
	{
		// Get topmost scene
		asIScriptObject *object = m_sceneStack.top();

		// Call hide func on scene
		asIScriptFunction *hideFunc = object->GetObjectType()->GetMethodByDecl("void hide()");
		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(hideFunc); assert(r >= 0);
		r = ctx->SetObject(object); assert(r >= 0);
		r = ctx->Execute(); AS_CHECK_RUNTIME_ERROR
		r = ctx->Release();

		// Relese the ref held by the engine
		if(object) object->Release();

		// Pop scene
		m_sceneStack.pop();
		
		// Clear garbage
		r = XScriptEngine::GetAngelScript()->GarbageCollect(asGC_FULL_CYCLE | asGC_DESTROY_GARBAGE); assert(r >= 0);

		// Get next scene
		asIScriptObject *nextScene = m_sceneStack.size() > 0 ? m_sceneStack.top() : 0;
		if(nextScene)
		{
			// Cache draw and update functions
			asIObjectType *objectType = nextScene->GetObjectType();
			m_sceneDrawFunc = objectType->GetMethodByDecl("void draw()");
			m_sceneUpdateFunc = objectType->GetMethodByDecl("void update()");

			// Call show on topmost scene
			asIScriptFunction *showFunc = objectType->GetMethodByDecl("void show()");
			asIScriptContext *ctx = XScriptEngine::CreateContext();
			int r = ctx->Prepare(showFunc); assert(r >= 0);
			r = ctx->SetObject(nextScene); assert(r >= 0);
			r = ctx->Execute(); AS_CHECK_RUNTIME_ERROR
			r = ctx->Release();
		}
	}
}*/

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
	m_flags = config.flags;

	m_workDir = config.workDir;
	replace(m_workDir.begin(), m_workDir.end(), '\\', '/');
	util::toDirectoryPath(m_workDir);

	m_saveDir = getSaveDir();
	replace(m_saveDir.begin(), m_saveDir.end(), '\\', '/');
	util::toDirectoryPath(m_saveDir);
	
	m_console = new XConsole();
	m_fileSystem = new XFileSystem();
	if(IsEnabled(XD_EXPORT_LOG))
	{
		m_console->m_output = new XFileWriter(util::getAbsoluteFilePath(":/console.log"));
	}

	m_timer = new XTimer();
	m_graphics = new XGraphics();
	m_audio = new XAudioManager();
	m_math = new XMath();
	m_input = new XInput();
	m_assetManager = new XAssetManager;

	m_mainFunc = config.main;
	m_drawFunc = config.draw;
	m_updateFunc = config.update;

	m_console->m_engine = this;

	m_window = new XWindow(this, m_input, m_graphics);
	m_graphics->init(m_window);

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
		m_initialized = true;
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
	m_drawFunc();
	m_graphics->swapBuffers();
}

void XEngine::update()
{
	m_input->checkBindings();
	m_updateFunc();
}

void XEngine::exit()
{
	m_running = false;
}

int XEngine::run()
{
	assert(m_initialized);

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

		m_running = true;

		// Game loop
		while(m_running)
		{
			// Step begin
			/*if(m_debugger)
			{
				m_debugger->getProfiler()->stepBegin();
			}*/

			// Process game events
			m_window->processEvents();

			// Check if game is paused or out of focus
			if(m_paused)// || !m_focus)
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
			while(acc >= m_graphics->m_timeStep)
			{
				// Update the game
				update();
				acc -= m_graphics->m_timeStep;
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
				m_graphics->m_framesPerSecond = (float)int(fps/numFpsSamples);
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

	// Return OK
	return X2D_OK;
}