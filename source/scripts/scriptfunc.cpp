#include "scriptfunc.h"

CScriptFuncCall::CScriptFuncCall(asIScriptEngine *scriptEngine) :
	m_scriptEngine(scriptEngine),
	m_context(0),
	m_object(0)
{
}

int CScriptFuncCall::Prepare(void *function)
{
	asIScriptFunction *func = (asIScriptFunction*)function;
	m_context = m_scriptEngine->CreateContext();
	m_arguments.resize(func->GetParamCount());
	return m_context->Prepare(func);
}

void CScriptFuncCall::SetObject(void *value, int typeId)
{
	if(m_object == 0) {
		m_object  = new ScriptValue;
	}
	m_object->set(value, typeId);
}

void CScriptFuncCall::SetArgument(int index, void *value, int typeId)
{
	m_arguments[index].set(value, typeId);
}

int CScriptFuncCall::Execute()
{
	// Set script call arguments
	for(uint i = 0; i < m_arguments.size(); i++)
	{
		// Make sure all the argument is defined
		const ScriptValue &arg = m_arguments[i];
		if(!arg.valid) {
			asIScriptContext *ctx = asGetActiveContext();
			if(ctx) ctx->SetException("Invalid argument");
			return -2;
		}

		// Set argument
		if((arg.typeId & ~asTYPEID_MASK_SEQNBR) && !(arg.typeId & asTYPEID_OBJHANDLE)) // Script object type
			m_context->SetArgObject(i, arg.value);
		else if(arg.typeId & asTYPEID_OBJHANDLE) // Handle type
		{
			m_scriptEngine->AddRefScriptObject(arg.value, m_scriptEngine->GetObjectTypeById(arg.typeId));
			m_context->SetArgAddress(i, arg.value);
		}else if(arg.typeId == asTYPEID_BOOL || // 8-bit types
				arg.typeId == asTYPEID_INT8 ||
				arg.typeId == asTYPEID_UINT8)
			m_context->SetArgByte(i, *(char*)arg.value);
		else if(arg.typeId == asTYPEID_INT16 || // 16-bit types
				arg.typeId == asTYPEID_UINT16)
			m_context->SetArgWord(i, *(short*)arg.value);
		else if(arg.typeId == asTYPEID_INT32 || // 32-bit types
				arg.typeId == asTYPEID_UINT32 ||
				arg.typeId == asTYPEID_FLOAT ||
				arg.typeId > asTYPEID_DOUBLE) // enums have a type id larger than doubles
			m_context->SetArgDWord(i, *(int*)arg.value);
		else if(arg.typeId == asTYPEID_INT64 || // 64-bit types
				arg.typeId == asTYPEID_UINT64 ||
				arg.typeId == asTYPEID_DOUBLE)
			m_context->SetArgQWord(i, *(asQWORD*)arg.value);
	}

	// Set this object
	if(m_object) {
		m_context->SetObject(m_object->value);
	}

	// Execute script func
	int r = 0;
	r = m_context->Execute(); AS_ASSERT
	r = m_context->Release(); AS_ASSERT

	delete m_object;
	m_object = 0;

	m_context = 0;
	m_arguments.clear();
	return r;
}