//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/extention.h>

#include <new>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h> // sprintf

// Set the default memory routines
// Use the angelscript engine's memory routines by default
static asALLOCFUNC_t userAlloc = asAllocMem;
static asFREEFUNC_t  userFree  = asFreeMem;

// Allows the application to set which memory routines should be used by the array object
void XScriptGrid::SetMemoryFunctions(asALLOCFUNC_t allocFunc, asFREEFUNC_t freeFunc)
{
	userAlloc = allocFunc;
	userFree = freeFunc;
}

static void RegisterScriptGrid_Native(asIScriptEngine *engine);

struct SGridBuffer
{
	asDWORD width;
	asDWORD height;
	asBYTE  data[1];
};

XScriptGrid *XScriptGrid::Create(asIObjectType *ot)
{
	return XScriptGrid::Create(ot, 0, 0);
}

XScriptGrid *XScriptGrid::Create(asIObjectType *ot, asUINT w, asUINT h)
{
	asIScriptContext *ctx = asGetActiveContext();

	// Allocate the memory
	void *mem = userAlloc(sizeof(XScriptGrid));
	if( mem == 0 )
	{
		if( ctx )
			ctx->SetException("Out of memory");

		return 0;
	}

	// Initialize the object
	XScriptGrid *a = new(mem) XScriptGrid(w, h, ot);

	// It's possible the constructor raised a script exception, in which case we
	// need to free the memory and return null instead, else we get a memory leak.
	if( ctx && ctx->GetState() == asEXECUTION_EXCEPTION )
	{
		a->Release();
		return 0;
	}

	return a;
}

XScriptGrid *XScriptGrid::Create(asIObjectType *ot, void *initList)
{
	asIScriptContext *ctx = asGetActiveContext();

	// Allocate the memory
	void *mem = userAlloc(sizeof(XScriptGrid));
	if( mem == 0 )
	{
		if( ctx )
			ctx->SetException("Out of memory");

		return 0;
	}

	// Initialize the object
	XScriptGrid *a = new(mem) XScriptGrid(ot, initList);

	// It's possible the constructor raised a script exception, in which case we
	// need to free the memory and return null instead, else we get a memory leak.
	if( ctx && ctx->GetState() == asEXECUTION_EXCEPTION )
	{
		a->Release();
		return 0;
	}

	return a;
}

XScriptGrid *XScriptGrid::Create(asIObjectType *ot, asUINT w, asUINT h, void *defVal)
{
	asIScriptContext *ctx = asGetActiveContext();

	// Allocate the memory
	void *mem = userAlloc(sizeof(XScriptGrid));
	if( mem == 0 )
	{
		if( ctx )
			ctx->SetException("Out of memory");

		return 0;
	}

	// Initialize the object
	XScriptGrid *a = new(mem) XScriptGrid(w, h, defVal, ot);

	// It's possible the constructor raised a script exception, in which case we
	// need to free the memory and return null instead, else we get a memory leak.
	if( ctx && ctx->GetState() == asEXECUTION_EXCEPTION )
	{
		a->Release();
		return 0;
	}

	return a;
}

