//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>

#include <x2d/timer.h>
#include <x2d/filesystem.h>
#include <x2d/window.h>
#include <x2d/input.h>
#include <x2d/math.h>
#include <x2d/scriptengine.h>
#include <x2d/graphics.h>
#include <x2d/audio.h>
#include <x2d/console.h>
#include <x2d/profiler.h>
#include <x2d/debug.h>
#include <x2d/assetloader.h>

// AngelScript add-ons
#include "scripts/scriptstdstring.h"
#include "scripts/scriptbuilder.h"
#include "scripts/scripthelper.h"
#include "scripts/scriptarray.h"
#include "scripts/scriptgrid.h"
#include "scripts/scriptany.h"

#ifdef X2D_LINUX
#define MAX_PATH 256
#endif

xdEngine *g_engine = 0;
xdEngine *xdEngine::s_this = 0;

xdConfig::xdConfig() :
	flags(0),
	platform(""),
	workDir(""),
	loadPluginsFunc(0),
	timer(0),
	input(0),
	math(0),
	window(0),
	fileSystem(0),
	graphics(0),
	audio(0),
	console(0)
{
}

xdEngine *CreateEngine()
{
	return new xdEngine();
}

//------------------------------------------------------------------------
// Engine
//------------------------------------------------------------------------

AS_REG_SINGLETON(xdEngine, "ScriptEngine")

int xdEngine::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectMethod("ScriptEngine", "void exit()", asMETHOD(xdEngine, exit), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptEngine", "string get_platform() const", asMETHOD(xdEngine, getPlatformString), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptEngine", "string get_workDir() const", asMETHOD(xdEngine, getWorkingDirectory), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("ScriptEngine", "void toggleProfiler()", asMETHOD(xdEngine, toggleProfiler), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptEngine", "void pushProfile(const string &in)", asMETHOD(xdEngine, pushProfile), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptEngine", "void popProfile()", asMETHOD(xdEngine, popProfile), asCALL_THISCALL); AS_ASSERT

	return r;
}

xdEngine::xdEngine() :
	m_debugger(0),
	m_profiler(0),
	m_running(false),
	m_paused(false),
	m_updateFunc(0),
	m_drawFunc(0),
	m_initialized(false),
	m_toggleProfiler(false)
{
	s_this = this;
	g_engine = this;
}

xdEngine::~xdEngine()
{
#ifdef NO
	// Make sure the server is disconnected
	if(!gameServer->isDisconnected())
		gameServer->disconnect();

	// Make sure the client is disconnected
	if(!gameClient->isDisconnected())
		gameClient->disconnect();

	// Close sockets
	closeSockets();
#endif
	
	delete m_fileSystem;
	delete m_graphics;
	delete m_audio;
	delete m_profiler;
	delete m_timer;
	delete m_scripts;
	delete m_window;
	delete m_math;
	delete m_input;
	delete m_assetLoader;
	delete m_debugger;
	delete m_console;
}

string xdEngine::getPlatformString() const
{
	return m_platformString;
}

string xdEngine::getWorkingDirectory() const
{
	return m_workDir;
}

void xdEngine::toggleProfiler()
{
	m_toggleProfiler = true;
}

void xdEngine::pushProfile(const string &profile)
{
	m_profiler->pushProfile(profile);
}

void xdEngine::popProfile()
{
	m_profiler->popProfile();
}

#include <ctime>

