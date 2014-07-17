#include "event.h"
#include "animation.h"
#include <x2d/scripts/funccall.h>

spEventWrapper::spEventWrapper(spEvent *event, spEventType type, int loopCount) :
	m_self(event),
	m_type(type),
	m_loopCount(loopCount)
{
}

string spEventWrapper::getString() const
{
	return m_self != 0 ? m_self->data->stringValue : "";
}

int spEventWrapper::getInt() const
{
	return m_self != 0 ? m_self->data->intValue : 0;
}

float spEventWrapper::getFloat() const
{
	return m_self != 0 ? m_self->data->floatValue : 0.0f;
}

spEventType spEventWrapper::getType() const
{
	return m_type;
}

int spEventWrapper::getLoopCount() const
{
	return m_loopCount;
}

void spEventWrapper::call(void *func)
{
	if(func)
	{
		FunctionCall *funcCall = CreateFuncCall();
		funcCall->Prepare(func);
		spEventWrapper *self = this;
		addRef();
		funcCall->SetArgument(0, &self, 0x40000000);
		funcCall->Execute();
		delete funcCall;
	}
}