//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)

#include "scriptgrid.h"
#include "scriptarray.h"

struct SArrayBuffer
{
	asDWORD size;
	asDWORD capacity;
	asBYTE  data[1];
};

//-----------------------------------------------------------------
// GridRow // TODO: implement
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Grid
//-----------------------------------------------------------------

void RegisterGrid()
{
	int r;
	
	// Register the template
	AS_REGISTER_TEMPLATE("grid");
	AS_TEMPLATE_BEHAVIOURM(Grid, "grid", "void f()", asBEHAVE_ADDREF, addRef)
	AS_TEMPLATE_BEHAVIOURM(Grid, "grid", "void f()", asBEHAVE_RELEASE, releaseRef)
	AS_TEMPLATE_BEHAVIOURM(Grid, "grid", "int  f()", asBEHAVE_GETREFCOUNT, refCount)
	AS_TEMPLATE_BEHAVIOURM(Grid, "grid", "void f()", asBEHAVE_SETGCFLAG, garbageCollect)
	AS_TEMPLATE_BEHAVIOURM(Grid, "grid", "bool f()", asBEHAVE_GETGCFLAG, gcFlag)
	
	// Register the behaviours
	AS_TEMPLATE_BEHAVIOURF("grid", "bool f(int&in, bool&out)", asBEHAVE_TEMPLATE_CALLBACK, Grid::TemplateCallback)
	AS_TEMPLATE_BEHAVIOURF("grid", "grid<T>@ f(int&in)", asBEHAVE_FACTORY, Grid::GridFactory1)
	AS_TEMPLATE_BEHAVIOURF("grid", "grid<T>@ f(int&in, uint, uint)", asBEHAVE_FACTORY, Grid::GridFactory2)
	AS_TEMPLATE_BEHAVIOURF("grid", "grid<T>@ f(int&in, uint, uint, const T &in)", asBEHAVE_FACTORY, Grid::GridFactory3)
	AS_TEMPLATE_BEHAVIOURM(Grid, "grid", "void f(int&in)", asBEHAVE_ENUMREFS, enumReferences)
	AS_TEMPLATE_BEHAVIOURM(Grid, "grid", "void f(int&in)", asBEHAVE_RELEASEREFS, releaseAllHandles)

	// Register the methods
	//
	// Size
	AS_TEMPLATE_METHODPR(Grid, "grid", "void resize(uint, uint)", resize, (uint, uint), void)
	AS_TEMPLATE_METHODPR(Grid, "grid", "void resize(uint, uint, const T&in)", resize, (uint, uint, void*), void)
	AS_TEMPLATE_METHOD(Grid, "grid", "uint insertCol(const uint at, const T&in)", insertCol)
	AS_TEMPLATE_METHOD(Grid, "grid", "uint removeCol(const uint at)", removeCol)
	AS_TEMPLATE_METHOD(Grid, "grid", "uint insertRow(const uint at, const T&in)", insertRow)
	AS_TEMPLATE_METHOD(Grid, "grid", "uint removeRow(const uint at)", removeRow)
	AS_TEMPLATE_METHOD(Grid, "grid", "uint width() const", width)
	AS_TEMPLATE_METHOD(Grid, "grid", "uint height() const", height)
	AS_TEMPLATE_METHOD(Grid, "grid", "void clear()", clear)
	AS_TEMPLATE_METHOD(Grid, "grid", "bool isIndex(uint, uint) const", isIndex)

	// Accessing
	// setValue, getValue, isValue, setAll, setRect, setCircle
	AS_TEMPLATE_METHODPR(Grid, "grid", "T &at(uint, uint)", at, (uint, uint), void*)
	AS_TEMPLATE_METHODPR(Grid, "grid", "const T &at(uint, uint) const", at, (uint, uint) const, const void*)

	// Operators
	// NOTE: opIndex cannot work unless:
	//		 Support for n-dimention arrays can be registered
	//		 Support for n-numbers of arguments to the opIndex
	//		 Array objects can be created from by application
	//AS_TEMPLATE_METHODPR(Grid, "GridArray<T> &opIndex(uint)", atArray, (uint), void*)
	//AS_TEMPLATE_METHODPR(Grid, "const GridArray<T> &opIndex(uint) const", atArray, (uint) const, const void*)
	AS_TEMPLATE_METHOD(Grid, "grid", "grid<T> &opAssign(const grid<T>&in)", operator=)
	//AS_TEMPLATE_METHOD(Grid, "grid", "bool opEquals(const grid<T>&in) const", operator==)
}

