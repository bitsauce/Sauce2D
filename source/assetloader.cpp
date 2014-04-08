//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/              
//		MacroByte (C)

#include "x2d/assetloader.h"
#include "x2d/iosystem.h"

#include <freeimage.h>

// Load image from assets
int X2DAssetLoader::loadAssetAsImage(const char* assetPath, uint** data, uint &width, uint &height, const X2DImageFormat format)
{
	int r = X2D_OK;
	if(format == UnknownImage)
	{
		// Load asset as a image
		const char *assetData = 0;
		loadAsset(assetPath, &assetData);

		// attach the binary data to a memory stream
		FIMEMORY *hmem = FreeImage_OpenMemory((uchar*)assetData);
		
		// get the file type
		FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem);
		
		// load an image from the memory stream
		FIBITMAP *bitmap = FreeImage_LoadFromMemory(fif, hmem, 0);
		
		*data = (uint*)FreeImage_GetBits(bitmap);
		
		// always close the memory stream
		FreeImage_CloseMemory(hmem);

		delete[] assetData;
	}
	return r;
}

int X2DAssetLoader::saveImage(const char* filePath, uint *data, const uint width, const uint height, const X2DImageFormat format)
{
	if(format == UnknownImage)
	{
		FIBITMAP *bitmap = FreeImage_ConvertFromRawBits((uchar*)data, width, height, width * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);
		
		// For now, let's just save everything as png
		FreeImage_Save(FIF_PNG, bitmap, filePath);
	}
	return X2D_OK;
}