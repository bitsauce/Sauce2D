//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)

#include "x2d/scripts.h"
#include "x2d/engine.h"

// Script engine
ScriptEngine* scriptEngine = 0;

int getObjectTypeId(const char *objectName)
{
	ScriptType *type = scriptEngine->GetObjectTypeByName(objectName);
	if(!type) return -1;
	return type->GetTypeId();
}

int registerGlobalFunction(const char *funcDef, const asSFuncPtr &funcPointer)
{
	int r = scriptEngine->RegisterGlobalFunction(funcDef, funcPointer, asCALL_CDECL);
	if(r < 0) {
#ifdef DEPRICATED
		print("registerGlobalFunction failed with the error code %i, with funcDef '%s'", r, funcDef);
#endif
		assert(false);
	}
	return r;
}

int registerFunctionDef(const char *decl)
{
	int r = scriptEngine->RegisterFuncdef(decl);
	if(r < 0) {
#ifdef DEPRICATED
		print("registerFunctionDef failed with the error code %i, with decl '%s'", r, decl);
#endif
		assert(false);
	}
	return r;
}

int releaseScriptObject(void *obj)
{
	ScriptObject *scriptObject = (ScriptObject*)obj;
	return scriptObject->Release();
}

int releaseScriptFunc(void *func)
{
	ScriptFunction *scriptFunc = (ScriptFunction*)func;
	return scriptFunc->Release();
}

ScriptContext *currentCtx = 0;
vector<ScriptArgument> currentArgs;
int startScriptFuncCall(void *func)
{
	if(currentCtx != 0) return -1;
	currentCtx = gameEngine->scripts->createScriptContext();
	int r = currentCtx->Prepare((ScriptFunction*)func);
	if(r < 0) {
#ifdef DEPRICATED
		print("startScriptFuncCall failed with the error code %i", r);
#endif
		assert(false);
	}
	return r;
}

int addScriptFuncArg(void *value, int typeId)
{
	if(currentCtx == 0) return -1;
	currentArgs.resize(currentArgs.size()+1);
	currentArgs.back().set(value, typeId);
	return 0;
}

int endScriptFuncCall()
{
	if(currentCtx == 0) return -1;
	
	// Set script call arguments
	for(uint i = 0; i < currentArgs.size(); i++)
	{
		// Make sure all the argument is defined
		const ScriptArgument *arg = &currentArgs[i];
		if(!arg->valid) {
			ScriptContext *ctx = asGetActiveContext();
			if(ctx) ctx->SetException("Invalid argument");
			return -2;
		}

		// Set argument
		if((arg->typeId & ~asTYPEID_MASK_SEQNBR) && !(arg->typeId & asTYPEID_OBJHANDLE)) // Script object type
			currentCtx->SetArgObject(i, arg->value);
		else if(arg->typeId & asTYPEID_OBJHANDLE) // Handle type
		{
			scriptEngine->AddRefScriptObject(arg->value, scriptEngine->GetObjectTypeById(arg->typeId));
			currentCtx->SetArgAddress(i, arg->value);
		}else if(arg->typeId == asTYPEID_BOOL || // 8-bit types
				arg->typeId == asTYPEID_INT8 ||
				arg->typeId == asTYPEID_UINT8)
			currentCtx->SetArgByte(i, *(char*)arg->value);
		else if(arg->typeId == asTYPEID_INT16 || // 16-bit types
				arg->typeId == asTYPEID_UINT16)
			currentCtx->SetArgWord(i, *(short*)arg->value);
		else if(arg->typeId == asTYPEID_INT32 || // 32-bit types
				arg->typeId == asTYPEID_UINT32 ||
				arg->typeId == asTYPEID_FLOAT ||
				arg->typeId > asTYPEID_DOUBLE) // enums have a type id larger than doubles
			currentCtx->SetArgDWord(i, *(int*)arg->value);
		else if(arg->typeId == asTYPEID_INT64 || // 64-bit types
				arg->typeId == asTYPEID_UINT64 ||
				arg->typeId == asTYPEID_DOUBLE)
			currentCtx->SetArgQWord(i, *(double*)arg->value);
	}

	// Execute script func
	int r = currentCtx->Execute();
	if(r < 0) {
#ifdef DEPRICATED
		print("endScriptFuncCall failed with the error code %i", r);
#endif
		assert(false);
	}

	// Clean up
	r = currentCtx->Release();
	if(r < 0) {
#ifdef DEPRICATED
		print("endScriptFuncCall failed with the error code %i", r);
#endif
		assert(false);
	}
	currentCtx = 0;
	currentArgs.clear();
	return r;
}

int registerEnum(const char* name)
{
	int r = scriptEngine->RegisterEnum(name);
	if(r < 0) {
#ifdef DEPRICATED
		print("registerEnum failed with the error code %i", r);
#endif
		assert(false);
	}
	return r;
}

int registerEnumValue(const char *enumname, const char *valuename, int value)
{
	int r = scriptEngine->RegisterEnumValue(enumname, valuename, value);
	if(r < 0) {
#ifdef DEPRICATED
		print("registerEnumValue failed with the error code %i", r);
#endif
		assert(false);
	}
	return r;
}