Grid::Grid(ScriptType *type, uint width, uint height, void *val) :
	m_refCount(1),
	m_gcFlag(false),
	arrayType(type),
	subTypeId(type->GetSubTypeId()),
	elementSize(0),
	gridWidth(width),
	gridHeight(height)
{
	// Add reference to array object type
	arrayType->AddRef();

	// Determine element size
	if(subTypeId & asTYPEID_MASK_OBJECT)
		elementSize = sizeof(void*);
	else
		elementSize = scriptEngine->GetSizeOfPrimitiveType(subTypeId);
	
	// Make sure the array size isn't too large
	if(!checkMaxSize(width*height))
		return;
	createBuffer(&buffer, width*height);

	// Notify the GC of the successful creation
	if(arrayType->GetFlags() & asOBJ_GC)
		scriptEngine->NotifyGarbageCollectorOfNewObject(this, arrayType);

	// Initialize the elements with the default value
	if(val)
	{
		for(uint y = 0; y < height; y++)
			for(uint x = 0; x < width; x++)
				setValue(x, y, val);
	}
}

Grid::~Grid()
{
	// Delete buffer
	if(buffer)
		deleteBuffer(buffer);

	// Release array type reference
	if(arrayType) 
		arrayType->Release();
}

//
//	Grid functions
//

const void *Grid::at(uint x, uint y) const
{
	if(x >= gridWidth || y >= gridHeight)
	{
		// If this is called from a script we raise a script exception
		ScriptContext *ctx = asGetActiveContext();
		if(ctx) ctx->SetException("Index out of bounds");
		return 0;
	}

	if((subTypeId & asTYPEID_MASK_OBJECT) && !(subTypeId & asTYPEID_OBJHANDLE))
		return (void*)((size_t*)buffer->data)[y*gridWidth+x];
	return buffer->data + elementSize*(y*gridWidth+x);
}

void *Grid::at(uint x, uint y)
{
	return const_cast<void*>(const_cast<const Grid*>(this)->at(x, y));
}

void Grid::resize(uint width, uint height)
{
	// Check if size is larger or equal to 2147483648
	if(width & 0x80000000 || height & 0x80000000)
	{
		checkMaxSize(width*height);
		return;
	}
	resize((int)width - (int)gridWidth, (int)height - (int)gridHeight, UINT_MAX);
}

void Grid::resize(uint width, uint height, void *value)
{
	// Check if size is larger or equal to 2147483648
	if(width & 0x80000000 || height & 0x80000000)
	{
		checkMaxSize(width*height);
		return;
	}

	// Clear grid
	resize(0, 0);

	// Resize to new size
	resize((int)width - (int)gridWidth, (int)height - (int)gridHeight, UINT_MAX);

	// Set the value of the new elements
	for(uint y = 0; y < gridHeight; y++)
		for(uint x = 0; x < gridWidth; x++)
			setValue(x, y, value);
}

