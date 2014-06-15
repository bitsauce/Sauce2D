#include "plugins.h"
#include <x2d/engine.h>
#include <x2d/util.h>

// Plugin globals
typedef int  (*PLUGIN_FACTORY)(xdScriptEngine*);
typedef void (*PLUGIN_CLEANUP)();
vector<HMODULE> plugins;

int loadPlugins(xdScriptEngine *scriptEngine)
{
	string pluginDir = util::getAbsoluteFilePath(":/plugins/");

	// Find all DLLs in the directory
	char search_parms[MAX_PATH];
	strcpy_s(search_parms, MAX_PATH, pluginDir.c_str());
#ifdef _DEBUG
	strcat_s(search_parms, MAX_PATH, "*.dll.debug");
#else
	strcat_s(search_parms, MAX_PATH, "*.dll");
#endif
	
	WIN32_FIND_DATA find_data;
	HANDLE h_find = FindFirstFile(search_parms, &find_data);
	BOOL f_ok = TRUE;
	while(h_find != INVALID_HANDLE_VALUE && f_ok)
	{
		// Load each DLL and determine whether it is exporting
		// the functions we care about
		char plugin_full_name [MAX_PATH];
		strcpy_s(plugin_full_name, MAX_PATH, pluginDir.c_str());
		strcat_s(plugin_full_name, MAX_PATH, "");
		strcat_s(plugin_full_name, MAX_PATH, find_data.cFileName);
		
		HMODULE h_mod = LoadLibrary(plugin_full_name);
		if(h_mod != 0)
		{
			PLUGIN_FACTORY factoryFunc = (PLUGIN_FACTORY)GetProcAddress(h_mod, "CreatePlugin");
			PLUGIN_CLEANUP cleanupFunc = (PLUGIN_CLEANUP)GetProcAddress(h_mod, "ReleasePlugin");
			if(factoryFunc != 0 && cleanupFunc != 0)
			{
				// Call factory func
				int r = factoryFunc(scriptEngine);
				if(r < 0) return r;

				// Add cleanup func to a list
				plugins.push_back(h_mod);
			}
		}
		
		// Go for the next DLL
		f_ok = FindNextFile(h_find, &find_data);
	}
	return 0;
}

void cleanPlugins()
{
	// Clean up plugins
	for(uint i = 0; i < plugins.size(); i++)
	{
			PLUGIN_CLEANUP cleanupFunc = (PLUGIN_CLEANUP)GetProcAddress(plugins[i], "ReleasePlugin");
			cleanupFunc();
			FreeLibrary(plugins[i]);
	}
}
