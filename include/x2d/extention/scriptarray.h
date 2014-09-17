#ifndef X2D_SCRIPT_ARRAY_H
#define X2D_SCRIPT_ARRAY_H

#include "../config.h"

struct SArrayBuffer;
struct SArrayCache;

class XDAPI XScriptArray
{
public:
	XScriptArray(asUINT length, asIObjectType *ot);

	// Set the memory functions that should be used by all CScriptArrays
	static void SetMemoryFunctions(asALLOCFUNC_t allocFunc, asFREEFUNC_t freeFunc);

	// Factory functions
	static XScriptArray *Create(asIObjectType *ot);
	static XScriptArray *Create(asIObjectType *ot, asUINT length);
	static XScriptArray *Create(asIObjectType *ot, asUINT length, void *defaultValue);
	static XScriptArray *Create(asIObjectType *ot, void *listBuffer);

	// Memory management
	void AddRef() const;
	void Release() const;

	// Type information
	asIObjectType *GetArrayObjectType() const;
	int            GetArrayTypeId() const;
	int            GetElementTypeId() const;
	string         GetElementTypeName() const;


	asUINT GetSize() const;

	// Returns true if the array is empty
	bool   IsEmpty() const;

	// Pre-allocates memory for elements
	void   Reserve(asUINT maxElements);

	// Resize the array
	void   Resize(asUINT numElements);
	
	// Clears the array
	void   Clear();

	// Get a pointer to an element. Returns 0 if out of bounds
	void       *At(asUINT index);
	const void *At(asUINT index) const;

	// Set value of an element. 
	// The value arg should be a pointer to the value that will be copied to the element.
	// Remember, if the array holds handles the value parameter should be the 
	// address of the handle. The refCount of the object will also be incremented
	void  SetValue(asUINT index, void *value);

	// Copy the contents of one array to another (only if the types are the same)
	XScriptArray &operator=(const XScriptArray&);

	// Compare two arrays
	bool operator==(const XScriptArray&) const;

	// Array manipulation
	void InsertAt(asUINT index, void *value);
	void RemoveAt(asUINT index);
	void InsertLast(void *value);
	void RemoveLast();
	void SortAsc();
	void SortDesc();
	void SortAsc(asUINT startAt, asUINT count);
	void SortDesc(asUINT startAt, asUINT count);
	void Sort(asUINT startAt, asUINT count, bool asc);
	void StdSort(asIScriptFunction *func);
	void Reverse();
	int  Find(void *value) const;
	int  Find(asUINT startAt, void *value) const;
	int  FindByRef(void *ref) const;
	int  FindByRef(asUINT startAt, void *ref) const;

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
	SArrayBuffer     *buffer;
	int               elementSize;
	int               subTypeId;

	// Constructors
	XScriptArray(asIObjectType *ot, void *initBuf); // Called from script when initialized with list
	XScriptArray(asUINT length, void *defVal, asIObjectType *ot);
	XScriptArray(const XScriptArray &other);
	virtual ~XScriptArray();

	bool  Less(const void *a, const void *b, bool asc, asIScriptContext *ctx, SArrayCache *cache);
	void *GetArrayItemPointer(int index);
	void *GetDataPointer(void *buffer);
	void  Copy(void *dst, void *src);
	void  Precache();
	bool  CheckMaxSize(asUINT numElements);
	void  Resize(int delta, asUINT at);
	void  CreateBuffer(SArrayBuffer **buf, asUINT numElements);
	void  DeleteBuffer(SArrayBuffer *buf);
	void  CopyBuffer(SArrayBuffer *dst, SArrayBuffer *src);
	void  Construct(SArrayBuffer *buf, asUINT start, asUINT end);
	void  Destruct(SArrayBuffer *buf, asUINT start, asUINT end);
	bool  Equals(const void *a, const void *b, asIScriptContext *ctx, SArrayCache *cache) const;
};

void RegisterScriptArray(asIScriptEngine *engine, bool defaultArray);

#endif // X2D_SCRIPT_ARRAY_H