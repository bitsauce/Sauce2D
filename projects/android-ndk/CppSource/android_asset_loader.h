#ifndef ANDROID_ASSETLOADER_H
#define ANDROID_ASSETLOADER_H

#include "x2d/assetLoader.h"

struct AppManager;

class AndroidAssetLoader : public X2DAssetLoader
{
public:
	AndroidAssetLoader(AppManager *manager);

	int loadAsset(const char* filePath, const char** data, long* len = 0);
	void loadPlugins();
	int transformFilePath(string &assetPath);

private:
	AppManager *m_manager;
};

#endif // ANDROID_ASSETLOADER_H