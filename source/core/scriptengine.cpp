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

int getObjectTypeId(const char *objectName)
{
	asIObjectType *type = scriptEngine->GetObjectTypeByName(objectName);
	if(!type) return -1;
	return type->GetTypeId();
}

int registerGlobalFunction(const char *funcDef, const asSFuncPtr &funcPointer)
{
	int r = scriptEngine->RegisterGlobalFunction(funcDef, funcPointer, asCALL_CDECL);
	if(r < 0)
	{
		ERR("registerGlobalFunction failed with the error code %i, with funcDef '%s'", r, funcDef);
		assert(false);
	}
	return r;
}

int xdScriptEngine::registerSingletonType(const char *obj)
{
	return scriptEngine->RegisterObjectType(obj, 0, asOBJ_REF | asOBJ_NOHANDLE);
}

int xdScriptEngine::registerRefType(const char *obj, const asSFuncPtr &addRef, const asSFuncPtr &release)
{
	int r = 0;
	int typeId = r = scriptEngine->RegisterObjectType(obj, 0, asOBJ_REF); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour(obj, asBEHAVE_ADDREF, "void f()", addRef, asCALL_THISCALL); AS_ASSERT		
	r = scriptEngine->RegisterObjectBehaviour(obj, asBEHAVE_RELEASE, "void f()", release, asCALL_THISCALL); AS_ASSERT		
	return typeId;
}

int xdScriptEngine::registerValueType(const char *obj, const int size)
{
	return scriptEngine->RegisterObjectType(obj, size, asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CAK);
}

int xdScriptEngine::registerObjectFactory(const char *obj, const char *decl, const asSFuncPtr &func)
{
	return scriptEngine->RegisterObjectBehaviour(obj, asBEHAVE_FACTORY, decl, func, asCALL_CDECL);
}

int xdScriptEngine::registerObjectConstructor(const char *obj, const char *decl, const asSFuncPtr &func)
{
	return scriptEngine->RegisterObjectBehaviour(obj, asBEHAVE_CONSTRUCT, decl, func, asCALL_CDECL_OBJLAST);
}

int xdScriptEngine::registerObjectMethod(const char *obj, const char *decl, const asSFuncPtr &funcPointer)
{
	return scriptEngine->RegisterObjectMethod(obj, decl, funcPointer, asCALL_THISCALL);
}

int xdScriptEngine::registerObjectProperty(const char *obj, const char *decl, const int offset)
{
	return scriptEngine->RegisterObjectProperty(obj, decl, offset);
}

int xdScriptEngine::registerGlobalProperty(const char *decl, void *pointer)
{
	return scriptEngine->RegisterGlobalProperty(decl, pointer);
}

int xdScriptEngine::registerEnum(const char* name)
{
	return scriptEngine->RegisterEnum(name);
}

int xdScriptEngine::registerEnumValue(const char *enumname, const char *valuename, int value)
{
	return scriptEngine->RegisterEnumValue(enumname, valuename, value);
}

int xdScriptEngine::registerFuncdef(const char *decl)
{
	int r = scriptEngine->RegisterFuncdef(decl);
	if(r < 0)
	{
		ERR("registerFunctionDef failed with the error code %i, with decl '%s'", r, decl);
		assert(false);
	}
	return r;
}

int xdScriptEngine::releaseScriptObject(void *object)
{
	int r = 0;
	if(object) {
		r = static_cast<asIScriptObject*>(object)->Release();
	}
	return r;
}

int xdScriptEngine::releaseScriptFunc(void *function)
{
	int r = 0;
	if(function) {
		r = static_cast<asIScriptFunction*>(function)->Release();
	}
	return r;
}


void *getScriptFuncHandle(const char *decl)
{
	return 0;//xdEngine::GetScripts()->getModule()->GetFunctionByDecl(decl);
}

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

	// Script object // TODO: Transplant to MetaObject class
	/*r = scriptEngine->RegisterObjectMethod("ScriptManager", "void srcSetCreateArg(const uint, ?&in)", asMETHOD(xdScriptEngine, setCreateArg), asCALL_THISCALL);
	r = scriptEngine->RegisterObjectMethod("ScriptManager", "void srcResizeCreateArgs(const uint)", asMETHOD(xdScriptEngine, resizeCreateArgs), asCALL_THISCALL);
	r = scriptEngine->RegisterObjectMethod("ScriptManager", "void srcSetCreateObjectByName(const string)", asMETHOD(xdScriptEngine, setCreateObjectByName), asCALL_THISCALL);
	r = scriptEngine->RegisterObjectMethod("ScriptManager", "void srcSetCreateObjectById(const uint)", asMETHOD(xdScriptEngine, setCreateObjectById), asCALL_THISCALL);
	r = scriptEngine->RegisterObjectMethod("ScriptManager", "void srcCreateObject(?&out, const string s = \"\")", asMETHOD(xdScriptEngine, createObject), asCALL_THISCALL);
	r = scriptEngine->RegisterObjectMethod("ScriptManager", "string srcGetObjectClassName(?&in)", asMETHOD(xdScriptEngine, objectClassName), asCALL_THISCALL);*/

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
	m_createArgs.clear();
	
	// Clean up AngelScript
	asIScriptContext* ctx = asGetActiveContext();
	if(ctx) assert("AngelScript: There is an active context running!");
	if(scriptEngine) assert(scriptEngine->Release() == 0);
}

