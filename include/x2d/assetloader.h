#ifndef X2D_ASSETLOADER_H
#define X2D_ASSETLOADER_H

#include <x2d/config.h>
#include <x2d/util.h>

/*********************************************************************
**	Asset loader image formats										**
**********************************************************************/
enum xdImageFormat
{
	UnknownImage,
	PngImage,
	BmpImage
	//DdsImage,
	//PdsImage,
	//TgaImage,
	//JpgImage
};

/*********************************************************************
**	Asset loader sound formats										**
**********************************************************************/
enum xdSoundFormat
{
	UnknownSound,
	WavSound,
	//Mp3Sound,
	//OggSound
};

/*********************************************************************
**	Abstract Asset Loader											**
**********************************************************************/

class xdFileSystem;

class XDAPI xdAssetLoader
{
	friend class xdEngine;
	friend class xdGraphics;
public:
	xdAssetLoader(xdFileSystem *fileSystem);

private:
	int loadImage(string filePath, uchar** data, uint &width, uint &height, const xdImageFormat format = UnknownImage);
	int saveImage(string filePath, uchar *data, const uint width, const uint height, const xdImageFormat format = UnknownImage);

	int loadSound(string filePath, uchar** data, int &width, int &height, const xdSoundFormat format = UnknownSound);
	int saveSound(string filePath, uchar *data, const uint width, const uint height, const xdSoundFormat format = UnknownSound);
	
	xdFileSystem *m_fileSystem;
	static xdAssetLoader *s_this;
};

#endif // X2D_ASSETLOADER_H