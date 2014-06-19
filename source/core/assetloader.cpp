//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/assetloader.h>
#include <x2d/filesystem.h>

#include <freeimage.h>

xdAssetLoader *xdAssetLoader::s_this = 0;

xdAssetLoader::xdAssetLoader(xdFileSystem *fileSystem) :
	m_fileSystem(fileSystem)
{
}

uint read(void *buffer, uint size, uint count, fi_handle handle)
{
	return fread(buffer, size, count, (FILE*)handle);
}



// Load image from assets
int xdAssetLoader::loadImage(string filePath, uchar** data, uint &width, uint &height, const xdImageFormat format)
{
	int r = XD_OK;
	if(format == UnknownImage)
	{
		// Load asset as a image
		string content;
		m_fileSystem->readFile(filePath, content);

		// attach the binary data to a memory stream
		FIMEMORY *hmem = FreeImage_OpenMemory((uchar*)content.c_str(), content.size());
		
		// get the file type
		FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem);
		
		// load an image from the memory stream
		FIBITMAP *bitmap = FreeImage_LoadFromMemory(fif, hmem, 0);
		
		width = FreeImage_GetWidth(bitmap);
		height = FreeImage_GetHeight(bitmap);
		*data = new uchar[width*height*4];
		memcpy(*data, FreeImage_GetBits(bitmap), width*height*4);
		
		// always close the memory stream
		FreeImage_Unload(bitmap);
		FreeImage_CloseMemory(hmem);
	}
	return r;
}

int xdAssetLoader::saveImage(string filePath, uchar *data, const uint width, const uint height, const xdImageFormat format)
{
	/*if(format == UnknownImage)
	{
		FIBITMAP *bitmap = FreeImage_ConvertFromRawBits((uchar*)data, width, height, width * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);
		
		// For now, let's just save everything as png
		FreeImage_Save(FIF_PNG, bitmap, filePath);
	}*/
	return XD_OK;
}