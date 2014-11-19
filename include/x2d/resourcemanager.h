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
	
class XDAPI RefCounter
{
public:
	RefCounter() :
		refCount(1)
	{
	}
	
	RefCounter(const RefCounter &) :
		refCount(1)
	{
	}

	void addRef()
	{
		++refCount;
	}

	int release()
	{
		return --refCount;
	}

	int get() const
	{
		return refCount;
	}

private:
	int refCount;
};

class XPixmap;
//class XSound;

template<typename T>
class XDAPI Resource
{
public:
	Resource(T *res) :
		m_resourceObject(res)
	{
		m_refCounter = new RefCounter();
		m_refCounter->addRef();
	}

	Resource(const Resource<T> &other) :
		m_resourceObject(other.m_resourceObject),
		m_refCounter(other.m_refCounter)
	{
		m_refCounter->addRef();
	}

	~Resource()
	{
		if(m_refCounter->release() == 0)
		{
			delete m_resourceObject;
			delete m_refCounter;
		}
	}

	T &operator*()
	{
		return *m_resourceObject;
	}

	T *operator->()
	{
		return m_resourceObject;
	}

	Resource<T> &operator=(const Resource<T> &other)
	{
		if(this != &other)
		{
			if(m_refCounter->release() == 0)
			{
				delete m_resourceObject;
				delete m_refCounter;
			}

			m_resourceObject = other.m_resourceObject;
			m_refCounter = other.m_refCounter;
			m_refCounter->release();
		}
		return *this;
	}

	operator T*() const
	{
		return m_resourceObject;
	}

private:
	T *m_resourceObject;
	RefCounter *m_refCounter;
};

class XDAPI ResourceManager
{
public:
	template<typename T>
	static Resource<T> get(const string &name)
	{
		if(s_resources.find(name) != s_resources.end())
			return *(Resource<T>*)s_resources[name];
		return T::loadResource(name);
	}
private:
	static map<string, void*> s_resources;
};

}

#endif // X2D_RESOURCE_MANAGER_H