#ifndef X2D_ASSETLOADER_H
#define X2D_ASSETLOADER_H

#include "common/config.h"

/*********************************************************************
**	Asset loader image formats										**
**********************************************************************/
enum XImageFormat
{
	UNKNOWN_IMAGE_TYPE,
	PNG_IMAGE_TYPE,
	BMP_IMAGE_TYPE
	//DdsImage,
	//PdsImage,
	//TgaImage,
	//JpgImage
};

/*********************************************************************
**	Asset loader sound formats										**
**********************************************************************/
enum XSoundFormat
{
	UNKNOWN_SOUND_TYPE,
	WAV_SOUND_TYPE,
	//Mp3Sound,
	//OggSound
};

/*********************************************************************
**	Asset manager													**
**********************************************************************/
class XAudioBuffer;
class XPixmap;

class XDAPI XAssetManager
{
	friend class XEngine;
public:
	static XPixmap *LoadPixmap(string filePath, const XImageFormat format = UNKNOWN_IMAGE_TYPE);
	static void SavePixmap(string filePath, XPixmap *pixmap, const XImageFormat format = UNKNOWN_IMAGE_TYPE);

	static XAudioBuffer *LoadSound(string filePath, const XSoundFormat format = UNKNOWN_SOUND_TYPE);
	static void SaveSound(string filePath, XAudioBuffer *buffer, const XSoundFormat format = UNKNOWN_SOUND_TYPE);
	
private:
	static XFileSystem *s_fileSystem;
};

#endif // X2D_ASSETLOADER_H