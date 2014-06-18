#ifndef X2D_FUNC_CALL_H
#define X2D_FUNC_CALL_H

#include <x2d/config.h>
#include <x2d/util.h>

class XDAPI FunctionCall
{
public:
	virtual int Prepare(void *function) = 0;
	virtual void SetObject(void *value, int typeId) = 0;
	virtual void SetArgument(int index, void *value, int typeId) = 0;
	virtual int Execute() = 0;
};

XDAPI FunctionCall *CreateFuncCall();

#endif // X2D_FUNC_CALL_H