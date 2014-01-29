#ifndef CORE_SCRIPTS_H
#define CORE_SCRIPTS_H

#include "platform.h"

#ifdef USING_AS

// AngelScript header
#include "angelscript.h"

typedef asIScriptObject ScriptObject;
typedef asIObjectType ScriptType;
typedef asIScriptFunction ScriptFunction;
typedef asIScriptModule ScriptModule;
typedef asIScriptEngine ScriptEngine;
typedef asIScriptContext ScriptContext;

// AngelScript functions
void asMessageCallback(const asSMessageInfo *msg, void *param);
int  asCompileModule(string name);
void asRecompile();
void asExec(string &msg);
void asExit();

// Script engine
extern ScriptEngine *scriptEngine;

// Register global function
#define AS_GLOBAL_FUNCTION(wrapper, function) \
	r = scriptEngine->RegisterGlobalFunction(wrapper, asFUNCTION(function), asCALL_CDECL); \
	assert( r >= 0 );

#define AS_SINGLETON_FUNCTION(classname, wrapper, method, singleton) \
	r = scriptEngine->RegisterGlobalFunction(wrapper, asMETHOD(classname, method), asCALL_THISCALL_ASGLOBAL, singleton); \
	assert( r >= 0 );

#define AS_SINGLETON_FUNCTIONPR(classname, wrapper, method, params, ret, singleton) \
	r = scriptEngine->RegisterGlobalFunction(wrapper, asMETHODPR(classname, method, params, ret), asCALL_THISCALL_ASGLOBAL, singleton); \
	assert( r >= 0 );

#define AS_GLOBAL_FUNCTIONPR(wrapper, function, params, ret) \
	r = scriptEngine->RegisterGlobalFunction(wrapper, asFUNCTIONPR(function, params, ret), asCALL_CDECL); \
	assert( r >= 0 );

// Register global property
#define AS_GLOBAL_PROPERTY(wrapper, pointer) \
	r = scriptEngine->RegisterGlobalProperty(wrapper, pointer); \
	assert( r >= 0 );

// Register enum
#define AS_ENUM_REGISTER(enumname) \
	r = scriptEngine->RegisterEnum(enumname); \
	assert( r >= 0 );

#define AS_ENUM_VALUE(enumname, name, val) \
	r = scriptEngine->RegisterEnumValue(enumname, name, val); \
	assert( r >= 0 );

// Call object method by id (faster)
#define AS_OBJECT_CALL_ID(obj, id) \
	ScriptContext *ctx = scriptEngine->CreateContext(); \
	/*if(X2DEngine::GameDebugger) ctx->SetLineCallback(asMETHOD(Debugger, lineCallback), X2DEngine::GameDebugger, asCALL_THISCALL);*/ \
	ctx->Prepare(id); \
	ctx->SetObject(obj); \
	r = ctx->Execute(); assert( r >= 0 ); \
	ctx->Release();

// Start object call (for using arguments)
#define AS_OBJECT_CALL_START(obj, id) \
	asIScriptContext *ctx = scriptEngine->CreateContext(); \
	if(id < 0) return; \
	/*if(X2DEngine::GameDebugger) ctx->SetLineCallback(asMETHOD(Debugger, lineCallback), X2DEngine::GameDebugger, asCALL_THISCALL);*/ \
	ctx->Prepare(id); \
	ctx->SetObject(obj);

// End object call
#define AS_OBJECT_CALL_END() \
	r = ctx->Execute(); assert( r >= 0 ); \
	ctx->Release();

