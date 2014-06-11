#ifndef X2D_SCRIPTS_H
#define X2D_SCRIPTS_H

#include <x2d/config.h>
#include <x2d/util.h>
#include <x2d/base.h>

#ifdef USING_AS

// AngelScript header
#include <angelscript.h>

struct ScriptArgument {
	ScriptArgument();
	~ScriptArgument();

	void set(void *value, int typeId);
	void clear();

	bool valid;
	int typeId;
	void *value;
};

class xdScripts
{
	friend class xdEngine;
public:
	AS_DECL_SINGLETON

	xdScripts(asIScriptEngine *scriptEngine, class xdDebug *debugger);
	~xdScripts();

	// Script module
	asIScriptModule *getModule() const;
	
	// Script context
	asIScriptContext *createContext() const;

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

	// 
	void executeString(const string &str) const;

private:
	asIScriptModule *m_module;
	asIObjectType *m_createObjectType;
	vector<ScriptArgument> m_createArgs;
	class xdDebug *m_debugger;
};

// AngelScript functions
void asMessageCallback(const asSMessageInfo *msg, void *param);
int  asCompileModule(const string &name, class xdFileSystem *fileSystem);

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

// Global registering
XDAPI int getObjectTypeId(const char *objectName);
XDAPI int registerGlobalFunction(const char *funcDef, const asSFuncPtr &funcPointer);
XDAPI int registerFunctionDef(const char *decl);
XDAPI int releaseScriptObject(void *obj);
XDAPI int releaseScriptFunc(void *func);
XDAPI int startScriptFuncCall(void *func);
XDAPI int addScriptFuncArg(void *value, int typeId);
XDAPI int endScriptFuncCall();
XDAPI int registerEnum(const char *name);
XDAPI int registerEnumValue(const char *enumname, const char *valuename, int value);
XDAPI void *getScriptFuncHandle(const char *decl);

#endif // X2D_SCRIPTS_H