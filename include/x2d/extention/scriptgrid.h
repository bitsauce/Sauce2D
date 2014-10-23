#ifndef X2D_SCRIPT_GRID_H
#define X2D_SCRIPT_GRID_H

#include "../engine.h"

struct SGridBuffer;

class XScriptGrid
{
public:
	// Set the memory functions that should be used by all CScriptGrids
	static void SetMemoryFunctions(asALLOCFUNC_t allocFunc, asFREEFUNC_t freeFunc);

	// Factory functions
	static XScriptGrid *Create(asIObjectType *ot);
	static XScriptGrid *Create(asIObjectType *ot, asUINT width, asUINT height);
	static XScriptGrid *Create(asIObjectType *ot, asUINT width, asUINT height, void *defaultValue);
	static XScriptGrid *Create(asIObjectType *ot, void *listBuffer);

	// Memory management
	void AddRef() const;
	void Release() const;

	// Type information
	asIObjectType *GetGridObjectType() const;
	int            GetGridTypeId() const;
	int            GetElementTypeId() const;

	// Size
	asUINT GetWidth() const;
	asUINT GetHeight() const;
	void   Resize(asUINT width, asUINT height);

	// Get a pointer to an element. Returns 0 if out of bounds
	void       *At(asUINT x, asUINT y);
	const void *At(asUINT x, asUINT y) const;

	// Set value of an element
	// Remember, if the grid holds handles the value parameter should be the 
	// address of the handle. The refCount of the object will also be incremented
	void  SetValue(asUINT x, asUINT y, void *value);
	
	// Copy the contents of one array to another (only if the types are the same)
	XScriptGrid &operator=(const XScriptGrid&);

	// GC methods
	int  GetRefCount();
	void SetFlag();
	bool GetFlag();
	void EnumReferences(asIScriptEngine *engine);
	void ReleaseAllHandles(asIScriptEngine *engine);

protected:
	mutable int       refCount;
	mutable bool      gcFlag;
	asIObjectType    *objType;
	SGridBuffer      *buffer;
	int               elementSize;
	int               subTypeId;

	// Constructors
	XScriptGrid(asIObjectType *ot, void *initBuf); // Called from script when initialized with list
	XScriptGrid(asUINT w, asUINT h, asIObjectType *ot);
	XScriptGrid(asUINT w, asUINT h, void *defVal, asIObjectType *ot);
	virtual ~XScriptGrid();

	bool  CheckMaxSize(asUINT x, asUINT y);
	void  CreateBuffer(SGridBuffer **buf, asUINT w, asUINT h);
	void  DeleteBuffer(SGridBuffer *buf);
	void  CopyBuffer(SGridBuffer *dst, SGridBuffer *src);
	void  Construct(SGridBuffer *buf);
	void  Destruct(SGridBuffer *buf);
	void  SetValue(SGridBuffer *buf, asUINT x, asUINT y, void *value);
	void *At(SGridBuffer *buf, asUINT x, asUINT y);
};

void RegisterScriptGrid(asIScriptEngine *engine);

#endif // X2D_SCRIPT_GRID_H