void Grid::resize(int dw, int dh, uint at)
{
	if(!checkMaxSize((gridWidth+dw)*(gridHeight+dh)))
		return;

	// Calculate delta
	if(dw != 0)
	{
		if(dw < 0)
		{
			if(-dw > (int)gridWidth)
				dw = -(int)gridWidth;
			if(at > gridWidth + dw)
				at = gridWidth + dw;
		}
		else if(dw > 0)
		{
			// Make sure the array size isn't too large
			if(at > gridWidth)
				at = gridWidth;
		}

		// Width resize
		if(dw > 0)
		{
			uint size = (gridWidth+dw)*gridHeight;
			SArrayBuffer* newBuffer = (SArrayBuffer*)new uchar[sizeof(SArrayBuffer)-1 + elementSize*size];
			if(newBuffer)
			{
				// Set array info
				newBuffer->size = size;
				newBuffer->capacity = newBuffer->size;
			}
			else
			{
				// Out of memory
				ScriptContext *ctx = asGetActiveContext();
				if(ctx)	ctx->SetException("Out of memory");
				return;
			}

			// Copy old buffer into new
			for(uint i = 0; i < gridHeight; i++)
			{
				memcpy(newBuffer->data + ((gridWidth+dw)*i)*elementSize, buffer->data + (gridWidth*i)*elementSize, at*elementSize);
				if(at < gridWidth)
					memcpy(newBuffer->data + ((at+dw) + (gridWidth+dw)*i)*elementSize, buffer->data + (at + gridWidth*i)*elementSize, (gridWidth-at)*elementSize);
				
				if(subTypeId & asTYPEID_MASK_OBJECT)
					construct(newBuffer, at + (gridWidth+dw)*i, (at+dw) + (gridWidth+dw)*i);
			}

			// Release the old buffer
			delete[] (uchar*)buffer;

			buffer = newBuffer;
		}
		else if(dw < 0)
		{
			for(uint i = 0; i < gridHeight; i++)
			{
				destruct(buffer, at + (gridWidth+dw)*i, (at-dw) + (gridWidth+dw)*i);
				memmove(buffer->data + (at + (gridWidth+dw)*i)*elementSize, buffer->data + ((at-dw) + (gridWidth+dw)*i)*elementSize, ((gridWidth*gridHeight) - (at-dw) - gridWidth*i)*elementSize);
			}
			buffer->size += dw*gridHeight;
		}
		gridWidth += dw;
	}
	
	if(dh != 0)
	{
		// Calculate delta
		if(dh < 0)
		{
			if(-dh > (int)gridHeight)
				dh = -(int)gridHeight;
			if(at > gridHeight + dh)
				at = gridHeight + dh;
		}
		else if(dh > 0)
		{
			// Make sure the array size isn't too large
			if(at > gridHeight)
				at = gridHeight;
		}
	
		// Height resize
		if(dh > 0)
		{
			uint newSize = gridWidth*(gridHeight+dh);
			SArrayBuffer* newBuffer = (SArrayBuffer*)new uchar[sizeof(SArrayBuffer)-1 + elementSize*newSize];
			if(newBuffer)
			{
				// Set array info
				newBuffer->size = newSize;
				newBuffer->capacity = newBuffer->size;
			}
			else
			{
				// Out of memory
				ScriptContext *ctx = asGetActiveContext();
				if(ctx)	ctx->SetException("Out of memory");
				return;
			}

			// Copy old buffer into new
			memcpy(newBuffer->data, buffer->data, at*gridWidth*elementSize);
			if(at < gridHeight)
				memcpy(newBuffer->data + (at+dh)*gridWidth*elementSize, buffer->data + at*gridWidth*elementSize, (gridHeight-at)*gridWidth*elementSize);

			if(subTypeId & asTYPEID_MASK_OBJECT)
				construct(newBuffer, at*gridWidth, (at+dh)*gridWidth);

			// Release the old buffer
			delete[] (uchar*)buffer;

			buffer = newBuffer;
		}
		else if(dh < 0)
		{
			destruct(buffer, at*gridWidth, (at-dh)*gridWidth);
			memmove(buffer->data + at*gridWidth*elementSize, buffer->data + (at-dh)*gridWidth*elementSize, (buffer->size - ((at-dh)*gridWidth))*elementSize);
			buffer->size += dh*gridWidth;
		}
		gridHeight += dh;
	}
}

void Grid::insertCol(const uint col, void *value)
{
	if(col > gridWidth)
	{
		// If this is called from a script we raise a script exception
		ScriptContext *ctx = asGetActiveContext();
		if(ctx)
			ctx->SetException("Column out of bounds");
		return;
	}

	// Make room for the new elements
	resize(1, 0, col);

	// Set the value of the new elements
	for(uint i = 0; i < gridHeight; i++)
		setValue(col, i, value);
}

void Grid::removeCol(const uint col)
{
	if(col >= gridWidth)
	{
		// If this is called from a script we raise a script exception
		ScriptContext *ctx = asGetActiveContext();
		if(ctx)
			ctx->SetException("Column out of bounds");
		return;
	}

	// Remove the element
	resize(-1, 0, col);
}
	
