#include "android_asset_loader.h"
#include "android_engine.h"

#include <android/asset_manager.h>

AndroidAssetLoader::AndroidAssetLoader(AppManager *manager) :
	m_manager(manager)
{
	assert(manager);
}

int AndroidAssetLoader::loadAsset(const char* assetPath, const char** data, long* len)
{
	iosystem::print("Now opening asset '%s'", assetPath);
	
	// Get file path
	if(memcmp(assetPath, ":/", 2) == 0)
	{
		// Atempt to open asset
		AAssetManager *assetManager = m_manager->android->activity->assetManager;
		AAsset* asset = AAssetManager_open(assetManager, assetPath+2, AASSET_MODE_BUFFER);
		if(!asset)
		{
			iosystem::error("Couldn't open asset '%s'", assetPath);
			return X2D_AssetLoadError;
		}
	
		// Read asset data
		long length = AAsset_getLength(asset);
		*data = new char[length];
		AAsset_read(asset, (void*)(*data), length);
		AAsset_close(asset);
		if(len != 0) *len = length;
	}else{
		// Implies system file
		// TODO: implement
	}
	
	// Asset loaded successfully
	return X2D_OK;
}

void AndroidAssetLoader::loadPlugins()
{
}

int AndroidAssetLoader::transformFilePath(string &assetPath)
{
	// ???
	return X2D_OK;
}