asIScriptContext *xdScriptEngine::createContext() const
{
	asIScriptContext *ctx = scriptEngine->CreateContext();
	if(xdEngine::IsEnabled(XD_DEBUG))
		ctx->SetLineCallback(asMETHOD(xdDebug, lineCallback), m_debugger, asCALL_THISCALL);
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

void xdScriptEngine::setCreateArg(const uint idx, void *value, int typeId)
{
	// Check for valid index
	if(m_createArgs.size() <= idx) return;
	m_createArgs[idx].set(value, typeId);
}

void xdScriptEngine::resizeCreateArgs(const uint size)
{
	m_createArgs.resize(size);
}

void xdScriptEngine::setCreateObjectByName(const string name)
{
	m_createObjectType = m_module->GetObjectTypeByName(name.c_str());
}

void xdScriptEngine::setCreateObjectById(const uint id)
{
	m_createObjectType = scriptEngine->GetObjectTypeByIndex(id);
}

void xdScriptEngine::createObject(void *object, int typeId, const string decl) const
{
	// Make sure we have an object
	if(!m_createObjectType) {
		ERR("xdScriptEngine::createObject() No object was defined!");
		return;
	}

	// Get constructor
	string className = m_createObjectType->GetName();
	asIScriptFunction *func = m_createObjectType->GetFactoryByDecl(string(className+"@ "+className+"(" + decl.c_str() + ")").c_str());
	if(func->GetParamCount() != m_createArgs.size()) {
		ERR("xdScriptEngine::createObject() Invalid number of arguments (got %i, expected %i)!", m_createArgs.size(), func->GetParamCount());
		return;
	}
	
	// Prepare the object constructor
	int r;
	asIScriptContext *ctx = createContext();
	r = ctx->Prepare(func); assert(r >= 0);

	// Set constructor arguments
	for(uint i = 0; i < m_createArgs.size(); i++)
	{
		// Make sure all the argument is defined
		const ScriptArgument *arg = &m_createArgs[i];
		if(!arg->valid) {
			asIScriptContext *ctx = asGetActiveContext();
			if(ctx) ctx->SetException("Invalid argument");
			return;
		}

		// Set argument
		if((arg->typeId & ~asTYPEID_MASK_SEQNBR) && !(arg->typeId & asTYPEID_OBJHANDLE)) // Script object type
			ctx->SetArgObject(i, arg->value);
		else if(arg->typeId & asTYPEID_OBJHANDLE) // Handle type
		{
			scriptEngine->AddRefScriptObject(arg->value, scriptEngine->GetObjectTypeById(arg->typeId));
			ctx->SetArgAddress(i, arg->value);
		}else if(arg->typeId == asTYPEID_BOOL || // 8-bit types
				arg->typeId == asTYPEID_INT8 ||
				arg->typeId == asTYPEID_UINT8)
			ctx->SetArgByte(i, *(char*)arg->value);
		else if(arg->typeId == asTYPEID_INT16 || // 16-bit types
				arg->typeId == asTYPEID_UINT16)
			ctx->SetArgWord(i, *(short*)arg->value);
		else if(arg->typeId == asTYPEID_INT32 || // 32-bit types
				arg->typeId == asTYPEID_UINT32 ||
				arg->typeId == asTYPEID_FLOAT ||
				arg->typeId > asTYPEID_DOUBLE) // enums have a type id larger than doubles
			ctx->SetArgDWord(i, *(int*)arg->value);
		else if(arg->typeId == asTYPEID_INT64 || // 64-bit types
				arg->typeId == asTYPEID_UINT64 ||
				arg->typeId == asTYPEID_DOUBLE)
			ctx->SetArgQWord(i, *(asQWORD*)arg->value);
	}

	// Execute constructor
	r = ctx->Execute(); assert(r >= 0);
	if(typeId & asTYPEID_OBJHANDLE) {
		void *newObject = ctx->GetReturnAddress(); // Store return address
		((asIScriptObject*)newObject)->AddRef(); // Keep alive
		*(void**)object = newObject;
	}
	ctx->Release();
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

ScriptArgument::ScriptArgument() :
	valid(false)
{
}

ScriptArgument::~ScriptArgument()
{
	clear();
}

void ScriptArgument::set(void *value, int typeId)
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

void ScriptArgument::clear()
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