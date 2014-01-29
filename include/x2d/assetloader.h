#ifndef X2D_ASSETLOADER_H
#define X2D_ASSETLOADER_H

#include "x2d/platform.h"

/*********************************************************************
**	Asset loader image formats										**
**********************************************************************/
enum X2DImageFormat
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
enum X2DSoundFormat
{
	UnknownSound,
	WavSound,
	//Mp3Sound,
	//OggSound
};

/*********************************************************************
**	Abstract Asset Loader											**
**********************************************************************/
class X2DAPI X2DAssetLoader
{
public:

	/** Loads a asset file
	  * IMPLEMENTATION: Required
	  * INPUT: This function takes a asset relative path [assetPath]
	  * OUTPUT: The asset file data output as [data], and the length of this data as [len]
	  */
	virtual int loadAsset(const char* assetPath, const char** data, long* len = 0) = 0;
	virtual void loadPlugins() = 0;
	virtual int transformFilePath(string &assetPath) = 0;

public:

	/** Loads a asset file as a image using thirdparty libraries
	  * IMPLEMENTATION: Implement if you perfer other means of loading image files
	  * INPUT: The asset directory relative path [assetPath], and the image format as [format].
	  * OUTPUT: The image [data] rgba encoded as unsigned integers, and the [width] and [height] of the image
	  */
	int loadAssetAsImage(const char* assetPath, uint** data, uint &width, uint &height, const X2DImageFormat format = UnknownImage);
	int loadAssetAsSound(const char* assetPath, uint** data/*, int &frequency*/, const X2DSoundFormat format = UnknownSound);
	
	int loadImage(const char* filePath, uint** data, int &width, int &height, const X2DImageFormat format = UnknownImage);
	int saveImage(const char* filePath, uint *data, const uint width, const uint height, const X2DImageFormat format = UnknownImage);

	int loadSound(const char* filePath, uint** data, int &width, int &height, const X2DSoundFormat format = UnknownSound);
	int saveSound(const char* filePath, uint *data, const uint width, const uint height, const X2DSoundFormat format = UnknownSound);

	// TODO: Add settings file reading/writing (.ini/.cfg)

};

#endif // X2D_ASSETLOADER_H