// This optional callback is called when the template type is first used by the compiler.
// It allows the application to validate if the template can be instanciated for the requested
// subtype at compile time, instead of at runtime. The output argument dontGarbageCollect
// allow the callback to tell the engine if the template instance type shouldn't be garbage collected,
// i.e. no asOBJ_GC flag.
static bool ScriptGridTemplateCallback(asIObjectType *ot, bool &dontGarbageCollect)
{
	// Make sure the subtype can be instanciated with a default factory/constructor,
	// otherwise we won't be able to instanciate the elements.
	int typeId = ot->GetSubTypeId();
	if( typeId == asTYPEID_VOID )
		return false;
	if( (typeId & asTYPEID_MASK_OBJECT) && !(typeId & asTYPEID_OBJHANDLE) )
	{
		asIObjectType *subtype = ot->GetEngine()->GetObjectTypeById(typeId);
		asDWORD flags = subtype->GetFlags();
		if( (flags & asOBJ_VALUE) && !(flags & asOBJ_POD) )
		{
			// Verify that there is a default constructor
			bool found = false;
			for( asUINT n = 0; n < subtype->GetBehaviourCount(); n++ )
			{
				asEBehaviours beh;
				asIScriptFunction *func = subtype->GetBehaviourByIndex(n, &beh);
				if( beh != asBEHAVE_CONSTRUCT ) continue;

				if( func->GetParamCount() == 0 )
				{
					// Found the default constructor
					found = true;
					break;
				}
			}

			if( !found )
			{
				// There is no default constructor
				ot->GetEngine()->WriteMessage("array", 0, 0, asMSGTYPE_ERROR, "The subtype has no default constructor");
				return false;
			}
		}
		else if( (flags & asOBJ_REF) )
		{
			bool found = false;

			// If value assignment for ref type has been disabled then the array
			// can be created if the type has a default factory function
			if( !ot->GetEngine()->GetEngineProperty(asEP_DISALLOW_VALUE_ASSIGN_FOR_REF_TYPE) )
			{
				// Verify that there is a default factory
				for( asUINT n = 0; n < subtype->GetFactoryCount(); n++ )
				{
					asIScriptFunction *func = subtype->GetFactoryByIndex(n);
					if( func->GetParamCount() == 0 )
					{
						// Found the default factory
						found = true;
						break;
					}
				}
			}

			if( !found )
			{
				// No default factory
				ot->GetEngine()->WriteMessage("array", 0, 0, asMSGTYPE_ERROR, "The subtype has no default factory");
				return false;
			}
		}

		// If the object type is not garbage collected then the array also doesn't need to be
		if( !(flags & asOBJ_GC) )
			dontGarbageCollect = true;
	}
	else if( !(typeId & asTYPEID_OBJHANDLE) )
	{
		// Arrays with primitives cannot form circular references,
		// thus there is no need to garbage collect them
		dontGarbageCollect = true;
	}

	// The type is ok
	return true;
}

// Registers the template array type
void RegisterScriptGrid(asIScriptEngine *engine)
{
	// TODO: Implement the generic calling convention
	RegisterScriptGrid_Native(engine);
}

