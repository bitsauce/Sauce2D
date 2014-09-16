#ifndef SCRIPTS_FUNCCALL_H
#define SCRIPTS_FUNCCALL_H

#include "common/engine.h"

class XDAPI FuncCall
{
public:
	FuncCall();
	FuncCall(const FuncCall &other);
	FuncCall(asIScriptFunction *func);								// Function/delegate object
	FuncCall(const string &func);									// Function call by name
	FuncCall(void *object, int typeId, const string &method);		// Method call by name
	~FuncCall();

	void setObject(void *object, int typeId);
	void setArg(const uint index, void *value, int typeId);
	bool execute();
	//void getReturn(void *value, int typeId) const;
	
private:
	void init(void *object, int typeId, asIScriptFunction *func);

protected:
	pair<void*, int> m_object;
	vector<pair<void*, int>> m_arguments;
	asIScriptFunction *m_function;
};

int RegisterScriptFuncCall(asIScriptEngine *scriptEngine);

#endif // SCRIPTS_FUNCCALL_H