//------------------------------------------------------------------------
// Run
//------------------------------------------------------------------------
int xdEngine::init(const xdConfig &config)
{
	if(!config.isValid()) {
		return XD_INVALID_CONFIG;
	}

	// Set platform string and program dir
	//applyConfig(config);
	m_flags = config.flags;
	m_platformString = config.platform;

	m_workDir = config.workDir;
	replace(m_workDir.begin(), m_workDir.end(), '\\', '/');
	if(m_workDir.back() != '/') {
		m_workDir += "/";
	}

	m_timer = config.timer;
	m_fileSystem = config.fileSystem;
	m_graphics = config.graphics;
	m_audio = config.audio;
	m_console = config.console;
	m_window = config.window;
	m_math = config.math;
	m_input = config.input;
	m_assetLoader = new xdAssetLoader(m_fileSystem);

	// Set up stuff
	m_assetLoader->s_this = m_assetLoader;
	m_fileSystem->s_this = m_fileSystem;
	m_graphics->s_this = m_graphics;
	//m_audio->s_this = m_audio;

	if(!m_profiler) {
		m_profiler = new xdProfiler(m_timer);
	}

	if(!m_console) {
		m_console = new xdConsole();
	}
	m_console->m_fileSystem = m_fileSystem;

	if(!m_window) {
		m_window = new xdWindow;
	}

	if(!m_math) {
		m_math = new xdMath;
	}

	// Check for debug flag
	if(IsEnabled(XD_DEBUG) && m_debugger)
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
					m_debugger->sendPacket(XD_CONNECTED_PACKET);
				}else{
					// Failed to connect to external debugger (timed out)
				}
			}else{
				// Failed to listen on port
			}
		}else{
			// Failed to initialize socket
		}
		m_console->m_debugger = m_debugger;
	}
	if(IsEnabled(XD_EXPORT_LOG)) {
		m_console->m_output = xdFileSystem::CreateFileWriter(util::getAbsoluteFilePath(":/console.txt"));
	}
	
	// Print application message
	LOG("** x2D Game Engine **");
	
	// Create the script engine
	LOG("Initializing AngelScript (%s)", ANGELSCRIPT_VERSION_STRING);
	asIScriptEngine *scriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	scriptEngine->SetEngineProperty(asEP_COMPILER_WARNINGS, 0);
	
	// Create script manager
	m_scripts = new xdScriptEngine(scriptEngine, m_debugger);

	// Set the message callback to receive information on errors in human readable form.
	int r = scriptEngine->SetMessageCallback(asFUNCTION(asMessageCallback), 0, asCALL_CDECL); assert( r >= 0 );

	// Register stuff
	RegisterStdString(scriptEngine);
	RegisterScriptArray(scriptEngine, true);
	RegisterStdStringUtils(scriptEngine);
	RegisterScriptGrid(scriptEngine);
	RegisterScriptAny(scriptEngine);

	// This will register all game objects
	Base::Register(scriptEngine);
	
	// Register singleton objects
	r = scriptEngine->RegisterGlobalProperty("ScriptEngine Engine", this); AS_ASSERT
	r = scriptEngine->RegisterGlobalProperty("ScriptFileSystem FileSystem", m_fileSystem); AS_ASSERT
	r = scriptEngine->RegisterGlobalProperty("ScriptWindow Window", m_window); AS_ASSERT
	r = scriptEngine->RegisterGlobalProperty("ScriptInput Input", m_input); AS_ASSERT
	r = scriptEngine->RegisterGlobalProperty("ScriptMath Math", m_math); AS_ASSERT
	r = scriptEngine->RegisterGlobalProperty("ScriptManager Scripts", m_scripts); AS_ASSERT
	r = scriptEngine->RegisterGlobalProperty("ScriptGraphics Graphics", m_graphics); AS_ASSERT
	r = scriptEngine->RegisterGlobalProperty("ScriptAudio Audio", m_audio); AS_ASSERT
	r = scriptEngine->RegisterGlobalProperty("ScriptConsole Console", m_console); AS_ASSERT
	r = scriptEngine->RegisterGlobalProperty("ScriptProfiler Profiler", m_profiler); AS_ASSERT

	// Create network managers
	//initSockets();
	//gameClient = new Client();
	//gameServer = new Server();

	srand((uint)time(0));

	// Load plugins
	if(config.loadPluginsFunc != 0 && config.loadPluginsFunc(m_scripts) < 0) {
		return XD_PLUGIN_LOAD_ERROR;
	}

	// Compile the AngelScript module
	LOG("Compiling scripts...");
	r = asCompileModule("GameModule", m_fileSystem);
	if(r < 0)
	{
		// Script compilation failed
		return XD_COMPILE_FAILED;
	}
	
	// Create the game module
	asIScriptModule *mod = scriptEngine->GetModule("GameModule");
	m_scripts->m_module = mod;

	// Find the function that is to be called.
	asIScriptFunction* mainFunc = mod->GetFunctionByDecl("void main()");
	asIScriptFunction* updateFunc = mod->GetFunctionByDecl("void update()");
	asIScriptFunction* drawFunc = mod->GetFunctionByDecl("void draw()");
	if(!mainFunc || !updateFunc || !drawFunc)
	{
		// The function couldn't be found, exit the application
		LOG("Could not find 'void main()', 'void update()' and 'void draw()'. Please make sure these functions exists.");
		//exception(XD_MISSING_MAIN);
		return XD_MISSING_MAIN;
	}
	m_updateFunc = updateFunc;
	m_drawFunc = drawFunc;

	LOG("Running void main()...");

	// Create our context and run main
	asIScriptContext* ctx = m_scripts->createContext();
	r = ctx->Prepare(mainFunc); assert(r >= 0);
	r = ctx->Execute(); assert(r >= 0);

	// Check for run-time error
	if(r != asEXECUTION_FINISHED)
	{
		// Get exception section and line
		const char *sectionName; int line;
		ctx->GetExceptionLineNumber(&line, &sectionName);
		
		// Format output string
		LOG("Run-time exception '%s' occured in function '%s' in file '%s:%i'",
			ctx->GetExceptionString(), ctx->GetExceptionFunction()->GetDeclaration(), sectionName, line);
		//exception(XD_RUNTIME_EXCEPTION);
		return XD_RUNTIME_EXCEPTION;
	}
	r = ctx->Release(); assert(r >= 0);

	if(m_debugger)
	{
		// Tell the debugger the engine is initialized
		m_debugger->sendPacket(XD_INITIALIZED_PACKET);
	}
	
	LOG("x2D Engine Initialized");
	m_initialized = true;
	return XD_OK;
}

