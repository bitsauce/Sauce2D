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

	r = scriptEngine->RegisterInterface("Serializable"); AS_ASSERT
	r = scriptEngine->RegisterInterfaceMethod("Serializable", "void serialize(StringStream&)"); AS_ASSERT
	r = scriptEngine->RegisterInterfaceMethod("Serializable", "void deserialize(StringStream&)"); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("ScriptManager", "void serialize(Serializable@, string &in)", asMETHODPR(xdScriptEngine, serialize, (asIScriptObject*, string&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptManager", "Serializable @deserialize(string &in)", asMETHODPR(xdScriptEngine, deserialize, (string&), asIScriptObject*), asCALL_THISCALL); AS_ASSERT

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
	if(XEngine::IsEnabled(XD_DEBUG)) {
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
	//if(XEngine::IsEnabled(XD_DEBUG))
	//	XEngine::GetDebugger()->sendPacket(XD_COMPILE_PACKET, out);
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
			//if(XEngine::IsEnabled(XD_DEBUG))
			//	XEngine::GetDebugger()->sendPacket(XD_MESSAGE_PACKET, "Unable to load main.as. Make sure the file and other included files exist.");
			//else
				throw xdException(XD_COMPILE_FAILED, "Unable to load main.as. Make sure the file and other included files exist.");
			return XD_COMPILE_FAILED;
		}

		r = builder.BuildModule();
		if(r < 0)
		{
			if(!XEngine::IsEnabled(XD_DEBUG))
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

void xdScriptEngine::serialize(asIScriptObject *object, string &path)
{
	// Make sure we got a valid object
	if(object)
	{
		// Store object type
		StringStream ss;
		serialize(object, ss);
		xdFileSystem::WriteFile(path, ss.str());

		// Release handle
		object->Release();
	}else
	{
		LOG("Cannot serialize 'null' objects");
	}
}

void xdScriptEngine::serialize(asIScriptObject *object, StringStream &ss)
{
	// Get script object type and it's serialization function
	asIObjectType *type = object->GetObjectType(); // We need to do this to get the 'actual' object type (not a base type)
	asIScriptFunction *func = type->GetMethodByDecl("void serialize(StringStream&)");

	// Store object type name
	ss.write(string(type->GetName()));
	
	// Call serialization function on the object
	asIScriptContext *ctx = createContext();
	int r = ctx->Prepare(func); assert(r >= 0);
	r = ctx->SetObject(object); assert(r >= 0);
	r = ctx->SetArgAddress(0, &ss); assert(r >= 0);
	r = ctx->Execute();

	// Clean up
	r = ctx->Release();
}

asIScriptObject *xdScriptEngine::deserialize(string &path)
{
	// Out script object
	asIScriptObject *object = 0;

	// Read file
	string content;
	if(xdFileSystem::ReadFile(path, content))
	{
		// Deserialize object
		StringStream ss(content);
		object = deserialize(ss);
	}else
	{
		LOG("Unable to read serialized file '%s'", path);
	}

	// Return object
	return object;
}

asIScriptObject *xdScriptEngine::deserialize(StringStream &ss)
{
	// Get type name
	string typeName;
	ss.read(typeName);
	
	// Create uninitialized object
	asIObjectType *type = m_module->GetObjectTypeByName(typeName.c_str());
	asIScriptObject *object = (asIScriptObject*)scriptEngine->CreateUninitializedScriptObject(type);

	if(object)
	{
		// Get deserialization function
		asIScriptFunction *func = type->GetMethodByDecl("void deserialize(StringStream&)");

		// Call deserialization function on the object
		asIScriptContext *ctx = createContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		r = ctx->SetObject(object); assert(r >= 0);
		r = ctx->SetArgAddress(0, &ss); assert(r >= 0);
		r = ctx->Execute();

		// Clean up
		r = ctx->Release();
	}else
	{
		LOG("Unable to create script object of type '%s'", typeName.c_str());
	}

	// Return script object
	return object;
}