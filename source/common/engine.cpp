//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/extention.h>
#include <x2d/graphics.h>
#include <x2d/audio.h>

#include <ctime>

#ifdef X2D_LINUX
#define MAX_PATH 256
#endif

XEngine *XEngine::s_this = 0;

XConfig::XConfig() :
	flags(0),
	platform(""),
	workDir(""),
	saveDir(""),
	loadPluginsFunc(0),
	timer(0),
	input(0),
	window(0),
	fileSystem(0),
	graphics(0),
	audio(0),
	console(0)
{
}

//------------------------------------------------------------------------
// Engine
//------------------------------------------------------------------------

AS_REG_SINGLETON(XEngine)

XEngine *CreateEngine()
{
	return new XEngine();
}

int XEngine::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	// General
	r = scriptEngine->RegisterObjectMethod("XEngine", "void exit()", asMETHOD(XEngine, exit), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XEngine", "string get_platform() const", asMETHOD(XEngine, getPlatformString), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XEngine", "string get_workDir() const", asMETHOD(XEngine, getWorkingDirectory), asCALL_THISCALL); AS_ASSERT
	
	// Scene
	r = scriptEngine->RegisterObjectMethod("XEngine", "void pushScene(Scene@)", asMETHOD(XEngine, pushScene), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XEngine", "void popScene()", asMETHOD(XEngine, popScene), asCALL_THISCALL); AS_ASSERT

	return r;
}

XEngine::XEngine() :
	m_debugger(0),
	m_running(false),
	m_paused(false),
	m_defaultUpdateFunc(0),
	m_defaultDrawFunc(0),
	m_sceneUpdateFunc(0),
	m_sceneDrawFunc(0),
	m_initialized(false),
	m_toggleProfiler(false)
{
	s_this = this;
}

XEngine::~XEngine()
{
	// Pop all scene objects
	while(m_sceneStack.size() > 0)
	{
		popScene();
	}
	
	delete m_input;
	delete m_scripts;
	delete m_fileSystem;
	delete m_graphics;
	delete m_audio;
	delete m_timer;
	delete m_window;
	delete m_math;
	delete m_assetManager;
	delete m_debugger;
	delete m_console;
}

string XEngine::getPlatformString() const
{
	return m_platformString;
}

string XEngine::getWorkingDirectory() const
{
	return m_workDir;
}

string XEngine::getSaveDirectory() const
{
	return m_saveDir;
}

void XEngine::toggleProfiler()
{
	m_toggleProfiler = true;
}

