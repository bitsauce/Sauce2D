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
#include <assert.h>
#include <string.h>

// We'll use the generic interface for the factories as we need the engine pointer
static void ScriptAnyFactory_Generic(asIScriptGeneric *gen)
{
	asIScriptEngine *engine = gen->GetEngine();

	*(XScriptAny**)gen->GetAddressOfReturnLocation() = new XScriptAny(engine);
}

static void ScriptAnyFactory2_Generic(asIScriptGeneric *gen)
{
	asIScriptEngine *engine = gen->GetEngine();
	void *ref = (void*)gen->GetArgAddress(0);
	int refType = gen->GetArgTypeId(0);

	*(XScriptAny**)gen->GetAddressOfReturnLocation() = new XScriptAny(ref,refType,engine);
}

static XScriptAny &ScriptAnyAssignment(XScriptAny *other, XScriptAny *self)
{
	return *self = *other;
}

void RegisterScriptAny(asIScriptEngine *engine)
{
	int r;
	r = engine->RegisterObjectType("any", sizeof(XScriptAny), asOBJ_REF | asOBJ_GC); assert( r >= 0 );

	// We'll use the generic interface for the constructor as we need the engine pointer
	r = engine->RegisterObjectBehaviour("any", asBEHAVE_FACTORY, "any@ f()", asFUNCTION(ScriptAnyFactory_Generic), asCALL_GENERIC); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("any", asBEHAVE_FACTORY, "any@ f(?&in)", asFUNCTION(ScriptAnyFactory2_Generic), asCALL_GENERIC); assert( r >= 0 );

	r = engine->RegisterObjectBehaviour("any", asBEHAVE_ADDREF, "void f()", asMETHOD(XScriptAny,AddRef), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("any", asBEHAVE_RELEASE, "void f()", asMETHOD(XScriptAny,Release), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("any", "any &opAssign(any&in)", asFUNCTION(ScriptAnyAssignment), asCALL_CDECL_OBJLAST); assert( r >= 0 );
	r = engine->RegisterObjectMethod("any", "void store(?&in)", asMETHODPR(XScriptAny,Store,(void*,int),void), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("any", "void store(int64&in)", asMETHODPR(XScriptAny,Store,(asINT64&),void), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("any", "void store(double&in)", asMETHODPR(XScriptAny,Store,(double&),void), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("any", "bool retrieve(?&out)", asMETHODPR(XScriptAny,Retrieve,(void*,int) const,bool), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("any", "bool retrieve(int64&out)", asMETHODPR(XScriptAny,Retrieve,(asINT64&) const,bool), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectMethod("any", "bool retrieve(double&out)", asMETHODPR(XScriptAny,Retrieve,(double&) const,bool), asCALL_THISCALL); assert( r >= 0 );

	// Register GC behaviours
	r = engine->RegisterObjectBehaviour("any", asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(XScriptAny,GetRefCount), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("any", asBEHAVE_SETGCFLAG, "void f()", asMETHOD(XScriptAny,SetFlag), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("any", asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(XScriptAny,GetFlag), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("any", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(XScriptAny,EnumReferences), asCALL_THISCALL); assert( r >= 0 );
	r = engine->RegisterObjectBehaviour("any", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(XScriptAny,ReleaseAllHandles), asCALL_THISCALL); assert( r >= 0 );
}

XScriptAny &XScriptAny::operator=(const XScriptAny &other)
{
	// Hold on to the object type reference so it isn't destroyed too early
	if( other.value.valueObj && (other.value.typeId & asTYPEID_MASK_OBJECT) )
	{
		asIObjectType *ot = engine->GetObjectTypeById(other.value.typeId);
		if( ot )
			ot->AddRef();
	}

	FreeObject();

	value.typeId = other.value.typeId;
	if( value.typeId & asTYPEID_OBJHANDLE )
	{
		// For handles, copy the pointer and increment the reference count
		value.valueObj = other.value.valueObj;
		engine->AddRefScriptObject(value.valueObj, engine->GetObjectTypeById(value.typeId));
	}
	else if( value.typeId & asTYPEID_MASK_OBJECT )
	{
		// Create a copy of the object
		value.valueObj = engine->CreateScriptObjectCopy(other.value.valueObj, engine->GetObjectTypeById(value.typeId));
	}
	else
	{
		// Primitives can be copied directly
		value.valueInt = other.value.valueInt;
	}

	return *this;
}

int XScriptAny::CopyFrom(const XScriptAny *other)
{
	if( other == 0 ) return asINVALID_ARG;

	*this = *(XScriptAny*)other;

	return 0;
}

XScriptAny::XScriptAny(asIScriptEngine *engine)
{
	this->engine = engine;
	refCount = 1;
	gcFlag = false;

	value.typeId = 0;
	value.valueInt = 0;

	// Notify the garbage collector of this object
	engine->NotifyGarbageCollectorOfNewObject(this, engine->GetObjectTypeByName("any"));
}

XScriptAny::XScriptAny(void *ref, int refTypeId, asIScriptEngine *engine)
{
	this->engine = engine;
	refCount = 1;
	gcFlag = false;

	value.typeId = 0;
	value.valueInt = 0;

	// Notify the garbage collector of this object
	engine->NotifyGarbageCollectorOfNewObject(this, engine->GetObjectTypeByName("any"));

	Store(ref, refTypeId);
}

XScriptAny::~XScriptAny()
{
	FreeObject();
}

void XScriptAny::Store(void *ref, int refTypeId)
{
	// Hold on to the object type reference so it isn't destroyed too early
	if( *(void**)ref && (refTypeId & asTYPEID_MASK_OBJECT) )
	{
		asIObjectType *ot = engine->GetObjectTypeById(refTypeId);
		if( ot )
			ot->AddRef();
	}

	FreeObject();

	value.typeId = refTypeId;
	if( value.typeId & asTYPEID_OBJHANDLE )
	{
		// We're receiving a reference to the handle, so we need to dereference it
		value.valueObj = *(void**)ref;
		engine->AddRefScriptObject(value.valueObj, engine->GetObjectTypeById(value.typeId));
	}
	else if( value.typeId & asTYPEID_MASK_OBJECT )
	{
		// Create a copy of the object
		value.valueObj = engine->CreateScriptObjectCopy(ref, engine->GetObjectTypeById(value.typeId));
	}
	else
	{
		// Primitives can be copied directly
		value.valueInt = 0;

		// Copy the primitive value
		// We receive a pointer to the value.
		int size = engine->GetSizeOfPrimitiveType(value.typeId);
		memcpy(&value.valueInt, ref, size);
	}
}

void XScriptAny::Store(double &ref)
{
	Store(&ref, asTYPEID_DOUBLE);
}

void XScriptAny::Store(asINT64 &ref)
{
	Store(&ref, asTYPEID_INT64);
}


bool XScriptAny::Retrieve(void *ref, int refTypeId) const
{
	if( refTypeId & asTYPEID_OBJHANDLE )
	{
		// Is the handle type compatible with the stored value?

		// A handle can be retrieved if the stored type is a handle of same or compatible type
		// or if the stored type is an object that implements the interface that the handle refer to.
		if( (value.typeId & asTYPEID_MASK_OBJECT) && 
			engine->IsHandleCompatibleWithObject(value.valueObj, value.typeId, refTypeId) )
		{
			engine->AddRefScriptObject(value.valueObj, engine->GetObjectTypeById(value.typeId));
			*(void**)ref = value.valueObj;

			return true;
		}
	}
	else if( refTypeId & asTYPEID_MASK_OBJECT )
	{
		// Is the object type compatible with the stored value?

		// Copy the object into the given reference
		if( value.typeId == refTypeId )
		{
			engine->AssignScriptObject(ref, value.valueObj, engine->GetObjectTypeById(value.typeId));

			return true;
		}
	}
	else
	{
		// Is the primitive type compatible with the stored value?

		if( value.typeId == refTypeId )
		{
			int size = engine->GetSizeOfPrimitiveType(refTypeId);
			memcpy(ref, &value.valueInt, size);
			return true;
		}

		// We know all numbers are stored as either int64 or double, since we register overloaded functions for those
		if( value.typeId == asTYPEID_INT64 && refTypeId == asTYPEID_DOUBLE )
		{
			*(double*)ref = double(value.valueInt);
			return true;
		}
		else if( value.typeId == asTYPEID_DOUBLE && refTypeId == asTYPEID_INT64 )
		{
			*(asINT64*)ref = asINT64(value.valueFlt);
			return true;
		}
	}

	return false;
}

bool XScriptAny::Retrieve(asINT64 &value) const
{
	return Retrieve(&value, asTYPEID_INT64);
}

bool XScriptAny::Retrieve(double &value) const
{
	return Retrieve(&value, asTYPEID_DOUBLE);
}

int XScriptAny::GetTypeId() const
{
	return value.typeId;
}

void XScriptAny::FreeObject()
{
	// If it is a handle or a ref counted object, call release
	if( value.typeId & asTYPEID_MASK_OBJECT )
	{
		// Let the engine release the object
		asIObjectType *ot = engine->GetObjectTypeById(value.typeId);
		engine->ReleaseScriptObject(value.valueObj, ot);

		// Release the object type info
		if( ot )
			ot->Release();

		value.valueObj = 0;
		value.typeId = 0;
	}

	// For primitives, there's nothing to do
}


void XScriptAny::EnumReferences(asIScriptEngine *engine)
{
	// If we're holding a reference, we'll notify the garbage collector of it
	if( value.valueObj && (value.typeId & asTYPEID_MASK_OBJECT) )
	{
		engine->GCEnumCallback(value.valueObj);

		// The object type itself is also garbage collected
		asIObjectType *ot = engine->GetObjectTypeById(value.typeId);
		if( ot )
			engine->GCEnumCallback(ot);
	}
}

void XScriptAny::ReleaseAllHandles(asIScriptEngine * /*engine*/)
{
	FreeObject();
}

int XScriptAny::AddRef() const
{
	// Increase counter and clear flag set by GC
	gcFlag = false;
	return asAtomicInc(refCount);
}

int XScriptAny::Release() const
{
	// Decrease the ref counter
	gcFlag = false;
	if( asAtomicDec(refCount) == 0 )
	{
		// Delete this object as no more references to it exists
		delete this;
		return 0;
	}

	return refCount;
}

int XScriptAny::GetRefCount()
{
	return refCount;
}

void XScriptAny::SetFlag()
{
	gcFlag = true;
}

bool XScriptAny::GetFlag()
{
	return gcFlag;
}