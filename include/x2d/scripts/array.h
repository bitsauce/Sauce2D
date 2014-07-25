#ifndef X2D_ARRAY_H
#define X2D_ARRAY_H

#include <x2d/config.h>
#include <x2d/util.h>

class XDAPI Array
{
public:
	virtual void AddRef() const = 0;
	virtual void Release() const = 0;

	virtual void Reserve(unsigned int maxElements) = 0;
	virtual void Resize(unsigned int numElements) = 0;
	virtual void Clear() = 0;
	virtual uint GetSize() const = 0;
	virtual bool IsEmpty() const = 0;

	virtual void *At(unsigned int index) = 0;
	virtual const void *At(unsigned int index) const = 0;
	virtual void  SetValue(unsigned int index, void *value) = 0;
	
	virtual void InsertLast(void *value) = 0;

	virtual string GetElementTypeName() const = 0;
};

XDAPI Array *CreateArray(const string &type, const uint size);

#define NOT_IMPLEMENTED_ARR(func, type) Array *arr = CreateArray(type, 0); LOG("%s does not have a implementation.", #func); return arr;

#endif // X2D_ARRAY_H