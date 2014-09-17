#ifndef X2D_FUNCCALL_H
#define X2D_FUNCCALL_H

#include "../engine.h"

class XDAPI XFuncCall
{
public:
	XFuncCall();
	XFuncCall(const XFuncCall &other);
	XFuncCall(asIScriptFunction *func);								// Function/delegate object
	XFuncCall(const string &func);									// Function call by name
	XFuncCall(void *object, int typeId, const string &method);		// Method call by name
	~XFuncCall();

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

#endif // X2D_FUNCCALL_H