static void RegisterScriptGrid_Native(asIScriptEngine *engine)
{
	int r;

	// Register the grid type as a template
	r = engine->RegisterObjectType("grid<class T>", 0, asOBJ_REF | asOBJ_GC | asOBJ_TEMPLATE); assert( r >= 0 );

	// Register a callback for validating the subtype before it is used
	r = engine->RegisterObjectBehaviour("grid<T>", asBEHAVE_TEMPLATE_CALLBACK, "bool f(int&in, bool&out)", asFUNCTION(ScriptGridTemplateCallback), asCALL_CDECL); assert( r >= 0 );

	// Templates receive the object type as the first parameter. To the script writer this is hidden
	r = engine->RegisterObjectBehaviour("grid<T>", asBEHAVE_FACTORY, "grid<T>@ f(int&in)", asFUNCTIONPR(XScriptGrid::Create, (asIObjectType*), XScriptGrid*), asCALL_CDECL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("grid<T>", asBEHAVE_FACTORY, "grid<T>@ f(int&in, uint, uint)", asFUNCTIONPR(XScriptGrid::Create, (asIObjectType*, asUINT, asUINT), XScriptGrid*), asCALL_CDECL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("grid<T>", asBEHAVE_FACTORY, "grid<T>@ f(int&in, uint, uint, const T &in)", asFUNCTIONPR(XScriptGrid::Create, (asIObjectType*, asUINT, asUINT, void *), XScriptGrid*), asCALL_CDECL); assert( r >= 0 );

	// Register the factory that will be used for initialization lists
	r = engine->RegisterObjectBehaviour("grid<T>", asBEHAVE_LIST_FACTORY, "grid<T>@ f(int&in type, int&in list) {repeat {repeat_same T}}", asFUNCTIONPR(XScriptGrid::Create, (asIObjectType*, void*), XScriptGrid*), asCALL_CDECL); assert( r >= 0 );

	// The memory management methods
	r = engine->RegisterObjectBehaviour("grid<T>", asBEHAVE_ADDREF, "void f()", asMETHOD(XScriptGrid,AddRef), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("grid<T>", asBEHAVE_RELEASE, "void f()", asMETHOD(XScriptGrid,Release), asCALL_THISCALL); assert( r >= 0 );

	// The index operator returns the template subtype
	r = engine->RegisterObjectMethod("grid<T>", "T &opIndex(uint, uint)", asMETHODPR(XScriptGrid, At, (asUINT, asUINT), void*), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("grid<T>", "const T &opIndex(uint, uint) const", asMETHODPR(XScriptGrid, At, (asUINT, asUINT) const, const void*), asCALL_THISCALL); assert( r >= 0 );
	
	// The assignment operator
	r = engine->RegisterObjectMethod("grid<T>", "grid<T> &opAssign(const grid<T>&in)", asMETHOD(XScriptGrid, operator=), asCALL_THISCALL); assert( r >= 0 );

	// Other methods
	r = engine->RegisterObjectMethod("grid<T>", "void resize(uint width, uint height)", asMETHODPR(XScriptGrid, Resize, (asUINT, asUINT), void), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("grid<T>", "uint width() const", asMETHOD(XScriptGrid, GetWidth), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("grid<T>", "uint height() const", asMETHOD(XScriptGrid, GetHeight), asCALL_THISCALL); assert( r >= 0 );

	// Register GC behaviours in case the array needs to be garbage collected
	r = engine->RegisterObjectBehaviour("grid<T>", asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(XScriptGrid, GetRefCount), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("grid<T>", asBEHAVE_SETGCFLAG, "void f()", asMETHOD(XScriptGrid, SetFlag), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("grid<T>", asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(XScriptGrid, GetFlag), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("grid<T>", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(XScriptGrid, EnumReferences), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("grid<T>", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(XScriptGrid, ReleaseAllHandles), asCALL_THISCALL); assert( r >= 0 );
}

XScriptGrid::XScriptGrid(asIObjectType *ot, void *buf)
{
	refCount = 1;
	gcFlag = false;
	objType = ot;
	objType->AddRef();
	buffer = 0;
	subTypeId = objType->GetSubTypeId();

	asIScriptEngine *engine = ot->GetEngine();

	// Determine element size
	if( subTypeId & asTYPEID_MASK_OBJECT )
		elementSize = sizeof(asPWORD);
	else
		elementSize = engine->GetSizeOfPrimitiveType(subTypeId);

	// Determine the initial size from the buffer
	asUINT height = *(asUINT*)buf;
	asUINT width = *(asUINT*)((char*)(buf)+4);

	// Make sure the grid size isn't too large for us to handle
	if( !CheckMaxSize(width, height) )
	{
		// Don't continue with the initialization
		return;
	}

	// Skip the height value at the start of the buffer
	buf = (asUINT*)(buf)+1;

	// Copy the values of the grid elements from the buffer
	if( (ot->GetSubTypeId() & asTYPEID_MASK_OBJECT) == 0 )
	{
		CreateBuffer(&buffer, width, height);

		// Copy the values of the primitive type into the internal buffer
		for( asUINT y = 0; y < height; y++ )
		{
			// Skip the length value at the start of each row
			buf = (asUINT*)(buf)+1;

			// Copy the line
			memcpy(At(0,y), buf, width*elementSize);

			// Move to next line
			buf = (char*)(buf) + width*elementSize;

			// Align to 4 byte boundary
			if( asPWORD(buf) & 0x3 )
				buf = (char*)(buf) + 4 - (asPWORD(buf) & 0x3);
		}
	}
	else if( ot->GetSubTypeId() & asTYPEID_OBJHANDLE )
	{
		CreateBuffer(&buffer, width, height);

		// Copy the handles into the internal buffer
		for( asUINT y = 0; y < height; y++ )
		{
			// Skip the length value at the start of each row
			buf = (asUINT*)(buf)+1;

			// Copy the line
			memcpy(At(0,y), buf, width*elementSize);

			// With object handles it is safe to clear the memory in the received buffer
			// instead of increasing the ref count. It will save time both by avoiding the
			// call the increase ref, and also relieve the engine from having to release
			// its references too
			memset(buf, 0, width*elementSize);

			// Move to next line
			buf = (char*)(buf) + width*elementSize;

			// Align to 4 byte boundary
			if( asPWORD(buf) & 0x3 )
				buf = (char*)(buf) + 4 - (asPWORD(buf) & 0x3);
		}
	}
	else if( ot->GetSubType()->GetFlags() & asOBJ_REF )
	{
		// Only allocate the buffer, but not the objects
		subTypeId |= asTYPEID_OBJHANDLE;
		CreateBuffer(&buffer, width, height);
		subTypeId &= ~asTYPEID_OBJHANDLE;

		// Copy the handles into the internal buffer
		for( asUINT y = 0; y < height; y++ )
		{
			// Skip the length value at the start of each row
			buf = (asUINT*)(buf)+1;

			// Copy the line
			memcpy(At(0,y), buf, width*elementSize);

			// With object handles it is safe to clear the memory in the received buffer
			// instead of increasing the ref count. It will save time both by avoiding the
			// call the increase ref, and also relieve the engine from having to release
			// its references too
			memset(buf, 0, width*elementSize);

			// Move to next line
			buf = (char*)(buf) + width*elementSize;

			// Align to 4 byte boundary
			if( asPWORD(buf) & 0x3 )
				buf = (char*)(buf) + 4 - (asPWORD(buf) & 0x3);
		}
	}
	else
	{
		// TODO: Optimize by calling the copy constructor of the object instead of
		//       constructing with the default constructor and then assigning the value
		// TODO: With C++11 ideally we should be calling the move constructor, instead
		//       of the copy constructor as the engine will just discard the objects in the
		//       buffer afterwards.
		CreateBuffer(&buffer, width, height);

		// For value types we need to call the opAssign for each individual object
		asIObjectType *subType = ot->GetSubType();
		asUINT subTypeSize = subType->GetSize();
		for( asUINT y = 0;y < height; y++ )
		{
			// Skip the length value at the start of each row
			buf = (asUINT*)(buf)+1;

			// Call opAssign for each of the objects on the row
			for( asUINT x = 0; x < width; x++ )
			{
				void *obj = At(x,y);
				asBYTE *srcObj = (asBYTE*)(buf) + x*subTypeSize;
				engine->AssignScriptObject(obj, srcObj, subType);
			}

			// Move to next line
			buf = (char*)(buf) + width*subTypeSize;

			// Align to 4 byte boundary
			if( asPWORD(buf) & 0x3 )
				buf = (char*)(buf) + 4 - (asPWORD(buf) & 0x3);
		}
	}

	// Notify the GC of the successful creation
	if( objType->GetFlags() & asOBJ_GC )
		objType->GetEngine()->NotifyGarbageCollectorOfNewObject(this, objType);
}

XScriptGrid::XScriptGrid(asUINT width, asUINT height, asIObjectType *ot)
{
	refCount = 1;
	gcFlag = false;
	objType = ot;
	objType->AddRef();
	buffer = 0;
	subTypeId = objType->GetSubTypeId();

	// Determine element size
	if( subTypeId & asTYPEID_MASK_OBJECT )
		elementSize = sizeof(asPWORD);
	else
		elementSize = objType->GetEngine()->GetSizeOfPrimitiveType(subTypeId);

	// Make sure the array size isn't too large for us to handle
	if( !CheckMaxSize(width, height) )
	{
		// Don't continue with the initialization
		return;
	}

	CreateBuffer(&buffer, width, height);

	// Notify the GC of the successful creation
	if( objType->GetFlags() & asOBJ_GC )
		objType->GetEngine()->NotifyGarbageCollectorOfNewObject(this, objType);
}

void XScriptGrid::Resize(asUINT width, asUINT height)
{
	// Make sure the size isn't too large for us to handle
	if( !CheckMaxSize(width, height) )
		return;

	// Create a new buffer
	SGridBuffer *tmpBuffer = 0;
	CreateBuffer(&tmpBuffer, width, height);
	if( tmpBuffer == 0 )
		return;

	if( buffer )
	{
		// Copy the existing values to the new buffer
		asUINT w = width > buffer->width ? buffer->width : width;
		asUINT h = height > buffer->height ? buffer->height : height;
		for( asUINT y = 0; y < h; y++ )
			for( asUINT x = 0; x < w; x++ )
				SetValue(tmpBuffer, x, y, At(buffer, x, y));

		// Replace the internal buffer
		DeleteBuffer(buffer);
	}

	buffer = tmpBuffer;
}

XScriptGrid::XScriptGrid(asUINT width, asUINT height, void *defVal, asIObjectType *ot)
{
	refCount = 1;
	gcFlag = false;
	objType = ot;
	objType->AddRef();
	buffer = 0;
	subTypeId = objType->GetSubTypeId();

	// Determine element size
	if( subTypeId & asTYPEID_MASK_OBJECT )
		elementSize = sizeof(asPWORD);
	else
		elementSize = objType->GetEngine()->GetSizeOfPrimitiveType(subTypeId);

	// Make sure the array size isn't too large for us to handle
	if( !CheckMaxSize(width, height) )
	{
		// Don't continue with the initialization
		return;
	}

	CreateBuffer(&buffer, width, height);

	// Notify the GC of the successful creation
	if( objType->GetFlags() & asOBJ_GC )
		objType->GetEngine()->NotifyGarbageCollectorOfNewObject(this, objType);

	// Initialize the elements with the default value
	for( asUINT y = 0; y < GetHeight(); y++ )
		for( asUINT x = 0; x < GetWidth(); x++ )
			SetValue(x, y, defVal);
}

void XScriptGrid::SetValue(asUINT x, asUINT y, void *value)
{
	SetValue(buffer, x, y, value);
}

void XScriptGrid::SetValue(SGridBuffer *buf, asUINT x, asUINT y, void *value)
{
	// At() will take care of the out-of-bounds checking, though
	// if called from the application then nothing will be done
	void *ptr = At(buf, x, y);
	if( ptr == 0 ) return;

	if( (subTypeId & ~asTYPEID_MASK_SEQNBR) && !(subTypeId & asTYPEID_OBJHANDLE) )
		objType->GetEngine()->AssignScriptObject(ptr, value, objType->GetSubType());
	else if( subTypeId & asTYPEID_OBJHANDLE )
	{
		void *tmp = *(void**)ptr;
		*(void**)ptr = *(void**)value;
		objType->GetEngine()->AddRefScriptObject(*(void**)value, objType->GetSubType());
		if( tmp )
			objType->GetEngine()->ReleaseScriptObject(tmp, objType->GetSubType());
	}
	else if( subTypeId == asTYPEID_BOOL ||
			 subTypeId == asTYPEID_INT8 ||
			 subTypeId == asTYPEID_UINT8 )
		*(char*)ptr = *(char*)value;
	else if( subTypeId == asTYPEID_INT16 ||
			 subTypeId == asTYPEID_UINT16 )
		*(short*)ptr = *(short*)value;
	else if( subTypeId == asTYPEID_INT32 ||
			 subTypeId == asTYPEID_UINT32 ||
			 subTypeId == asTYPEID_FLOAT ||
			 subTypeId > asTYPEID_DOUBLE ) // enums have a type id larger than doubles
		*(int*)ptr = *(int*)value;
	else if( subTypeId == asTYPEID_INT64 ||
			 subTypeId == asTYPEID_UINT64 ||
			 subTypeId == asTYPEID_DOUBLE )
		*(double*)ptr = *(double*)value;
}

XScriptGrid &XScriptGrid::operator=(const XScriptGrid &other)
{
	// Only perform the copy if the array types are the same
	if( &other != this &&
		other.GetGridObjectType() == GetGridObjectType() )
	{
		// Make sure the arrays are of the same size
		Resize(other.buffer->width, other.buffer->height);

		// Copy the value of each element
		CopyBuffer(buffer, other.buffer);
	}

	return *this;
}

XScriptGrid::~XScriptGrid()
{
	if( buffer )
	{
		DeleteBuffer(buffer);
		buffer = 0;
	}
	if( objType ) objType->Release();
}

asUINT XScriptGrid::GetWidth() const
{
	if( buffer )
		return buffer->width;

	return 0;
}

asUINT XScriptGrid::GetHeight() const
{
	if( buffer )
		return buffer->height;

	return 0;
}

// internal
bool XScriptGrid::CheckMaxSize(asUINT width, asUINT height)
{
	// This code makes sure the size of the buffer that is allocated
	// for the array doesn't overflow and becomes smaller than requested

	asUINT maxSize = 0xFFFFFFFFul - sizeof(SGridBuffer) + 1;
	if( elementSize > 0 )
		maxSize /= elementSize;

	asINT64 numElements  = width * height;

	if( (numElements >> 32) || numElements > maxSize )
	{
		asIScriptContext *ctx = asGetActiveContext();
		if( ctx )
			ctx->SetException("Too large grid size");

		return false;
	}

	// OK
	return true;
}

asIObjectType *XScriptGrid::GetGridObjectType() const
{
	return objType;
}

int XScriptGrid::GetGridTypeId() const
{
	return objType->GetTypeId();
}

int XScriptGrid::GetElementTypeId() const
{
	return subTypeId;
}

void *XScriptGrid::At(asUINT x, asUINT y)
{
	return At(buffer, x, y);
}

// Return a pointer to the array element. Returns 0 if the index is out of bounds
void *XScriptGrid::At(SGridBuffer *buf, asUINT x, asUINT y)
{
	if( buf == 0 || x >= buf->width || y >= buf->height )
	{
		// If this is called from a script we raise a script exception
		asIScriptContext *ctx = asGetActiveContext();
		if( ctx )
			ctx->SetException("Index out of bounds");
		return 0;
	}

	asUINT index = x+y*buf->width;
	if( (subTypeId & asTYPEID_MASK_OBJECT) && !(subTypeId & asTYPEID_OBJHANDLE) )
		return *(void**)(buf->data + elementSize*index);
	else
		return buf->data + elementSize*index;
}
const void *XScriptGrid::At(asUINT x, asUINT y) const
{
	return const_cast<XScriptGrid*>(this)->At(const_cast<SGridBuffer*>(buffer), x, y);
}


// internal
void XScriptGrid::CreateBuffer(SGridBuffer **buf, asUINT w, asUINT h)
{
	asUINT numElements = w * h;

	*buf = reinterpret_cast<SGridBuffer*>(userAlloc(sizeof(SGridBuffer)-1+elementSize*numElements));

	if( *buf )
	{
		(*buf)->width  = w;
		(*buf)->height = h;
		Construct(*buf);
	}
	else
	{
		// Oops, out of memory
		asIScriptContext *ctx = asGetActiveContext();
		if( ctx )
			ctx->SetException("Out of memory");
	}
}

// internal
void XScriptGrid::DeleteBuffer(SGridBuffer *buf)
{
	assert( buf );

	Destruct(buf);

	// Free the buffer
	userFree(buf);
}

// internal
void XScriptGrid::CopyBuffer(SGridBuffer *dst, SGridBuffer *src)
{
	asIScriptEngine *engine = objType->GetEngine();
	int dstSize = dst->width*dst->height, srcSize = src->width*src->height;
	if( subTypeId & asTYPEID_OBJHANDLE )
	{
		// Copy the references and increase the reference counters
		if( dstSize > 0 && srcSize > 0 )
		{
			int count = dstSize > srcSize ? srcSize : dstSize;

			void **max = (void**)(dst->data + count * sizeof(void*));
			void **d   = (void**)dst->data;
			void **s   = (void**)src->data;

			for( ; d < max; d++, s++ )
			{
				void *tmp = *d;
				*d = *s;
				if( *d )
					engine->AddRefScriptObject(*d, objType->GetSubType());
				// Release the old ref after incrementing the new to avoid problem incase it is the same ref
				if( tmp )
					engine->ReleaseScriptObject(tmp, objType->GetSubType());
			}
		}
	}
	else
	{
		if( dstSize > 0 && srcSize > 0 )
		{
			int count = dstSize > srcSize ? srcSize : dstSize;
			if( subTypeId & asTYPEID_MASK_OBJECT )
			{
				// Call the assignment operator on all of the objects
				void **max = (void**)(dst->data + count * sizeof(void*));
				void **d   = (void**)dst->data;
				void **s   = (void**)src->data;

				asIObjectType *subType = objType->GetSubType();
				for( ; d < max; d++, s++ )
					engine->AssignScriptObject(*d, *s, subType);
			}
			else
			{
				// Primitives are copied byte for byte
				memcpy(dst->data, src->data, count*elementSize);
			}
		}
	}
}

// internal
void XScriptGrid::Construct(SGridBuffer *buf)
{
	assert( buf );

	if( subTypeId & asTYPEID_OBJHANDLE )
	{
		// Set all object handles to null
		void *d = (void*)(buf->data);
		memset(d, 0, (buf->width*buf->height)*sizeof(void*));
	}
	else if( subTypeId & asTYPEID_MASK_OBJECT )
	{
		void **max = (void**)(buf->data + (buf->width*buf->height) * sizeof(void*));
		void **d = (void**)(buf->data);

		asIScriptEngine *engine = objType->GetEngine();
		asIObjectType *subType = objType->GetSubType();

		for( ; d < max; d++ )
		{
			*d = (void*)engine->CreateScriptObject(subType);
			if( *d == 0 )
			{
				// Set the remaining entries to null so the destructor 
				// won't attempt to destroy invalid objects later
				memset(d, 0, sizeof(void*)*(max-d));

				// There is no need to set an exception on the context,
				// as CreateScriptObject has already done that
				return;
			}
		}
	}
}

// internal
void XScriptGrid::Destruct(SGridBuffer *buf)
{
	assert( buf );

	if( subTypeId & asTYPEID_MASK_OBJECT )
	{
		asIScriptEngine *engine = objType->GetEngine();

		void **max = (void**)(buf->data + (buf->width*buf->height) * sizeof(void*));
		void **d   = (void**)(buf->data);

		for( ; d < max; d++ )
		{
			if( *d )
				engine->ReleaseScriptObject(*d, objType->GetSubType());
		}
	}
}

// GC behaviour
void XScriptGrid::EnumReferences(asIScriptEngine *engine)
{
	if( buffer == 0 ) return;

	// If the array is holding handles, then we need to notify the GC of them
	if( subTypeId & asTYPEID_MASK_OBJECT )
	{
		asUINT numElements = buffer->width * buffer->height;
		void **d = (void**)buffer->data;
		for( asUINT n = 0; n < numElements; n++ )
		{
			if( d[n] )
				engine->GCEnumCallback(d[n]);
		}
	}
}

// GC behaviour
void XScriptGrid::ReleaseAllHandles(asIScriptEngine*)
{
	if( buffer == 0 ) return;

	DeleteBuffer(buffer);
	buffer = 0;
}

void XScriptGrid::AddRef() const
{
	// Clear the GC flag then increase the counter
	gcFlag = false;
	asAtomicInc(refCount);
}

void XScriptGrid::Release() const
{
	// Clearing the GC flag then descrease the counter
	gcFlag = false;
	if( asAtomicDec(refCount) == 0 )
	{
		// When reaching 0 no more references to this instance
		// exists and the object should be destroyed
		this->~XScriptGrid();
		userFree(const_cast<XScriptGrid*>(this));
	}
}

// GC behaviour
int XScriptGrid::GetRefCount()
{
	return refCount;
}

// GC behaviour
void XScriptGrid::SetFlag()
{
	gcFlag = true;
}

// GC behaviour
bool XScriptGrid::GetFlag()
{
	return gcFlag;
}