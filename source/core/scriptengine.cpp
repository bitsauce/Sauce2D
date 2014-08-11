//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/scriptengine.h>
#include <x2d/engine.h>
#include <x2d/filesystem.h>
#include <x2d/debug.h>
#include <x2d/exception.h>

// Script engine
asIScriptEngine* scriptEngine = 0;

// HAX

#include <x2d/scripts/array.h>
#include <x2d/scriptengine.h>
#include "scripts/scriptarray.h"
#include "scripts/scriptany.h"
#include "scripts/scriptfunc.h"

Array *CreateArray(const string &type, const uint size)
{
	// Create decl
	string decl = "array<";
	decl += type;
	decl += ">";

    // Obtain a pointer to the engine
	asIObjectType *arrayType = scriptEngine->GetObjectTypeById(scriptEngine->GetTypeIdByDecl(decl.c_str()));

    // Create the array object
    return new CScriptArray(size, arrayType);
}

AnyObject *CreateAnyObject()
{
    // Create the array object
	return new CScriptAny(scriptEngine);
}

FunctionCall *CreateFuncCall()
{
    // Create the array object
	return new CScriptFuncCall(scriptEngine);
}

// HAX END

AS_REG_SINGLETON(xdScriptEngine, "ScriptManager")

int xdScriptEngine::Register(asIScriptEngine *scriptEngine)
{
	int r;

	// Scirpt classes
	r = scriptEngine->RegisterObjectMethod("ScriptManager", "uint getClassCount() const", asMETHOD(xdScriptEngine, classCount), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptManager", "int getClassIdByName(const string)", asMETHOD(xdScriptEngine, classIdByName), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptManager", "string getClassNameById(const uint)", asMETHOD(xdScriptEngine, classNameById), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptManager", "bool classExists(const string)", asMETHOD(xdScriptEngine, isClassName), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptManager", "bool classDerivesFromName(const string, const string)", asMETHOD(xdScriptEngine, classDerivesFromName), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptManager", "bool classDerivesFromId(const uint, const uint)", asMETHOD(xdScriptEngine, classDerivesFromId), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("ScriptManager", "void serialize(?&in, string &in)", asMETHODPR(xdScriptEngine, serialize, (void*, int, string&), void), asCALL_THISCALL);
	r = scriptEngine->RegisterObjectMethod("ScriptManager", "void deserialize(?&out, string &in)", asMETHODPR(xdScriptEngine, deserialize, (void*, int, string&), void), asCALL_THISCALL);

	return r;
}

xdScriptEngine::xdScriptEngine(asIScriptEngine *scriptEngine, xdDebug *debugger) :
	m_module(0), // Set later by the engine
	m_debugger(debugger)
{
	::scriptEngine = scriptEngine;
}

xdScriptEngine::~xdScriptEngine()
{
	// Clean up AngelScript
	asIScriptContext* ctx = asGetActiveContext();
	if(ctx) assert("AngelScript: There is an active context running!");
	if(scriptEngine) assert(scriptEngine->Release() == 0);
}

asIScriptContext *xdScriptEngine::createContext() const
{
	asIScriptContext *ctx = scriptEngine->CreateContext();
	if(xdEngine::IsEnabled(XD_DEBUG)) {
		ctx->SetLineCallback(asMETHOD(xdDebug, lineCallback), m_debugger, asCALL_THISCALL);
	}
	return ctx;
}

asIScriptModule *xdScriptEngine::getModule() const
{
	return m_module;
}

asIScriptEngine *xdScriptEngine::getASEngine() const
{
	return scriptEngine;
}

uint xdScriptEngine::classCount() const
{
	return m_module->GetObjectTypeCount();
}

int xdScriptEngine::classIdByName(const string name) const
{
	for(uint i = 0; i < m_module->GetObjectTypeCount(); i++) {
		if(name == string(m_module->GetObjectTypeByIndex(i)->GetName()))
			return i;
	}
	return -1;
}

string xdScriptEngine::classNameById(const uint id) const
{
	asIObjectType *objType = m_module->GetObjectTypeByIndex(id);
	if(!objType) return ""; // Invalid id
	return objType->GetName();
}

bool xdScriptEngine::isClassName(const string name) const
{
	for(uint i = 0; i < m_module->GetObjectTypeCount(); i++) {
		if(name == string(m_module->GetObjectTypeByIndex(i)->GetName()))
			return true;
	}
	return false;
}

bool xdScriptEngine::classDerivesFromName(const string name1, const string name2) const
{
	int id1 = classIdByName(name1);
	if(id1 < 0) return false;
	int id2 = classIdByName(name2);
	if(id2 < 0) return false;
	return classDerivesFromId(id1, id2);
}

bool xdScriptEngine::classDerivesFromId(const uint id1, const uint id2) const
{
	asIObjectType *objType = m_module->GetObjectTypeByIndex(id1);
	if(objType->DerivesFrom(m_module->GetObjectTypeByIndex(id2)) && id1 != id2) return true;
	return false;
}

string xdScriptEngine::objectClassName(void *obj, int typeId) const
{
	// Check if the provied object
	// is a script object
	if(!(typeId & asTYPEID_SCRIPTOBJECT)) {
		asIScriptContext *ctx = asGetActiveContext();
		if(ctx) {
			// Set a script exception
			ctx->SetException("xdScriptEngine::objectClassName() should only be called on a script object!");
		}
		return "";
	}

	// Get the provided script object
	asIScriptObject* scriptObject = NULL;
	scriptObject = static_cast<asIScriptObject*>(*(void**)obj); // Reference-to-pointer

	// Return name
	return scriptObject->GetObjectType()->GetName();
}

ScriptValue::ScriptValue() :
	valid(false)
{
}

ScriptValue::~ScriptValue()
{
	clear();
}

void ScriptValue::set(void *value, int typeId)
{
	// Set argument value
	asIObjectType *objectType = scriptEngine->GetObjectTypeById(typeId);
	if(typeId == asTYPEID_VOID || typeId & asTYPEID_OBJHANDLE)
	{
		// Add handle reference and store address
		scriptEngine->AddRefScriptObject(*(void**)value, objectType);
		this->value = *(void**)value;
	}else if(typeId & asTYPEID_MASK_OBJECT)
	{
		// Construct and copy script object
		this->value = scriptEngine->CreateScriptObject(objectType);
		scriptEngine->AssignScriptObject(this->value, value, objectType);
	}else{
		// Copy primitive value
		int primitiveSize = scriptEngine->GetSizeOfPrimitiveType(typeId);
		this->value = malloc(primitiveSize);
		memcpy(this->value, value, primitiveSize);
	}
	this->valid = true;
	this->typeId = typeId;
}

void ScriptValue::clear()
{
	// Clear the argument data
	if(valid) {
		if(typeId == asTYPEID_VOID || typeId & asTYPEID_OBJHANDLE)
			scriptEngine->ReleaseScriptObject(value, scriptEngine->GetObjectTypeById(typeId));
		else
			free(value);
	}
	valid = false;
}

//----------------------------------------------------------------------------
// AngelScript
//----------------------------------------------------------------------------

#include <x2d/console.h>
#include <x2d/debug.h>

#include "scripts/scripthelper.h"
#include "scripts/scriptbuilder.h"

#include <stdio.h>

// AngelScript callback function
string messageCallback;
void asMessageCallback(const asSMessageInfo *msg, void*)
{
	// Callback type
	const char *type = "ERR";
	if(msg->type == asMSGTYPE_WARNING) 
		type = "WARN";
	else if(msg->type == asMSGTYPE_INFORMATION) 
		type = "INFO";

	// Print to debug
	char out[512];
#ifdef USE_CTR_SECURE
	sprintf_s(out, "%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message);
#else
	sprintf(out, "%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message);
#endif
	/*if(msg->type == asMSGTYPE_ERROR)
		ERR(str);
	else if(msg->type == asMSGTYPE_WARNING)
		iosystem::warn(str);
	else if(msg->type == asMSGTYPE_INFORMATION)
		LOG(str);*/
	LOG("%s: %s", type, out);
	
	// Store or send message info
	//if(xdEngine::IsEnabled(XD_DEBUG))
	//	xdEngine::GetDebugger()->sendPacket(XD_COMPILE_PACKET, out);
	//else
		messageCallback += string(out);
}

// Compiles an angelscript module with name
int asCompileModule(const string &name, xdFileSystem *fileSystem)
{
	bool error = true;
	int r;
	while(error)
	{
		CScriptBuilder builder(fileSystem);
		r = builder.StartNewModule(scriptEngine, name.c_str()); 
		if(r < 0)
		{
			// If the code fails here it is usually because there is no more memory to allocate the module
			throw xdException(XD_COMPILE_FAILED, "Unable to start a new module. Usually do to lack of memory.");
		}

		r = builder.AddSectionFromFile("main.as");
		if(r < 0)
		{
			// The builder wasn't able to load the file. Maybe the file
			// has been removed, or the wrong name was given, or some
			// preprocessing commands are incorrectly written.
			//if(xdEngine::IsEnabled(XD_DEBUG))
			//	xdEngine::GetDebugger()->sendPacket(XD_MESSAGE_PACKET, "Unable to load main.as. Make sure the file and other included files exist.");
			//else
				throw xdException(XD_COMPILE_FAILED, "Unable to load main.as. Make sure the file and other included files exist.");
			return XD_COMPILE_FAILED;
		}

		r = builder.BuildModule();
		if(r < 0)
		{
			if(!xdEngine::IsEnabled(XD_DEBUG))
			{
				// An error occurred. Instruct the script writer to fix the 
				// compilation errors that were listed in the output stream.
				string debug = "AngelScript has encountered the following error:\n\n";
				debug += messageCallback;
				debug += "\nPlease fix the error(s) and press retry, or press cancel to exit the application.";
				messageCallback.clear();

				// TODO: Implement retry
				// Show retry dialog
				throw xdException(XD_COMPILE_FAILED, debug);
			}

			return XD_COMPILE_FAILED;
		}else{
			error = false;
		}
	}
	return 0;
}

// Execute string
void xdScriptEngine::executeString(const string &str) const
{
	ExecuteString(scriptEngine, str.c_str(), scriptEngine->GetModule("GameModule"));
}

#include "../scripts/serializer.h"

void xdScriptEngine::serialize(void *value, int typeId, string &path)
{
	// Make sure we got a valid object
	if(value != 0 && (typeId & asTYPEID_SCRIPTOBJECT) && (typeId &asTYPEID_OBJHANDLE))
	{
		// Store object type
		stringstream ss;
		ss << (*(asIScriptObject**)value)->GetObjectType()->GetName() << endl;
		serialize(value, typeId, ss);
		xdFileSystem::WriteFile(path, ss.str());
		m_pointerAddresses.clear();
	}else{
		LOG("Cannot serialize object of type '%s'", scriptEngine->GetObjectTypeById(typeId)->GetBaseType()->GetName());
	}

	if(typeId & asTYPEID_MASK_OBJECT)
	{
		//scriptEngine->ReleaseScriptObject(value, scriptEngine->GetObjectTypeById(typeId));
	}
}

void xdScriptEngine::serialize(void *value, int typeId, stringstream &ss)
{
	// Make sure we got a valid object
	if(value == 0)
		return;
	
	if(typeId & asTYPEID_MASK_OBJECT)
	{
		// For script objects we need to serialize it's properties
		if(typeId & asTYPEID_SCRIPTOBJECT)
		{
			// Get object and type
			asIScriptObject *object = 0;
			if(typeId & asTYPEID_OBJHANDLE)
			{
				// If it is a handle we need to dereference it
				asIScriptObject *handleObj = *(asIScriptObject**)value;

				// Get address of the object pointed to by the handle
				intptr_t addr = (intptr_t)handleObj;
				if(m_pointerAddresses.find(addr) == m_pointerAddresses.end())
				{
					// Add this object to our address list
					m_pointerAddresses[addr] = handleObj;
					object = handleObj; // Set the object up for serialization
				}

				// Store the object address
				ss << addr << endl;
			}else{
				object = (asIScriptObject*)value;
			}
		
			if(object)
			{
				// Serialize the object and its properties
				asIObjectType *type = object->GetObjectType();
				for(int i = 0; i < type->GetPropertyCount(); i++)
				{
					serialize(object->GetAddressOfProperty(i), object->GetPropertyTypeId(i), ss);
				}
			}
		}else // if(asAPP_CLASS)
		{
			// Get object pointer
			void *object = 0;
			if(typeId & asTYPEID_OBJHANDLE)
			{
				// Have we already stored this object?
				if(m_pointerAddresses.find((intptr_t)value) == m_pointerAddresses.end())
				{
					// We're receiving a reference to the handle, so we need to dereference it
					object = *(void**)value;
					ss << (intptr_t)value;
				}

				// 1) is adress already stored?
				// 2) no? serialize the object which the handle points to and store its adress. yes? do nothing
				// 3) assign this property to the appropriate adress
			}else{
				object = value;
			}
			
			// Get object type
			asIObjectType *type = scriptEngine->GetObjectTypeById(typeId);
			if(object)
			{
				asIScriptFunction *func = type->GetMethodByDecl("void serialize(stringstream &in) const");
				if(func)
				{
					// Call 'void serialize()' on object
					asIScriptContext *ctx = createContext();
					ctx->Prepare(func);
					ctx->SetObject(object);
					ctx->SetArgAddress(0, &ss);
					ctx->Execute();

					// Clean up
					ctx->Release();
				}else{
					LOG("Object type '%s' does not support serialization!", type->GetName());
				}
			}
		}
	}else
	{
		// Primitive types
		if(typeId == asTYPEID_BOOL || typeId == asTYPEID_INT8 || typeId == asTYPEID_UINT8)
			ss << *(char*)value;
		else if(typeId == asTYPEID_INT16 || typeId == asTYPEID_UINT16)
			ss << *(short*)value;
		else if(typeId == asTYPEID_INT32 || typeId == asTYPEID_UINT32 || typeId == asTYPEID_FLOAT || typeId > asTYPEID_DOUBLE) // enums have a type id larger than doubles
			ss << *(int*)value;
		else if(typeId == asTYPEID_INT64 || typeId == asTYPEID_UINT64 || typeId == asTYPEID_DOUBLE)
			ss << *(double*)value;
		ss << endl;
	}
}

void xdScriptEngine::deserialize(void *value, int typeId, string &path)
{
	// Read file
	string content;
	if(!xdFileSystem::ReadFile(path, content))
	{
		LOG("Scripts::deserialize() - Unable to read serialized file '%s'", path);
		return;
	}
	stringstream ss(content);
	
	// Get object type by name
	string typeName;
	ss >> typeName;
	asIObjectType *type = m_module->GetObjectTypeByName(typeName.c_str());
	if(type)
	{
		// Create uninitialized script object
		void *object = 0;
		deserialize(&object, typeId, ss);
		m_pointerAddresses.clear();

		// Set out object
		if(typeId & asTYPEID_MASK_OBJECT && scriptEngine->IsHandleCompatibleWithObject(object, type->GetTypeId(), typeId))
		{
			*(void**)value = object;
		}else if(typeId != asTYPEID_VOID) {
			LOG("Scripts::deserialize() - Output variable is not a handle");
		}
	}
}

void xdScriptEngine::deserialize(void *value, int typeId, stringstream &ss)
{
	if(typeId & asTYPEID_MASK_OBJECT)
	{
		// Get object type
		asIObjectType *type = scriptEngine->GetObjectTypeById(typeId);

		// For script objects we need to deserialize it's properties
		if(typeId & asTYPEID_SCRIPTOBJECT)
		{
			// Get object and type
			asIScriptObject *object = 0;
			if(typeId & asTYPEID_OBJHANDLE)
			{
				// Get the stored address value
				intptr_t addr;
				ss >> addr;
				ss.ignore();

				// Check if we've already restored this object
				if(m_pointerAddresses.find(addr) == m_pointerAddresses.end())
				{
					// No. Create uninitialized object
					asIScriptObject *handleObj = *(asIScriptObject**)value;
					if(handleObj == 0) {
						handleObj = (asIScriptObject*)scriptEngine->CreateUninitializedScriptObject(type);
					}

					// Add this object to our address list
					m_pointerAddresses[addr] = handleObj;
					object = handleObj; // This will deserialize the object
					*(void**)value = handleObj;
				}else{
					// Yes. Set out value and add a reference to the script object
					asIScriptObject *handleObj = (asIScriptObject*)m_pointerAddresses[addr];
					handleObj->AddRef();
					*(void**)value = handleObj;
				}
			}else{
				object = (asIScriptObject*)value;
			}
		
			if(object)
			{
				// Deserialize all object properties
				for(int i = 0; i < type->GetPropertyCount(); i++)
				{
					deserialize(object->GetAddressOfProperty(i), object->GetPropertyTypeId(i), ss);
				}
			}
		}else
		{
			// Get object type
			asIObjectType *type = scriptEngine->GetObjectTypeById(typeId);

			// Get object pointer
			void *object = 0;
			if(typeId & asTYPEID_OBJHANDLE)
			{
				if(m_pointerAddresses.find((intptr_t)value) == m_pointerAddresses.end())
				{
					// We're receiving a reference to the handle, so we need to dereference it
					object = *(void**)value;
					ss << (intptr_t)value;
				}

				// 1) is adress already stored?
				// 2) no? serialize the object which the handle points to and store its adress. yes? do nothing
				// 3) assign this property to the appropriate adress
			}else{
				object = value;
			}
		
			if(object)
			{
				asIScriptFunction *func = type->GetMethodByDecl("void deserialize(stringstream &in)");
				if(func)
				{
					// Call 'void deserialize()' on object
					asIScriptContext *ctx = createContext();
					ctx->Prepare(func);
					ctx->SetObject(object);
					ctx->SetArgAddress(0, &ss);
					ctx->Execute();

					// Clean up
					ctx->Release();
				}else{
					LOG("Object type '%s' does not support serialization!", type->GetName());
				}
			}
		}
	}else
	{
		// Primitive types
		if(typeId == asTYPEID_BOOL || typeId == asTYPEID_INT8 || typeId == asTYPEID_UINT8)
			ss >> *(char*)value;
		else if(typeId == asTYPEID_INT16 || typeId == asTYPEID_UINT16)
			ss >> *(short*)value;
		else if(typeId == asTYPEID_INT32 || typeId == asTYPEID_UINT32 || typeId == asTYPEID_FLOAT || typeId > asTYPEID_DOUBLE) // enums have a type id larger than doubles
			ss >> *(int*)value;
		else if(typeId == asTYPEID_INT64 || typeId == asTYPEID_UINT64 || typeId == asTYPEID_DOUBLE)
			ss >> *(double*)value;
		ss.ignore();
	}
}