#ifndef X2D_SCRIPTS_H
#define X2D_SCRIPTS_H

#include <x2d/config.h>
#include <x2d/util.h>
#include <x2d/base.h>

// Include AngelScript header
#ifdef USING_AS
#include <angelscript.h>
#else
#include "faux_angelscript.h"
#endif // USING_AS

// Script func-call argument
struct ScriptArgument
{
	ScriptArgument();
	~ScriptArgument();

	void set(void *value, int typeId);
	void clear();

	bool valid;
	int typeId;
	void *value;
};

// A asIScriptEngine wrapper (mainly used by external plugins)
class XDAPI xdScriptEngine
{
	friend class xdEngine;
public:
	AS_DECL_SINGLETON

	xdScriptEngine(asIScriptEngine *scriptEngine, class xdDebug *debugger);
	~xdScriptEngine();

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

	// Registers
	int registerSingletonType(const char *obj);
	int registerRefType(const char *obj, const asSFuncPtr &addRef, const asSFuncPtr &release);
	int registerValueType(const char *obj, int size);
	int registerObjectFactory(const char *obj, const char *decl, const asSFuncPtr &func);
	int registerObjectConstructor(const char *obj, const char *decl, const asSFuncPtr &func);
	int registerObjectMethod(const char *obj, const char *decl, const asSFuncPtr &funcPointer); 
	int registerObjectProperty(const char *obj, const char *decl, const int offset); 
	int registerGlobalProperty(const char *decl, void *pointer);
	int registerEnum(const char *decl);
	int registerEnumValue(const char *enumname, const char *valuename, int value);
	int registerFuncdef(const char *decl);

private:
	asIScriptModule *m_module;
	asIObjectType *m_createObjectType;
	vector<ScriptArgument> m_createArgs;
	class xdDebug *m_debugger;
};

#ifdef USING_AS

// AngelScript functions
void asMessageCallback(const asSMessageInfo *msg, void *param);
int  asCompileModule(const string &name, class xdFileSystem *fileSystem);

#endif // USING_AS


#endif // X2D_SCRIPTS_H