void xdEngine::draw()
{
	// Start draw
	m_profiler->pushProfile("Draw");

	asIScriptContext *ctx = m_scripts->createContext();
	int r = ctx->Prepare(m_drawFunc); assert(r >= 0);
	r = ctx->Execute(); assert(r >= 0);
	r = ctx->Release(); assert(r >= 0);

	m_graphics->swapBuffers();

	// End draw
	m_profiler->popProfile();
}

void xdEngine::update()
{
	m_profiler->pushProfile("Update");

	asIScriptContext *ctx = m_scripts->createContext();
	int r = ctx->Prepare(m_updateFunc); assert(r >= 0);
	r = ctx->Execute(); assert(r >= 0);
	if(r != asEXECUTION_FINISHED)
	{
		// Get exception section and line
		const char *sectionName;
		int line;
		ctx->GetExceptionLineNumber(&line, &sectionName);
		
		// Format output string
		ERR("Run-Time exception '%s' occured in function '%s' in file '%s:%i'",
			ctx->GetExceptionString(), ctx->GetExceptionFunction()->GetDeclaration(), sectionName, line);
	}
	ctx->GetEngine()->GarbageCollect(asGC_ONE_STEP);
	r = ctx->Release(); assert(r >= 0);
	
	m_profiler->popProfile();
}

void xdEngine::exit()
{
	// TODO: post exit event
	m_running = false;
}

int xdEngine::run()
{
	assert(m_initialized);

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
		// Toggle profiler
		if(m_toggleProfiler) {
			if(!m_profiler->isActive()) {
				m_profiler->start();
			}else{
				m_profiler->stop();
				m_profiler->printResults();
			}
			m_toggleProfiler = false;
		}
		m_profiler->pushProfile("Game Loop");

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
			m_graphics->m_framesPerSecond = int(fps/numFpsSamples);
			currFpsSample = 0;
		}
		
		m_profiler->popProfile();
	}

	// Return OK
	return XD_OK;
}