void XEngine::pushScene(asIScriptObject *object)
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
		r = ctx->Execute();
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
		r = ctx->Execute();
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
		r = ctx->Execute();
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
			r = ctx->Execute();
			r = ctx->Release();
		}
	}
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
	//applyConfig(config);
	m_flags = config.flags;
	m_platformString = config.platform;

	m_workDir = config.workDir;
	replace(m_workDir.begin(), m_workDir.end(), '\\', '/');
	util::toDirectoryPath(m_workDir);

	m_saveDir = config.saveDir;
	replace(m_saveDir.begin(), m_saveDir.end(), '\\', '/');
	util::toDirectoryPath(m_saveDir);

	m_timer = config.timer;
	m_fileSystem = config.fileSystem;
	m_graphics = config.graphics;
	m_audio = config.audio;
	m_console = config.console;
	m_window = config.window;
	m_math = new XMath;
	m_input = config.input;
	m_assetManager = new XAssetManager;

	// Set up stuff
	//m_assetManager->s_this = m_assetManager;
	m_fileSystem->s_this = m_fileSystem;
	m_graphics->s_this = m_graphics;
	m_audio->s_this = m_audio;

	if(!m_console)
	{
		m_console = new XConsole;
	}
	m_console->s_this = m_console;

	if(!m_window)
	{
		m_window = new XWindow;
	}

	if(!m_math)
	{
		m_math = new XMath;
	}

	// Check for debug flag
	if(IsEnabled(X2D_USE_DEBUGGER) && m_debugger)
	{
		// Init debugger
		if(m_debugger->init())
		{
			// Socket initialized, listen on port 5120
			if(m_debugger->listen(5120))
			{
				// Try to connect to the external debugger
				if(m_debugger->connect())
				{
					// Tell the external debugger the connection was successful
					m_debugger->sendPacket(X2D_CONNECTED_PACKET);
				}
				else
				{
					// Failed to connect to external debugger (timed out)
				}
			}
			else
			{
				// Failed to listen on port
			}
		}
		else
		{
			// Failed to initialize socket
		}
		m_console->m_debugger = m_debugger;
	}

	if(IsEnabled(X2D_EXPORT_LOG))
	{
		m_console->m_output = XFileSystem::CreateFileWriter(util::getAbsoluteFilePath(":/console.log"));
	}
	
	try {
		// Print application message
		LOG("** x2D Game Engine **");
	
		// Create the script engine
		LOG("Initializing AngelScript %s", ANGELSCRIPT_VERSION_STRING);
		asIScriptEngine *scriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
		scriptEngine->SetEngineProperty(asEP_COMPILER_WARNINGS, 0);

		m_console->s_initialized = true;
	
		// Create script manager
		m_scripts = new XScriptEngine;
		m_scripts->s_debugger = m_debugger;
		m_scripts->s_engine = scriptEngine;

		// Set the message callback to receive information on errors in human readable form.
		int r = scriptEngine->SetMessageCallback(asFUNCTION(asMessageCallback), 0, asCALL_CDECL); assert( r >= 0 );

		// Register stuff
		RegisterStdString(scriptEngine);
		RegisterScriptArray(scriptEngine, true);
		RegisterStdStringUtils(scriptEngine);
		RegisterScriptGrid(scriptEngine);
		RegisterScriptAny(scriptEngine);
		RegisterScriptDictionary(scriptEngine);

		r = scriptEngine->RegisterInterface("Scene"); AS_ASSERT
		r = scriptEngine->RegisterInterfaceMethod("Scene", "void show()"); AS_ASSERT
		r = scriptEngine->RegisterInterfaceMethod("Scene", "void hide()"); AS_ASSERT
		r = scriptEngine->RegisterInterfaceMethod("Scene", "void draw()"); AS_ASSERT
		r = scriptEngine->RegisterInterfaceMethod("Scene", "void update()"); AS_ASSERT

		// This will register all game objects
		ClassRegister::Register(scriptEngine);
	
		// Register singleton objects
		r = scriptEngine->RegisterGlobalProperty("XEngine Engine", this); AS_ASSERT
		r = scriptEngine->RegisterGlobalProperty("XFileSystem FileSystem", m_fileSystem); AS_ASSERT
		r = scriptEngine->RegisterGlobalProperty("XWindow Window", m_window); AS_ASSERT
		r = scriptEngine->RegisterGlobalProperty("XInput Input", m_input); AS_ASSERT
		r = scriptEngine->RegisterGlobalProperty("XMath Math", m_math); AS_ASSERT
		r = scriptEngine->RegisterGlobalProperty("XScriptEngine Scripts", m_scripts); AS_ASSERT
		r = scriptEngine->RegisterGlobalProperty("XGraphics Graphics", m_graphics); AS_ASSERT
		r = scriptEngine->RegisterGlobalProperty("XAudioManager Audio", m_audio); AS_ASSERT
		r = scriptEngine->RegisterGlobalProperty("XConsole Console", m_console); AS_ASSERT

		// Create network managers
		//initSockets();
		//gameClient = new Client();
		//gameServer = new Server();

		srand((uint)time(0));

		// Load plugins
		LOG("Loading plugins...");
		if(config.loadPluginsFunc != 0 && config.loadPluginsFunc(scriptEngine) < 0) {
			return X2D_PLUGIN_LOAD_ERROR;
		}

		// Compile the AngelScript module
		LOG("Compiling scripts...");
		r = asCompileModule("GameModule");
		if(r < 0)
		{
			// Script compilation failed
			return X2D_COMPILE_FAILED;
		}
	
		// Create the game module
		asIScriptModule *mod = scriptEngine->GetModule("GameModule");
		m_scripts->s_module = mod;

		// Load events
		if(config.loadEventsFunc != 0 && config.loadEventsFunc(scriptEngine) < 0) {
			return X2D_PLUGIN_LOAD_ERROR;
		}

		// Find the function that is to be called.
		asIScriptFunction* mainFunc = mod->GetFunctionByDecl("void main()");
		asIScriptFunction* updateFunc = mod->GetFunctionByDecl("void update()");
		asIScriptFunction* drawFunc = mod->GetFunctionByDecl("void draw()");
		if(!mainFunc || !updateFunc || !drawFunc)
		{
			// main/update/draw function(s) were not found, exit the application
			LOG("Could not find 'void main()', 'void update()' and 'void draw()'. Please make sure these functions exists.");
			return X2D_MISSING_MAIN;
		}
		m_defaultUpdateFunc = updateFunc;
		m_defaultDrawFunc = drawFunc;

		LOG("Running void main()...");

		// Create our context and run main
		asIScriptContext* ctx = XScriptEngine::CreateContext();
		r = ctx->Prepare(mainFunc); assert(r >= 0);
		r = ctx->Execute();

		// Check for run-time error
		if(r != asEXECUTION_FINISHED)
		{
			// Get exception section and line
			const char *sectionName; int line;
			ctx->GetExceptionLineNumber(&line, &sectionName);
		
			// Format output string
			LOG("Run-time exception '%s' occured in function '%s' in file '%s:%i'",
				ctx->GetExceptionString(), ctx->GetExceptionFunction()->GetDeclaration(), sectionName, line);
			return X2D_RUNTIME_EXCEPTION;
		}
		r = ctx->Release();

		if(IsEnabled(X2D_USE_DEBUGGER) && m_debugger)
		{
			// Tell the debugger the engine is initialized
			m_debugger->sendPacket(X2D_INITIALIZED_PACKET);
		}
	
		LOG("x2D Engine Initialized");
		m_initialized = true;
	}catch(XException e)
	{
		LOG("An exception occured: %s", e.message().c_str());
		return e.errorCode();
	}catch(...)
	{
		asIScriptContext *ctx = asGetActiveContext();
		if(ctx)
		{
			// Will probably never get here as long as AS_NO_EXCEPTION is not defined
			const char *tmp;
			int line = ctx->GetLineNumber(0, 0, &tmp);
			LOG("Unknown exception occured while a script was running.");
			if(tmp) {
				LOG("LOC : %s (%i)", line, tmp);
			}
		}else
		{
			LOG("Unknown exception occured.");
		}
		return X2D_UNKNOWN_EXCEPTION;
	}
	return X2D_OK;
}