void *getScriptFuncHandle(const char *decl)
{
	return scriptEngine->GetGlobalFunctionByDecl(decl);
}


ScriptManager::ScriptManager()
{
	int r;

	// Scirpt classes
	AS_SINGLETON_FUNCTION(ScriptManager, "uint srcGetClassCount()", classCount, this)
	AS_SINGLETON_FUNCTION(ScriptManager, "int srcGetClassIdByName(const string)", classIdByName, this)
	AS_SINGLETON_FUNCTION(ScriptManager, "string srcGetClassNameById(const uint)", classNameById, this)
	AS_SINGLETON_FUNCTION(ScriptManager, "bool srcIsClassName(const string)", isClassName, this)
	AS_SINGLETON_FUNCTION(ScriptManager, "bool srcClassDerivesFromName(const string, const string)", classDerivesFromName, this)
	AS_SINGLETON_FUNCTION(ScriptManager, "bool srcClassDerivesFromId(const uint, const uint)", classDerivesFromId, this)

	// Script object
	AS_SINGLETON_FUNCTION(ScriptManager, "void srcSetCreateArg(const uint, ?&in)", setCreateArg, this)
	AS_SINGLETON_FUNCTION(ScriptManager, "void srcResizeCreateArgs(const uint)", resizeCreateArgs, this)
	AS_SINGLETON_FUNCTION(ScriptManager, "void srcSetCreateObjectByName(const string)", setCreateObjectByName, this)
	AS_SINGLETON_FUNCTION(ScriptManager, "void srcSetCreateObjectById(const uint)", setCreateObjectById, this)
	AS_SINGLETON_FUNCTION(ScriptManager, "void srcCreateObject(?&out, const string s = \"\")", createObject, this)
	AS_SINGLETON_FUNCTION(ScriptManager, "string srcGetObjectClassName(?&in)", objectClassName, this)
}


ScriptManager::~ScriptManager()
{
	m_createArgs.clear();
}

ScriptContext *ScriptManager::createScriptContext() const
{
	ScriptContext *ctx = scriptEngine->CreateContext();
	//if(X2DEngine::GameDebugger)
	//	ctx->SetLineCallback(asMETHOD(Debugger, lineCallback), X2DEngine::GameDebugger, asCALL_THISCALL);
	return ctx;
}

void ScriptManager::setModule(ScriptModule *mod)
{
	m_module = mod;
}

ScriptModule *ScriptManager::module()
{
	return m_module;
}

uint ScriptManager::classCount() const
{
	return m_module->GetObjectTypeCount();
}

int ScriptManager::classIdByName(const string name) const
{
	for(uint i = 0; i < m_module->GetObjectTypeCount(); i++) {
		if(name == string(m_module->GetObjectTypeByIndex(i)->GetName()))
			return i;
	}
	return -1;
}

string ScriptManager::classNameById(const uint id) const
{
	ScriptType *objType = m_module->GetObjectTypeByIndex(id);
	if(!objType) return ""; // Invalid id
	return objType->GetName();
}

bool ScriptManager::isClassName(const string name) const
{
	for(uint i = 0; i < m_module->GetObjectTypeCount(); i++) {
		if(name == string(m_module->GetObjectTypeByIndex(i)->GetName()))
			return true;
	}
	return false;
}

bool ScriptManager::classDerivesFromName(const string name1, const string name2) const
{
	int id1 = classIdByName(name1);
	if(id1 < 0) return false;
	int id2 = classIdByName(name2);
	if(id2 < 0) return false;
	return classDerivesFromId(id1, id2);
}

bool ScriptManager::classDerivesFromId(const uint id1, const uint id2) const
{
	ScriptType *objType = m_module->GetObjectTypeByIndex(id1);
	if(objType->DerivesFrom(m_module->GetObjectTypeByIndex(id2)) && id1 != id2) return true;
	return false;
}

void ScriptManager::setCreateArg(const uint idx, void *value, int typeId)
{
	// Check for valid index
	if(m_createArgs.size() <= idx) return;
	m_createArgs[idx].set(value, typeId);
}

void ScriptManager::resizeCreateArgs(const uint size)
{
	m_createArgs.resize(size);
}

void ScriptManager::setCreateObjectByName(const string name)
{
	m_createObjectType = m_module->GetObjectTypeByName(name.c_str());
}

void ScriptManager::setCreateObjectById(const uint id)
{
	m_createObjectType = scriptEngine->GetObjectTypeByIndex(id);
}

