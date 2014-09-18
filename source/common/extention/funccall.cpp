//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/extention.h>

AS_REG_REF(XFuncCall, "FuncCall")

int XFuncCall::Register(asIScriptEngine *scriptEngine)
{
	int r;
	r = scriptEngine->RegisterObjectBehaviour("FuncCall", asBEHAVE_FACTORY, "FuncCall @f()", asFUNCTIONPR(XFuncCall::Factory, (), XFuncCall*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("FuncCall", asBEHAVE_FACTORY, "FuncCall @f(const string &in)", asFUNCTIONPR(XFuncCall::Factory, (const string&), XFuncCall*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("FuncCall", asBEHAVE_FACTORY, "FuncCall @f(?&in, const string &in)", asFUNCTIONPR(XFuncCall::Factory, (void*, int, const string&), XFuncCall*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("FuncCall", asBEHAVE_FACTORY, "FuncCall @f(?&in)", asFUNCTIONPR(XFuncCall::Factory, (void*, int), XFuncCall*), asCALL_CDECL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("FuncCall", "void setObject(?&in)", asMETHOD(XFuncCall, setObject), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("FuncCall", "void setArg(const int, ?&in)", asMETHOD(XFuncCall, setArg), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("FuncCall", "bool execute()", asMETHODPR(XFuncCall, execute, (), bool), asCALL_THISCALL); AS_ASSERT
	//r = scriptEngine->RegisterObjectMethod("funccall", "void getReturn(?&out) const", asMETHOD(XFuncCall, getReturn), asCALL_THISCALL); AS_ASSERT
	return r;
}

XFuncCall::XFuncCall()
{
	init(0, 0, 0);
}

/*XFuncCall::XFuncCall(const XFuncCall &other)
{
	// Set function
	m_function = other.m_function;

	// Set object
	if(other.m_object.second & asTYPEID_OBJHANDLE)
	{
		setObject((void*)&other.m_object.first, other.m_object.second);
	}
	else
	{
		setObject(other.m_object.first, other.m_object.second);
	}

	// Set arguments
	m_arguments.resize(other.m_arguments.size());
	for(uint i = 0; i < other.m_arguments.size(); i++)
	{
		int typeId = other.m_arguments[i].second;
		if(typeId & asTYPEID_OBJHANDLE)
		{
			setArg(i, (void*)&other.m_arguments[i].first, typeId);
		}
		else
		{
			setArg(i, other.m_arguments[i].first, typeId);
		}
	}
}*/

XFuncCall::XFuncCall(asIScriptFunction *func)
{
	init(0, 0, func);
}

XFuncCall::XFuncCall(const string &func)
{
	asIScriptModule *mod = XScriptEngine::GetModule();
	asIScriptFunction *function = mod->GetFunctionByDecl(func.c_str());
	if(!function) function = mod->GetFunctionByName(func.c_str());
	init(0, 0, function);
}

XFuncCall::XFuncCall(void *object, int typeId, const string &method)
{
	asIScriptFunction *func = 0;
	if(typeId & asTYPEID_MASK_OBJECT)
	{
		asIObjectType *type = XScriptEngine::GetAngelScript()->GetObjectTypeById(typeId);
		func = type->GetMethodByDecl(method.c_str());
		if(!func) func = type->GetMethodByName(method.c_str());
	}
	init(object, typeId, func);
}

XFuncCall::~XFuncCall()
{
	// Make sure we have a valid funcion
	if(!m_function) {
		return;
	}

	// Release handles
	for(pair<void*, int> &arg : m_arguments)
	{
		if(arg.second & asTYPEID_OBJHANDLE)
		{
			XScriptEngine::GetAngelScript()->ReleaseScriptObject(arg.first, XScriptEngine::GetAngelScript()->GetObjectTypeById(arg.second)); 
		}
	}
	
	if(m_object.second & asTYPEID_OBJHANDLE)
	{
		XScriptEngine::GetAngelScript()->ReleaseScriptObject(m_object.first, XScriptEngine::GetAngelScript()->GetObjectTypeById(m_object.second));
	}
}

void XFuncCall::init(void *object, int typeId, asIScriptFunction *func)
{
	setObject(object, typeId);
	m_function = func;
	if(m_function)
	{
		m_arguments.resize(func->GetParamCount());
		for(pair<void*, int> &arg : m_arguments)
		{
			arg.first = 0;
			arg.second = 0;
		}
	}
}

void XFuncCall::setObject(void *object, int typeId)
{
	if(typeId & asTYPEID_MASK_OBJECT)
	{
		if(typeId & asTYPEID_OBJHANDLE)
		{
			m_object.first = *(void**)object;
			
			// Add handle ref
			XScriptEngine::GetAngelScript()->AddRefScriptObject(*(void**)object, XScriptEngine::GetAngelScript()->GetObjectTypeById(typeId));
		}
		else
		{
			m_object.first = object;
		}
		m_object.second = typeId;
	}
}

void XFuncCall::setArg(const uint index, void *value, int typeId)
{
	// Make sure we have a valid funcion
	if(!m_function)
		return;
	
	if(index >= 0 && index < m_function->GetParamCount())
	{
		int paramTypeId;
		m_function->GetParam(index, &paramTypeId);
		if(typeId == paramTypeId || XScriptEngine::GetAngelScript()->IsHandleCompatibleWithObject(value, typeId, paramTypeId))
		{
			pair<void*, int> &arg = m_arguments[index];
			if(typeId & asTYPEID_OBJHANDLE)
			{
				arg.first = *(void**)value;

				// Add handle ref
				XScriptEngine::GetAngelScript()->AddRefScriptObject(*(void**)value, XScriptEngine::GetAngelScript()->GetObjectTypeById(typeId));
			}
			else
			{
				arg.first = value;
			}
			arg.second = typeId;
		}
		else
		{
			LOG("XFuncCall::setArg() - Unexpected argument type");
		}
	}
	else
	{
		LOG("XFuncCall::setArg() - Argument index out of bounds");
	}
}

bool XFuncCall::execute(asIScriptContext *ctx)
{
	// Make sure we have a context
	if(!ctx)
	{
		return false;
	}

	// Make sure we have a valid funcion
	if(!m_function)
	{
		ctx->Release();
		return false;
	}

	// Prepare context
	int r = ctx->Prepare(m_function); assert(r >= 0);
	if(m_object.first != 0 && m_object.second != 0)
	{
		r = ctx->SetObject(m_object.first); assert(r >= 0);
	}

	// Set arguments
	for(uint i = 0; i < m_function->GetParamCount(); i++)
	{
		pair<void*, int> &arg = m_arguments[i];

		// Make sure argument is valid
		if(arg.first == 0 && arg.second == 0)
		{
			LOG("XFuncCall::execute() - Argument #%i not set", i);
			return false;
		}

		if(arg.second & asTYPEID_OBJHANDLE) // Handle type
		{
			r = ctx->SetArgAddress(i, arg.first); assert(r >= 0);
		}
		else if(arg.second & asTYPEID_MASK_OBJECT) // Script object type
		{
			r = ctx->SetArgObject(i, arg.first); assert(r >= 0);
		}
		else // Primitive type
		{
			asDWORD flags;
			m_function->GetParam(i, 0, &flags);
			if(flags == asTM_NONE || flags == asTM_CONST)
			{
				if(arg.second == asTYPEID_BOOL || // 8-bit types
						arg.second == asTYPEID_INT8 ||
						arg.second == asTYPEID_UINT8)
				{
					r = ctx->SetArgByte(i, *(char*)arg.first); assert(r >= 0);
				}
				else if(arg.second == asTYPEID_INT16 || // 16-bit types
						arg.second == asTYPEID_UINT16)
				{
					r = ctx->SetArgWord(i, *(short*)arg.first); assert(r >= 0);
				}
				else if(arg.second == asTYPEID_INT32 || // 32-bit types
						arg.second == asTYPEID_UINT32 ||
						arg.second == asTYPEID_FLOAT ||
						arg.second > asTYPEID_DOUBLE) // enums have a type id larger than doubles
				{
					r = ctx->SetArgDWord(i, *(int*)arg.first); assert(r >= 0);
				}
				else if(arg.second == asTYPEID_INT64 || // 64-bit types
						arg.second == asTYPEID_UINT64 ||
						arg.second == asTYPEID_DOUBLE)
				{
					r = ctx->SetArgQWord(i, *(asQWORD*)arg.first); assert(r >= 0);
				}
			}
			else if(flags & asTM_INREF)
			{
				r = ctx->SetArgAddress(i, arg.first); assert(r >= 0);
			}
			else
			{
				LOG("Currently unsupported arg type");
			}
		}
	}

	// Execute script func
	int executeResult = r = ctx->Execute();
	if(r != asEXECUTION_FINISHED)
	{
		// Print runtime exception message
		const char *sectionName; int line;
		ctx->GetExceptionLineNumber(&line, &sectionName);
		LOG("Runtime exception '%s' occured in function '%s' in file '%s:%i'", ctx->GetExceptionString(), ctx->GetExceptionFunction()->GetDeclaration(), sectionName, line);
	}
	r = ctx->Release();
	return executeResult == asEXECUTION_FINISHED;
}

bool XFuncCall::execute()
{
	return execute(XScriptEngine::CreateContext());
}

/*void XFuncCall::getReturn(void *value, int typeId) const
{
	// Make sure we have a valid funcion
	if(!m_function) {
		return;
	}

	// Get return type
	int retTypeId = m_function->GetReturnTypeId();
	if(typeId == retTypeId || m_engine->IsHandleCompatibleWithObject(value, typeId, retTypeId))
	{
		if(typeId & asTYPEID_OBJHANDLE) // Handle type
		{
			*(void**)value = m_context->GetReturnAddress();
		}
		else if(typeId & asTYPEID_MASK_OBJECT) // Script object type
		{
			m_engine->AssignScriptObject(value, m_context->GetReturnObject(), m_engine->GetObjectTypeById(retTypeId));
		}
		else if(typeId == asTYPEID_BOOL || // 8-bit types
				typeId == asTYPEID_INT8 ||
				typeId == asTYPEID_UINT8)
		{
			*(char*)value = m_context->GetReturnByte();
		}
		else if(typeId == asTYPEID_INT16 || // 16-bit types
				typeId == asTYPEID_UINT16)
		{
			*(short*)value = m_context->GetReturnWord();
		}
		else if(typeId == asTYPEID_INT32 || // 32-bit types
				typeId == asTYPEID_UINT32 ||
				typeId == asTYPEID_FLOAT ||
				typeId > asTYPEID_DOUBLE) // enums have a type id larger than doubles
		{
			*(int*)value = m_context->GetReturnDWord();
		}
		else if(typeId == asTYPEID_INT64 || // 64-bit types
				typeId == asTYPEID_UINT64 ||
				typeId == asTYPEID_DOUBLE)
		{
			*(asQWORD*)value = m_context->GetReturnQWord();
		}
	}
}*/

XFuncCall *XFuncCall::Factory()
{
	return new XFuncCall();
}

XFuncCall *XFuncCall::Factory(const string &func)
{
	return new XFuncCall(func);
}

XFuncCall *XFuncCall::Factory(void *value, int typeId, const string &method)
{
	return new XFuncCall(value, typeId, method);
}

XFuncCall *XFuncCall::Factory(void *value, int typeId)
{
	asIScriptFunction *func = 0;
	if(typeId & asTYPEID_MASK_OBJECT && typeId & asTYPEID_APPOBJECT)
	{
		string name = XScriptEngine::GetAngelScript()->GetObjectTypeById(typeId)->GetName();
		if(name == "_builtin_function_")
		{
			func = *(asIScriptFunction**)value;
		}
	}
	return new XFuncCall(func);
}