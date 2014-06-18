#ifndef SCRIPTFUNCCALL_H
#define SCRIPTFUNCCALL_H

#ifndef ANGELSCRIPT_H 
// Avoid having to inform include path if header is already include before
#include <angelscript.h>
#endif

#include <x2d/scripts/funccall.h>
#include <x2d/scriptengine.h>

BEGIN_AS_NAMESPACE

struct SArrayBuffer;
struct SArrayCache;

class CScriptFuncCall : public FunctionCall
{
public:
	CScriptFuncCall(asIScriptEngine *scriptEngine);

	int Prepare(void *function);
	void SetObject(void *value, int typeId);
	void SetArgument(int index, void *value, int typeId);
	int Execute();

protected:
	asIScriptEngine *m_scriptEngine;
	asIScriptContext *m_context;
	vector<ScriptArgument> m_arguments;
	ScriptArgument *m_object;
};

void RegisterScriptArray(asIScriptEngine *engine, bool defaultArray);

END_AS_NAMESPACE

#endif