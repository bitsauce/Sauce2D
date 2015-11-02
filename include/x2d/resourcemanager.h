#ifndef X2D_RESOURCE_MANAGER_H
#define X2D_RESOURCE_MANAGER_H

#include "config.h"

/*********************************************************************
**	Resource manager												**
**********************************************************************/

BEGIN_XD_NAMESPACE

/**
 * \brief This class handles resource loading and handling.
 *
 * Resources in this context are objects which are loaded from the hard drive
 * and which should only be read once.
 *
 * Resources are loaded as follows \code ResouceClassPtr resource = ResourceManager::get<ResourceClass>(":/path_to_file.res"); \endcode
 * Where ResourceClass is the name the class of the type of resource you are trying to load.
 * ResouceManager can currently load image files and font files.
 */
class XDAPI ResourceManager
{
public:
	/**
	 * Returns a resource given a file path. If this is the first time the
	 * resource was accessed, it will be loaded from the disk. If it was
	 * already loaded, this returns a pointer to the existing resource.
	 *
	 * \param filePath The file path to the resource file
	 * \note This can only load Texture2D and Font.
	 */
	template<typename T>
	static shared_ptr<T> get(const string &filePath)
	{
		// NOTE TO SELF: I need to notify the resource manager of resources that have been deleted so I can remove it from s_resources.
		if(s_resources.find(filePath) != s_resources.end()) {
			return *(shared_ptr<T>*)s_resources[filePath];
		}

		shared_ptr<T> resource = T::loadResource(filePath);
		s_resources[filePath] = new shared_ptr<T>(resource);
		return resource;
	}
private:
	static map<string, void*> s_resources;
};

END_XD_NAMESPACE

#endif // X2D_RESOURCE_MANAGER_H