void ScriptManager::createObject(void *object, int typeId, const string decl) const
{
	// Make sure we have an object
	if(!m_createObjectType) {
		print("ScriptManager::createObject() No object was defined!");
		return;
	}

	// Get constructor
	string className = m_createObjectType->GetName();
	ScriptFunction *func = m_createObjectType->GetFactoryByDecl(string(className+"@ "+className+"(" + decl.c_str() + ")").c_str());
	if(func->GetParamCount() != m_createArgs.size()) {
#ifdef DEPRICATED
		print("ScriptManager::createObject() Invalid number of arguments (got %i, expected %i)!", m_createArgs.size(), func->GetParamCount());
#endif
		return;
	}
	
	// Prepare the object constructor
	int r;
	ScriptContext *ctx = createScriptContext();
	r = ctx->Prepare(func); assert(r >= 0);

	// Set constructor arguments
	for(uint i = 0; i < m_createArgs.size(); i++)
	{
		// Make sure all the argument is defined
		const ScriptArgument *arg = &m_createArgs[i];
		if(!arg->valid) {
			ScriptContext *ctx = asGetActiveContext();
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
			ctx->SetArgQWord(i, *(double*)arg->value);
	}

	// Execute constructor
	r = ctx->Execute(); assert(r >= 0);
	if(typeId & asTYPEID_OBJHANDLE) {
		void *newObject = ctx->GetReturnAddress(); // Store return address
		((ScriptObject*)newObject)->AddRef(); // Keep alive
		*(void**)object = newObject;
	}
	ctx->Release();
}

string ScriptManager::objectClassName(void *obj, int typeId) const
{
	// Check if the provied object
	// is a script object
	if(!(typeId & asTYPEID_SCRIPTOBJECT)) {
		ScriptContext *ctx = asGetActiveContext();
		if(ctx) {
			// Set a script exception
			ctx->SetException("ScriptManager::objectClassName() should only be called on a script object!");
		}
		return "";
	}

	// Get the provided script object
	ScriptObject* scriptObject = NULL;
	scriptObject = static_cast<ScriptObject*>(*(void**)obj); // Reference-to-pointer

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
	ScriptType *objectType = scriptEngine->GetObjectTypeById(typeId);
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
		if(typeId & asTYPEID_OBJHANDLE)
			scriptEngine->ReleaseScriptObject(value, scriptEngine->GetObjectTypeById(typeId));
		delete value;
	}
	valid = false;
}

//----------------------------------------------------------------------------
// AngelScript
//----------------------------------------------------------------------------

#include "x2d/app.h"
#include "core/debugger.h"

#include "scripts/scripthelper.h"
#include "scripts/scriptbuilder.h"

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
#ifdef DEPRICATED
	// Print to debug
	string str = sprint("%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message);
	if(msg->type == asMSGTYPE_ERROR)
		print(str);
	else if(msg->type == asMSGTYPE_WARNING)
		print(str);
	else if(msg->type == asMSGTYPE_INFORMATION)
		return; //inform(str);
	
	// Store or send message info
	if(gameEngine->debug)
		gameEngine->debug->sendData(str.c_str(), X2DDebug::CompilePacket);
	else
		messageCallback += str;
#endif
}

// Compiles an angelscript module with name
int asCompileModule(string name)
{
	bool error = true;
	int r;
	while(error)
	{
		CScriptBuilder builder;
		r = builder.StartNewModule(scriptEngine, name.c_str()); 
		if(r < 0) 
		{
			// If the code fails here it is usually because there
			// is no more memory to allocate the module
			gameEngine->app->exception(X2D_CompileError, "Unable to start a new module. Usually do to lack of memory.");
			return X2D_CompileError;
		}

		r = builder.AddSectionFromFile(gameEngine->getWorkingPath("main.as").c_str());
		if(r < 0)
		{
			// The builder wasn't able to load the file. Maybe the file
			// has been removed, or the wrong name was given, or some
			// preprocessing commands are incorrectly written.
			if(!gameEngine->debug)
				gameEngine->app->exception(X2D_CompileError, "Unable to load main.as. Make sure the file and other included files exist.");
			return X2D_CompileError;
		}

		r = builder.BuildModule();
		if(r < 0)
		{
			if(!gameEngine->debug)
			{
				// An error occurred. Instruct the script writer to fix the 
				// compilation errors that were listed in the output stream.
				string debug = "AngelScript has encountered the following error:\n\n";
				debug += messageCallback;
				debug += "\nPlease fix the error(s) and press retry, or press cancel to exit the application.";
				messageCallback.clear();

				// TODO: Implement retry
				// Show retry dialog
				gameEngine->app->exception(X2D_CompileError, debug.c_str());
			}

			return X2D_CompileError;
		}else{
			error = false;
		}
	}
	return 0;
}

// Recompiles the script modules
void asRecompile()
{
	// Get module name
	//ScriptModule* mod = scriptEngine->GetModule("GameModule");

	// Tell the serializer how the user types should be serialized
	// by adding the implementations of the CUserType interface
	//CSerializer backup;
	//backup.AddUserType(new CStringType(), "string");
	//backup.AddUserType(new CArrayType(), "array");

	// Backup the values of the global variables
	//backup.Store(mod);
  
	// Application can now recompile the module
	//asCompileModule("GameModule");

	// Restore the values of the global variables in the new module
	//mod = scriptEngine->GetModule("GameModule", asGM_ONLY_IF_EXISTS);
	//backup.Restore(mod);
}

// Execute string
void asExec(string &exec)
{
	ExecuteString(scriptEngine, exec.c_str(), scriptEngine->GetModule("GameModule"));
}

// Exit game
void asExit()
{
	// Close game
	gameEngine->app->close();
}