//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/              
//		MacroByte (C)

#include "x2d/engine.h"

// Windows application
#include "winapp.h"
#include "windebug.h"
#include "openal.h"
#include "opengl.h"
#include "winassetloader.h"

// Visual Leak Detector
#if defined(X2D_WINDOWS) && defined(X2D_DEBUG)
#include <vld.h>
#endif

// Win32 entry point
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// Process the command-line
	int flags = 0;
	for(int i = 0; i < __argc; i++) {
		if(strcmp(__argv[i], "-d") == 0)
			flags |= X2D_Debug;
		else if(strcmp(__argv[i], "-v") == 0)
			flags |= X2D_ExportLog;
	}
	
	string assetDir;
#ifdef X2D_DEBUG
	assetDir = "C:/Users/Marcus/Dropbox/Random Projects/ScriptDebug/";
	//assetDir = "C:/Users/Marcus/Documents/GitHub/ctw/";
	flags |= X2D_Debug;
#endif

	// Init engine
	X2DEngine *engine = CreateEngine(flags);

	// Create input/output
	WinDebug debug;
	engine->debug = &debug;

	// Create asset loader
	AssetLoader assetLoader(assetDir);
	engine->assetLoader = &assetLoader;

	// Create renderer
	OpenGLRender gfx(engine, &assetLoader);
	engine->gfx = &gfx;

	// Create sound manager
	OpenALManager sfx;
	engine->sfx = &sfx;

	// Create window applicaation
	Win32App app(engine, &gfx);
	engine->app  = &app;
	
	gfx.init();
	sfx.init();

	// Initialize engine
	if(engine->init() != X2D_OK)
		return -1; // Initialization failed
	app.initEvents();

	// Run engine (and return)
	return engine->run();
}