// Register class property
#define AS_CLASS_PROPERTY(classname, wrapper, classprop) \
	r = scriptEngine->RegisterObjectProperty("C" #classname, wrapper, offsetof(classname, classprop)); \
	assert( r >= 0 );

// Register class method
#define AS_CLASS_METHOD(classname, wrapper, method) \
	r = scriptEngine->RegisterObjectMethod("C" #classname, wrapper, asMETHOD(classname, method), asCALL_THISCALL); \
	assert( r >= 0 );
#define AS_CLASS_METHODPR(classname, wrapper, method, params, ret) \
	r = scriptEngine->RegisterObjectMethod("C" #classname, wrapper, asMETHODPR(classname, method, params, ret), asCALL_THISCALL); \
	assert( r >= 0 );

// Register class behaviour
#define AS_CLASS_BEHAVIOURM(classname, wrapper, beh, func) \
	r = scriptEngine->RegisterObjectBehaviour("C" #classname, beh, wrapper, asMETHOD(classname, func), asCALL_THISCALL); \
	assert( r >= 0 );
#define AS_CLASS_BEHAVIOURF(classname, wrapper, beh, func) \
	r = scriptEngine->RegisterObjectBehaviour("C" #classname, beh, wrapper, asFUNCTION(classname::func), asCALL_CDECL); \
	assert( r >= 0 );
#define AS_CLASS_BEHAVIOURO(classname, wrapper, beh, func) \
	r = scriptEngine->RegisterObjectBehaviour("C" #classname, beh, wrapper, asFUNCTION(classname::func), asCALL_CDECL_OBJLAST); \
	assert( r >= 0 );

// Register value types
#define AS_REGISTER_VALUE(classname) \
	r = scriptEngine->RegisterObjectType(#classname, sizeof(classname), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CAK); assert( r >= 0 );

// Register value property
#define AS_VALUE_PROPERTY(classname, wrapper, classprop) \
	r = scriptEngine->RegisterObjectProperty(#classname, wrapper, offsetof(classname, classprop)); \
	assert( r >= 0 );

// Register value behaviour
#define AS_VALUE_BEHAVIOURM(classname, wrapper, beh, func) \
	r = scriptEngine->RegisterObjectBehaviour(#classname, beh, wrapper, asMETHOD(classname, func), asCALL_THISCALL); \
	assert( r >= 0 );
#define AS_VALUE_BEHAVIOURF(classname, wrapper, beh, func) \
	r = scriptEngine->RegisterObjectBehaviour(#classname, beh, wrapper, asFUNCTION(classname::func), asCALL_CDECL); \
	assert( r >= 0 );
#define AS_VALUE_BEHAVIOURO(classname, wrapper, beh, func) \
	r = scriptEngine->RegisterObjectBehaviour(#classname, beh, wrapper, asFUNCTION(classname::func), asCALL_CDECL_OBJLAST); \
	assert( r >= 0 );

// Register value method
#define AS_VALUE_METHOD(classname, wrapper, method) \
	r = scriptEngine->RegisterObjectMethod(#classname, wrapper, asMETHOD(classname, method), asCALL_THISCALL); \
	assert( r >= 0 );
#define AS_VALUE_METHODPR(classname, wrapper, method, params, ret) \
	r = scriptEngine->RegisterObjectMethod(#classname, wrapper, asMETHODPR(classname, method, params, ret), asCALL_THISCALL); \
	assert( r >= 0 );

// Register template types
#define AS_REGISTER_TEMPLATE(classname) \
	r = scriptEngine->RegisterObjectType(classname "<class T>", 0, asOBJ_REF | asOBJ_GC | asOBJ_TEMPLATE);

// Register template property
#define AS_TEMPLATE_PROPERTY(name, wrapper, classprop) \
	r = scriptEngine->RegisterObjectProperty(#name "<T>", wrapper, offsetof(classname, classprop)); \
	assert( r >= 0 );

// Register template behaviour
#define AS_TEMPLATE_BEHAVIOURM(classname, name, wrapper, beh, func) \
	r = scriptEngine->RegisterObjectBehaviour(name "<T>", beh, wrapper, asMETHOD(classname, func), asCALL_THISCALL); \
	assert( r >= 0 );
#define AS_TEMPLATE_BEHAVIOURF(name, wrapper, beh, func) \
	r = scriptEngine->RegisterObjectBehaviour(name "<T>", beh, wrapper, asFUNCTION(func), asCALL_CDECL); \
	assert( r >= 0 );
#define AS_TEMPLATE_BEHAVIOURO(name, wrapper, beh, func) \
	r = scriptEngine->RegisterObjectBehaviour(name "<T>", beh, wrapper, asFUNCTION(func), asCALL_CDECL_OBJLAST); \
	assert( r >= 0 );

// Register template method
#define AS_TEMPLATE_METHOD(classname, name, wrapper, method) \
	r = scriptEngine->RegisterObjectMethod(name "<T>", wrapper, asMETHOD(classname, method), asCALL_THISCALL); \
	assert( r >= 0 );
#define AS_TEMPLATE_METHODPR(classname, name, wrapper, method, params, ret) \
	r = scriptEngine->RegisterObjectMethod(name "<T>", wrapper, asMETHODPR(classname, method, params, ret), asCALL_THISCALL); \
	assert( r >= 0 );

struct ScriptArgument {
	ScriptArgument();
	~ScriptArgument();

	void set(void *value, int typeId);
	void clear();

	bool valid;
	int typeId;
	void *value;
};

class ScriptManager
{
public:
	ScriptManager();
	~ScriptManager();
	
	ScriptContext *createScriptContext() const;

	// Script module
	void setModule(ScriptModule *mod);
	ScriptModule *module();

	// Script classes
	uint classCount() const;
	int classIdByName(const string className) const;
	string classNameById(const uint idx) const;
	bool isClassName(const string className) const;
	bool classDerivesFromName(const string class1, const string class2) const;
	bool classDerivesFromId(const uint id1, const uint id2) const;

	// Script objects
	void setCreateArg(const uint idx, void *object, int typeId);
	void resizeCreateArgs(const uint size);
	void setCreateObjectByName(const string name);
	void setCreateObjectById(const uint id);
	void createObject(void *object, int typeId, const string decl = "") const;
	string objectClassName(void *obj, int typeId) const;

private:
	ScriptModule *m_module;
	ScriptType *m_createObjectType;
	vector<ScriptArgument> m_createArgs;
};

#else

#include <stddef.h>

#define asFUNCTION(f) asFunctionPtr(f)

//-----------------------------------------------------------------
// Function pointers

class asCUnknownClass;
typedef void (asCUnknownClass::*asMETHOD_t)();
typedef void (*asFUNCTION_t)();

struct asSFuncPtr
{
	asSFuncPtr(uchar f)
	{
		for( size_t n = 0; n < sizeof(ptr.dummy); n++ )
			ptr.dummy[n] = 0;
		flag = f;
	}

	void CopyMethodPtr(const void *mthdPtr, size_t size)
	{
		for( size_t n = 0; n < size; n++ )
			ptr.dummy[n] = reinterpret_cast<const char *>(mthdPtr)[n];
	}

	union
	{
		// The largest known method point is 20 bytes (MSVC 64bit),
		// but with 8byte alignment this becomes 24 bytes. So we need
		// to be able to store at least that much.
		char dummy[25]; 
		struct {asMETHOD_t   mthd; char dummy[25-sizeof(asMETHOD_t)];} m;
		struct {asFUNCTION_t func; char dummy[25-sizeof(asFUNCTION_t)];} f;
	} ptr;
	uchar flag; // 1 = generic, 2 = global func, 3 = method
};

// Template function to capture all global functions,
// except the ones using the generic calling convention
template <class T>
inline asSFuncPtr asFunctionPtr(T func)
{
	// Mark this as a global function
	asSFuncPtr p(2);

	// MSVC6 doesn't like the size_t cast above so I
	// solved this with a separate code for 32bit.
	p.ptr.f.func = reinterpret_cast<asFUNCTION_t>(func);

	return p;
}

#endif


class X2DAPI X2DScript
{
public:

	// Object type ids
	int getObjectTypeId(const char *objectName);

	// Global registering
	int registerGlobalFunction(const char* funcDef, const asSFuncPtr &funcPtr);
	int registerGlobalVariable(const char* varDef, void *varPtr, int typeId);
	int registerEnum(const char* enumName);
	int registerEnumValue(const char* enumName, const char *valName, const int value);

	// Object registering
	int registerObject(const char* className);
	int registerObjectMethod(const char* className, const char* funcDef, const asSFuncPtr &funcPrt);
	int registerObjectProptery(const char* className, const char* propDef, void *propPrt);
	int registerObjectBehavour(const char* className, const char* type);

	// Function handles
	void *getFunctionHandle(const char* decl);

	// Reloading
	void reload();
};

// Registering
X2DAPI int getObjectTypeId(const char *objectName);
X2DAPI int registerGlobalFunction(const char *funcDef, const asSFuncPtr &funcPointer);
X2DAPI int registerFunctionDef(const char *decl);
X2DAPI int releaseScriptObject(void *obj);
X2DAPI int releaseScriptFunc(void *func);
X2DAPI int startScriptFuncCall(void *func);
X2DAPI int addScriptFuncArg(void *value, int typeId);
X2DAPI int endScriptFuncCall();
X2DAPI int registerEnum(const char *name);
X2DAPI int registerEnumValue(const char *enumname, const char *valuename, int value);
X2DAPI void *getScriptFuncHandle(const char *decl);

#endif // CORE_SCRITPS_H