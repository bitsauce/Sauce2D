#ifndef X2D_ANY_OBJECT_H
#define X2D_ANY_OBJECT_H

#include <x2d/config.h>
#include <x2d/util.h>

class XDAPI AnyObject
{
public:
	virtual int AddRef() const = 0;
	virtual int Release() const = 0;

	virtual void Store(void *ref, int refTypeId) = 0;
	virtual bool Retrieve(void *ref, int refTypeId) const = 0;
};

XDAPI AnyObject *CreateAnyObject();

#endif // X2D_ANY_OBJECT_H