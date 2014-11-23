#ifndef X2D_RESOURCE_MANAGER_H
#define X2D_RESOURCE_MANAGER_H

#include "config.h"

/*********************************************************************
**	Resource manager												**
**********************************************************************/

namespace xd
{

// Image formats
/*enum XImageFormat
{
	ANY_IMAGE_TYPE,
	PNG_IMAGE_TYPE,
	BMP_IMAGE_TYPE,
	DDS_IMAGE_TYPE,
	PSD_IMAGE_TYPE,
	TGA_IMAGE_TYPE,
	JPG_IMAGE_TYPE
};

// Sound formats
enum XSoundFormat
{
	ANY_SOUND_TYPE,
	WAV_SOUND_TYPE,
	MP3_SOUND_TYPE,
	OGG_SOUND_TYPE
};*/

class XDAPI ResourceManager
{
public:
	template<typename T>
	static shared_ptr<T> get(const string &name)
	{
		// NOTE TO SELF: I need to notify the resource manager of resources that have been deleted so I can remove it from s_resources.
		if(s_resources.find(name) != s_resources.end()) {
			return *(shared_ptr<T>*)s_resources[name];
		}

		shared_ptr<T> resource = T::loadResource(name);
		s_resources[name] = new shared_ptr<T>(resource);
		return resource;
	}
private:
	static map<string, void*> s_resources;
};

}

#endif // X2D_RESOURCE_MANAGER_H