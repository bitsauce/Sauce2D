//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "winassetloader.h"
#include <x2d/util.h>

#include <fstream>
#include <sstream>

// Plugin globals
typedef void (*PLUGIN_FACTORY)();
typedef void (*PLUGIN_CLEANUP)();
vector<HMODULE> plugins;
/*
AssetLoader::AssetLoader(const string &assetDir)
{
	// If a working directory isn't specified, assing one
	if(assetDir.empty())
	{
		// Get application path
		char appPath[MAX_PATH];
		GetModuleFileName(NULL, appPath, MAX_PATH);

		// Remove everything after the last backslash
		string appStr = appPath;
		appStr.erase(appStr.find_last_of("\\") + 1);

		// Set work directory
		m_assetDir = appStr;
	}else{
		m_assetDir = assetDir;
		replace(m_assetDir.begin(), m_assetDir.end(), '/', '\\');
	}
}

AssetLoader::~AssetLoader()
{
	// Clean up plugins
	for(uint i = 0; i < plugins.size(); i++)
	{
			PLUGIN_CLEANUP cleanupFunc = (PLUGIN_CLEANUP)GetProcAddress(plugins[i], "ReleasePlugin");
			cleanupFunc();
			FreeLibrary(plugins[i]);
	}
}

int AssetLoader::transformFilePath(string &assetPath)
{
	if(assetPath.substr(0, 2) != ":/")
		return -1;

	assetPath = m_assetDir + assetPath.substr(2, assetPath.size()-2);
	return X2D_OK;
}

int AssetLoader::loadAsset(const char* assetPath, const char** data, long* len)
{
	// Get file path
	string filePath;
	if(memcmp(assetPath, ":/", 2) == 0)
	{
		// Implies asset
		filePath = m_assetDir + string(assetPath + 2);
	}else{
		// Implies system file
		filePath = assetPath;
	}
	
	// Load asset file
	FILE *fp;
	fopen_s(&fp, filePath.c_str(), "rb");
	if(!fp)
	{
		// Failed to open
		return X2D_AssetLoadError;
	}
	
	// Read file content
	string content;
	fseek(fp, 0, SEEK_END);
	content.resize(ftell(fp));
	rewind(fp);
	fread(&content[0], 1, content.size(), fp);
	fclose(fp);

	int length = content.size();
	*data = new char[length];
	memcpy((void*)*data, content.c_str(), length);
	if(len != 0) *len = length;

	return X2D_OK;
}

void AssetLoader::loadPlugins()
{
	// Find all DLLs in the directory
	char search_parms[MAX_PATH];
	strcpy_s(search_parms, MAX_PATH, m_assetDir.c_str());
#ifdef _DEBUG
	strcat_s(search_parms, MAX_PATH, "plugins\\*.dll.debug");
#else
	strcat_s(search_parms, MAX_PATH, "plugins\\*.dll");
#endif
	
	WIN32_FIND_DATA find_data;
	HANDLE h_find = FindFirstFile(search_parms, &find_data);
	BOOL f_ok = TRUE;
	while(h_find != INVALID_HANDLE_VALUE && f_ok)
	{
		// Load each DLL and determine whether it is exporting
		// the functions we care about
		char plugin_full_name [MAX_PATH];
		strcpy_s(plugin_full_name, MAX_PATH, m_assetDir.c_str());
		strcat_s(plugin_full_name, MAX_PATH, "plugins\\");
		strcat_s(plugin_full_name, MAX_PATH, find_data.cFileName);
		
		HMODULE h_mod = LoadLibrary(plugin_full_name);
		if(h_mod != 0)
		{
			PLUGIN_FACTORY factoryFunc = (PLUGIN_FACTORY)GetProcAddress(h_mod, "CreatePlugin");
			PLUGIN_CLEANUP cleanupFunc = (PLUGIN_CLEANUP)GetProcAddress(h_mod, "ReleasePlugin");
			if(factoryFunc != 0 && cleanupFunc != 0)
			{
				// Call factory func
				factoryFunc();

				// Add cleanup func to a list
				plugins.push_back(h_mod);
			}
		}
		
		// Go for the next DLL
		f_ok = FindNextFile(h_find, &find_data);
	}
}*/