void Grid::insertRow(const uint row, void *value)
{
	if(row > gridHeight)
	{
		// If this is called from a script we raise a script exception
		ScriptContext *ctx = asGetActiveContext();
		if(ctx)
			ctx->SetException("Row out of bounds");
		return;
	}

	// Make room for the new elements
	resize(0, 1, row);

	// Set the value of the new elements
	for(uint i = 0; i < gridWidth; i++)
		setValue(i, row, value);
}

void Grid::removeRow(const uint row)
{
	if(row >= gridHeight)
	{
		// If this is called from a script we raise a script exception
		ScriptContext *ctx = asGetActiveContext();
		if(ctx)
			ctx->SetException("Row out of bounds");
		return;
	}

	// Remove the element
	resize(0, -1, row);
}

/*bool Grid::equals(const void *a, const void *b, ScriptContext *ctx) const
{
	if(!(subTypeId & ~asTYPEID_MASK_SEQNBR))
	{
		// Simple compare of values
		switch(subTypeId)
		{
			#define COMPARE(T) *((T*)a) == *((T*)b)
			case asTYPEID_BOOL: return COMPARE(bool);
			case asTYPEID_INT8: return COMPARE(signed char);
			case asTYPEID_UINT8: return COMPARE(unsigned char);
			case asTYPEID_INT16: return COMPARE(signed short);
			case asTYPEID_UINT16: return COMPARE(unsigned short);
			case asTYPEID_INT32: return COMPARE(signed int);
			case asTYPEID_UINT32: return COMPARE(unsigned int);
			case asTYPEID_FLOAT: return COMPARE(float);
			case asTYPEID_DOUBLE: return COMPARE(double);
			default: return COMPARE(signed int); // All enums fall here
			#undef COMPARE
		}
	}
	else if(subTypeId & asTYPEID_OBJHANDLE)
	{
		return *((void**)a) == *((void**)b);
	}
	else
	{
		int r = 0;

		// Execute object opEquals if available
		if(eqFuncId >= 0)
		{
			// TODO: Add proper error handling
			r = ctx->Prepare(eqFuncId); assert(r >= 0);
			r = ctx->SetObject((void*)a); assert(r >= 0);
			r = ctx->SetArgObject(0, (void*)b); assert(r >= 0);
			r = ctx->Execute();

			if( r == asEXECUTION_FINISHED )
			{
				return ctx->GetReturnByte() != 0;
			}
		}

		// Execute object opCmp if available
		if(cmpFuncId >= 0)
		{
			// TODO: Add proper error handling
			r = ctx->Prepare(cmpFuncId); assert(r >= 0);
			r = ctx->SetObject((void*)a); assert(r >= 0);
			r = ctx->SetArgObject(0, (void*)b); assert(r >= 0);
			r = ctx->Execute();

			if( r == asEXECUTION_FINISHED )
			{
				return (int)ctx->GetReturnDWord() == 0;
			}
		}
	}

	return false;
}*/

void Grid::clear()
{
	resize(0, 0);
}

uint Grid::width() const
{
	return gridWidth;
}

uint Grid::height() const
{
	return gridHeight;
}

bool Grid::isIndex(uint x, uint y) const
{
	if(x >= gridWidth || y >= gridHeight)
		return false;
	return true;
}

//
//	Internal
//

