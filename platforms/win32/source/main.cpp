//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/x2d.h>
#include <direct.h> // for _getcw()
#include <ShlObj.h>

// Windows application
#include "common/window.h"
#include "common/input.h"
#include "common/filesystem.h"
#include "common/timer.h"
#include "common/console.h"
#include "common/plugins.h"
#include "common/debug.h"

#include "audio/openal/alaudio.h"
#include "graphics/opengl/glgraphics.h"

// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring &wstr)
{
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo( size_needed, 0 );
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// Convert an UTF8 string to a wide Unicode String
std::wstring utf8_decode(const std::string &str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo( size_needed, 0 );
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

// Visual Leak Detector
#if defined(X2D_WINDOWS) && defined(X2D_DEBUG)
#include <vld.h>
#endif

// Win32 entry point
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// Process the command-line
	int flags = 0;
	string workDir;
	bool useDebugger = false;
	for(int i = 0; i < __argc; i++)
	{
		if(__argv[i][0] == '-')
		{
			switch(__argv[i][1])
			{
			case 'd': useDebugger = true; break;
			case 'v': flags |= XD_EXPORT_LOG; break;
			case 'w': workDir = string(__argv[i]+3); break;
			}
		}
	}
	flags |= XD_EXPORT_LOG; // For now we force this flag

	XEngine *engine = CreateEngine();

	XConfig config;
	config.loadPluginsFunc = &loadPlugins;
	config.loadEventsFunc = &loadEvents;
	config.platform = "windows";
#ifdef X2D_DEBUG
	//config.workDir = "C:\\Users\\Marcus\\Documents\\GitHub\\Overworld\\";
	//config.workDir = "C:\\Users\\Marcus\\Documents\\GitHub\\x2D-Game-Engine\\examples\\Debug\\";
	config.workDir = "C:\\Users\\Marcus\\Dropbox\\Game Development\\Testing\\TerrainGen\\";
	//engine->setDebugger(new Debugger);
#else
	config.workDir = workDir.empty() ? _getcwd(0, 0) : workDir.c_str();
	if(useDebugger)
	{
		engine->setDebugger(new Debugger);
	}
#endif
	string saveDir = getSaveDir();
	config.saveDir = saveDir.c_str();
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

	if(engine->init(config) != X2D_OK)
	{
		delete engine;
		return -1;
	}

	int r = engine->run();
	delete engine;
	cleanPlugins();
	return r;
}