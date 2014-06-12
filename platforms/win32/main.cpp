//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)
#include <x2d/engine.h>

// Windows application
#include "windebug.h"
#include "winassetloader.h"

#include "source/gfx/opengl/glgraphics.h"
#include "source/sfx/openal/alaudio.h"

#include "window.h"
#include "input.h"
#include "filesystem.h"
#include "timer.h"
#include "console.h"

// Visual Leak Detector
#if defined(X2D_WINDOWS) && defined(X2D_DEBUG)
#include <vld.h>
#endif

#include <direct.h>

int loadPlugins()
{
	return 0;
}

// Win32 entry point
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// Process the command-line
	int flags = 0;
	for(int i = 0; i < __argc; i++) {
		if(strcmp(__argv[i], "-d") == 0)
			flags |= XD_DEBUG;
		else if(strcmp(__argv[i], "-v") == 0)
			flags |= XD_EXPORT_LOG;
	}

	xdEngine *engine = CreateEngine();

	xdConfig config;
	config.loadPluginsFunc = &loadPlugins;
	config.platform = "windows";
#ifdef X2D_DEBUG
	config.workDir = "C:\\Users\\Marcus\\Documents\\GitHub\\Overworld\\";
	flags |= XD_DEBUG;
#else
	config.workDir = _getcwd(0, 0);
#endif
	config.flags = flags;
	config.timer = new Timer;
	config.fileSystem = new FileSystem;
	config.console = new Console;
	config.audio = new OpenAL;
	Input *input = new Input;
	config.input = input;
	OpenGL *graphics = new OpenGL;
	config.graphics = graphics;
	Window *window = new Window(engine, input, graphics);
	config.window = window;
	graphics->init(window);

	if(engine->init(config) != XD_OK) {
		delete engine;
		return -1;
	}

	window->initEvents();

	int r = engine->run();
	delete engine;
	return r;
}