void Grid::setValue(uint x, uint y, void *value)
{
	// Get pointer to value at index
	void *ptr = at(x, y);
	if(ptr == 0) return;

	// Check array type id
	if((subTypeId & ~asTYPEID_MASK_SEQNBR) && !(subTypeId & asTYPEID_OBJHANDLE))
	{
		// Copy script object
		scriptEngine->AssignScriptObject(ptr, value, arrayType->GetSubType());
	}else if(subTypeId & asTYPEID_OBJHANDLE)
	{
		// Set hadle value
		void *tmp = *(void**)ptr;
		*(void**)ptr = *(void**)value;

		// Add ref and release
		scriptEngine->AddRefScriptObject(*(void**)value, arrayType->GetSubType());
		if(tmp) scriptEngine->ReleaseScriptObject(*(void**)value, arrayType->GetSubType());
	}
	else if(subTypeId == asTYPEID_BOOL ||
			 subTypeId == asTYPEID_INT8 ||
			 subTypeId == asTYPEID_UINT8)
		*(char*)ptr = *(char*)value;
	else if(subTypeId == asTYPEID_INT16 ||
			 subTypeId == asTYPEID_UINT16)
		*(short*)ptr = *(short*)value;
	else if( subTypeId == asTYPEID_INT32 ||
			 subTypeId == asTYPEID_UINT32 ||
			 subTypeId == asTYPEID_FLOAT ||
			 subTypeId > asTYPEID_DOUBLE) // enums have a type id larger than doubles
		*(int*)ptr = *(int*)value;
	else if(subTypeId == asTYPEID_INT64 ||
			 subTypeId == asTYPEID_UINT64 ||
			 subTypeId == asTYPEID_DOUBLE)
		*(double*)ptr = *(double*)value;
}

bool Grid::checkMaxSize(uint size)
{
	// This code makes sure the size of the buffer that is allocated 
	// for the array doesn't overflow and becomes smaller than requested
	uint max = 0xFFFFFFFFul;
	if(size > max/elementSize)
	{
		ScriptContext *ctx = asGetActiveContext();
		if(ctx) ctx->SetException("Too large array size");
		return false;
	}

	// OK
	return true;
}

void Grid::createBuffer(SArrayBuffer **buf, uint size)
{
	// Allocate buffer space
	*buf = (SArrayBuffer*)new uchar[sizeof(SArrayBuffer)-1 + elementSize*size];
	if(*buf)
	{
		// Set array info
		(*buf)->size = size;
		(*buf)->capacity = size;
		construct(*buf, 0, size);
	}
	else
	{
		// Out of memory
		ScriptContext *ctx = asGetActiveContext();
		if(ctx) ctx->SetException("Out of memory");
	}
}

void Grid::deleteBuffer(SArrayBuffer *buf)
{
	destruct(buf, 0, buf->size);

	// Free the buffer
	delete[] (uchar*)buf;
}

void Grid::copyBuffer(SArrayBuffer *dst, SArrayBuffer *src)
{
	if(subTypeId & asTYPEID_OBJHANDLE)
	{
		// Copy the references and increase the reference counters
		if(dst->size > 0 && src->size > 0)
		{
			int count = dst->size > src->size ? src->size : dst->size;

			void **max = (void**)(dst->data + count * sizeof(void*));
			void **d   = (void**)dst->data;
			void **s   = (void**)src->data;
			
			for(d; d < max; d++, s++)
			{
				void *tmp = *d;
				*d = *s;
				if(*d) scriptEngine->AddRefScriptObject(*d, arrayType->GetSubType());

				// Release the old ref after incrementing the new to avoid problem incase it is the same ref
				if(tmp) scriptEngine->ReleaseScriptObject(tmp, arrayType->GetSubType());
			}
		}
	}
	else
	{
		if(dst->size > 0 && src->size > 0)
		{
			int count = dst->size > src->size ? src->size : dst->size;
			if(subTypeId & asTYPEID_MASK_OBJECT)
			{
				// Call the assignment operator on all of the objects
				void **max = (void**)(dst->data + count * elementSize);
				void **d   = (void**)dst->data;
				void **s   = (void**)src->data;

				for(d; d < max; d++, s++ )
					scriptEngine->AssignScriptObject(*d, *s, arrayType->GetSubType());
			}
			else
			{
				// Primitives are copied byte for byte
				memcpy(dst->data, src->data, count*elementSize);
			}
		}
	}
}

void Grid::construct(SArrayBuffer *buf, uint start, uint end)
{
	if(subTypeId & asTYPEID_OBJHANDLE)
	{
		// Set all object handles to null
		void *d = (void*)(buf->data + start * elementSize);
		memset(d, 0, (end-start) * elementSize);
	}
	else if(subTypeId & asTYPEID_MASK_OBJECT)
	{
		void **max = (void**)(buf->data + end * elementSize);
		void **d = (void**)(buf->data + start * elementSize);

		for(d; d < max; d++)
			*d = (void*)scriptEngine->CreateScriptObject(arrayType->GetSubType());
	}
}