void XEngine::draw()
{
	ctxmtx.lock();
	for(XRenderContext **context : m_graphics->s_contextToCreate)
	{
		*context = XGraphics::CreateContext();
	}
	m_graphics->s_contextToCreate.clear();
	ctxmtx.unlock();

	asIScriptObject *object = m_sceneStack.size() > 0 ? m_sceneStack.top() : 0;
	asIScriptFunction *func = object != 0 ? m_sceneDrawFunc : m_defaultDrawFunc;
	if(func)
	{
		// Call draw function
		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		if(object) {
			r = ctx->SetObject(object); assert(r >= 0);
		}
		r = ctx->Execute();
		r = ctx->Release();
	}

	m_graphics->swapBuffers();
}

void XEngine::update()
{
	// Check all bindings
	m_input->checkBindings();
	
	asIScriptObject *object = m_sceneStack.size() > 0 ? m_sceneStack.top() : 0;
	asIScriptFunction *func = object != 0 ? m_sceneUpdateFunc : m_defaultUpdateFunc;
	if(func)
	{
		// Call 'void update()'
		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		if(object) {
			r = ctx->SetObject(object); assert(r >= 0);
		}
		r = ctx->Execute();

		if(r != asEXECUTION_FINISHED)
		{
			// Print runtime exception message
			const char *sectionName; int line;
			ctx->GetExceptionLineNumber(&line, &sectionName);
			LOG("Runtime exception '%s' occured in function '%s' in file '%s:%i'", ctx->GetExceptionString(), ctx->GetExceptionFunction()->GetDeclaration(), sectionName, line);
		}
		r = ctx->Release();
		
		r = XScriptEngine::GetAngelScript()->GarbageCollect(asGC_ONE_STEP | asGC_DESTROY_GARBAGE); assert(r >= 0);
	}
}

void XEngine::exit()
{
	m_running = false;
}

int XEngine::run()
{
	assert(m_initialized);

	try {
		// Setup game loop
		m_timer->start();
		float prevTime = m_timer->getTime();
		float acc = 0.0f;

		// Fps sampling
		const int numFpsSamples = 8;
		float fpsSamples[numFpsSamples];
		int currFpsSample = 0;

		m_running = true;

		// Game loop
		while(m_running)
		{
			// Process game events
			m_window->processEvents();

			// Check if game is paused or out of focus
			if(m_paused)// || !m_focus)
				continue;

			// Calculate time delta
			const float currentTime = m_timer->getTime();
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
			if(deltaTime != 0.0f) fpsSamples[currFpsSample++] = 1.0f/deltaTime;
			if(currFpsSample >= numFpsSamples) {
				float fps = 0.0f;
				for(int i = 0; i < numFpsSamples; i++) fps += fpsSamples[i];
				m_graphics->m_framesPerSecond = (float)int(fps/numFpsSamples);
				currFpsSample = 0;
			}
		}
	}catch(XException e)
	{
		LOG("An exception occured: %s", e.message().c_str());
		return e.errorCode();
	}catch(...)
	{
		asIScriptContext *ctx = asGetActiveContext();
		if(ctx) {
			// Will probably never get here as long as AS_NO_EXCEPTION is not defined
			const char *tmp;
			int line = ctx->GetLineNumber(0, 0, &tmp);
			LOG("Unknown exception occured while a script was running.");
			if(tmp) {
				LOG("LOC : %s (%i)", line, tmp);
			}
		}else{
			LOG("Unknown exception occured.");
		}
		return X2D_UNKNOWN_EXCEPTION;
	}

	LOG("Exiting x2D Engine...");

	// Return OK
	return X2D_OK;
}