void Grid::destruct(SArrayBuffer *buf, uint start, uint end)
{
	if(subTypeId & asTYPEID_MASK_OBJECT)
	{
		void **max = (void**)(buf->data + end * elementSize);
		void **d   = (void**)(buf->data + start * elementSize);

		for(d; d < max; d++)
		{
			// Sanity check
			if(!*d) continue;
			
			scriptEngine->ReleaseScriptObject(*d, arrayType->GetSubType());
			/*if(subTypeId & ~asTYPEID_OBJHANDLE && delFuncId >= 0)
			{
				// Call remove on SceneObjects
				int r;
				AS_OBJECT_CALL_ID(*d, delFuncId)
			}*/
		}
	}
}

//
//	Grid operators
//

Grid &Grid::operator=(const Grid &other)
{
	// Only perform the copy if the array types are the same
	if(&other != this && other.arrayType == arrayType)
	{
		// Make sure the arrays are of the same size
		resize(other.width(), other.height());

		// Copy the value of each element
		copyBuffer(buffer, other.buffer);
	}

	return *this;
}

/*bool Grid::operator==(const Grid &other) const
{
	if(arrayType != other.arrayType)
		return false;
	if(gridWidth != other.width() ||
		gridHeight != other.height())
		return false;
	
	ScriptContext *cmpContext = 0;
	if((subTypeId & ~asTYPEID_MASK_SEQNBR) && !(subTypeId & asTYPEID_OBJHANDLE))
	{
		// TODO: Ideally this context would be retrieved from a pool, so we don't have to 
		//       create a new one everytime. We could keep a context with the array object 
		//       but that would consume a lot of resources as each context is quite heavy.
		cmpContext = scriptEngine->CreateContext();
	}
	
	for(uint y = 0; y < gridHeight; y++)
	{
		for(uint x = 0; x < gridWidth; x++)
		{
			if(!equals(at(x, y), other.at(x, y), cmpContext))
			{
				if(cmpContext)
					cmpContext->Release();
				return false;
			}
		}
	}
	if(cmpContext)
		cmpContext->Release();

	return true;
}*/

//
//	AngelScript
//

Grid* Grid::GridFactory1(ScriptType *type)
{
	return GridFactory2(type, 0, 0);
}

Grid* Grid::GridFactory2(ScriptType *type, uint width, uint height)
{
	return GridFactory3(type, width, height, NULL);
}

Grid* Grid::GridFactory3(ScriptType *type, uint width, uint height, void *val)
{
	Grid *grid = new Grid(type, width, height, val);
	ScriptContext *ctx = asGetActiveContext();
	if(ctx && ctx->GetState() == asEXECUTION_EXCEPTION)
	{
		delete grid;
		return NULL;
	}
	return grid;
}

// This optional callback is called when the template type is first used by the compiler.
// It allows the application to validate if the template can be instanciated for the requested 
// subtype at compile time, instead of at runtime. The output argument dontGarbageCollect
// allow the callback to tell the engine if the template instance type shouldn't be garbage collected, 
// i.e. no asOBJ_GC flag. 
bool Grid::TemplateCallback(asIObjectType *ot, bool &dontGarbageCollect)
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

void Grid::enumReferences(ScriptEngine *engine)
{
	// If the array is holding handles, then we need to notify the GC of them
	if(subTypeId & asTYPEID_MASK_OBJECT)
	{
		void **d = (void**)buffer->data;
		for(uint n = 0; n < buffer->size; n++)
		{
			if(d[n])
				engine->GCEnumCallback(d[n]);
		}
	}
}

void Grid::releaseAllHandles(ScriptEngine*)
{
	// Resizing to zero will release everything
	resize(0, 0);
}

void Grid::addRef() const
{
	m_gcFlag = false;
	m_refCount++;
}

void Grid::releaseRef() const
{
	m_gcFlag = false;
	if(--m_refCount == 0)
		delete this;
}

int Grid::refCount() const
{
	return m_refCount;
}

void Grid::garbageCollect()
{
	m_gcFlag = true;
}

bool Grid::gcFlag()
{
	return m_gcFlag;
}