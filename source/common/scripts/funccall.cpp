#include <x2d/scripts/funccall.h>

FuncCall::FuncCall()
{
	init(0, 0, 0);
}

FuncCall::FuncCall(const FuncCall &other)
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
}

FuncCall::FuncCall(asIScriptFunction *func)
{
	init(0, 0, func);
}

FuncCall::FuncCall(const string &func)
{
	asIScriptModule *mod = g_engine->getScriptEngine()->getModule();
	asIScriptFunction *function = mod->GetFunctionByDecl(func.c_str());
	if(!function) function = mod->GetFunctionByName(func.c_str());
	init(0, 0, function);
}

FuncCall::FuncCall(void *object, int typeId, const string &method)
{
	asIScriptFunction *func = 0;
	if(typeId & asTYPEID_MASK_OBJECT)
	{
		asIObjectType *type = g_engine->getScriptEngine()->getASEngine()->GetObjectTypeById(typeId);
		func = type->GetMethodByDecl(method.c_str());
		if(!func) func = type->GetMethodByName(method.c_str());
	}
	init(object, typeId, func);
}

FuncCall::~FuncCall()
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
			asIScriptEngine *scriptEngine = g_engine->getScriptEngine()->getASEngine();
			scriptEngine->ReleaseScriptObject(arg.first, scriptEngine->GetObjectTypeById(arg.second)); 
		}
	}
	
	if(m_object.second & asTYPEID_OBJHANDLE)
	{
		asIScriptEngine *scriptEngine = g_engine->getScriptEngine()->getASEngine();
		scriptEngine->ReleaseScriptObject(m_object.first, scriptEngine->GetObjectTypeById(m_object.second));
	}
}

void FuncCall::init(void *object, int typeId, asIScriptFunction *func)
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

void FuncCall::setObject(void *object, int typeId)
{
	if(typeId & asTYPEID_MASK_OBJECT)
	{
		if(typeId & asTYPEID_OBJHANDLE)
		{
			m_object.first = *(void**)object;
			
			// Add handle ref
			asIScriptEngine *scriptEngine = g_engine->getScriptEngine()->getASEngine();
			scriptEngine->AddRefScriptObject(*(void**)object, scriptEngine->GetObjectTypeById(typeId));
		}
		else
		{
			m_object.first = object;
		}
		m_object.second = typeId;
	}
}

void FuncCall::setArg(const uint index, void *value, int typeId)
{
	// Make sure we have a valid funcion
	if(!m_function)
		return;
	
	if(index >= 0 && index < m_function->GetParamCount())
	{
		int paramTypeId;
		m_function->GetParam(index, &paramTypeId);
		if(typeId == paramTypeId || g_engine->getScriptEngine()->getASEngine()->IsHandleCompatibleWithObject(value, typeId, paramTypeId))
		{
			pair<void*, int> &arg = m_arguments[index];
			if(typeId & asTYPEID_OBJHANDLE)
			{
				arg.first = *(void**)value;

				// Add handle ref
				asIScriptEngine *scriptEngine = g_engine->getScriptEngine()->getASEngine();
				scriptEngine->AddRefScriptObject(*(void**)value, scriptEngine->GetObjectTypeById(typeId));
			}
			else
			{
				arg.first = value;
			}
			arg.second = typeId;
		}
		else
		{
			LOG("funccall::setArg() - Unexpected argument type");
		}
	}
	else
	{
		LOG("funccall::setArg() - Argument index out of bounds");
	}
}

bool FuncCall::execute()
{
	// Make sure we have a valid funcion
	if(!m_function)
		return false;

	int r;

	// Prepare context
	asIScriptContext *ctx = g_engine->getScriptEngine()->createContext();
	r = ctx->Prepare(m_function); assert(r >= 0);
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
			LOG("funccall::execute() - Argument #%i not set", i);
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

/*void FuncCall::getReturn(void *value, int typeId) const
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

void ConstructDefault(FuncCall *self)
{
	new (self) FuncCall();
}

void CopyConstructFuncCall(const FuncCall &other, FuncCall *self)
{
	new (self) FuncCall(other);
}

void ConstructFunction(const string &func, FuncCall *self)
{
	new (self) FuncCall(func);
}

void ConstructMethod(void *value, int typeId, const string &method, FuncCall *self)
{
	new (self) FuncCall(value, typeId, method);
}

void ConstructDelegate(void *value, int typeId, FuncCall *self)
{
	asIScriptFunction *func = 0;
	if(typeId & asTYPEID_MASK_OBJECT && typeId & asTYPEID_APPOBJECT)
	{
		string name = g_engine->getScriptEngine()->getASEngine()->GetObjectTypeById(typeId)->GetName();
		if(name == "_builtin_function_")
		{
			func = *(asIScriptFunction**)value;
		}
	}
	new (self) FuncCall(func);
}

void Destruct(FuncCall *self)
{
	self->~FuncCall();
}

int RegisterScriptFuncCall(asIScriptEngine *scriptEngine)
{
	int r;

	r = scriptEngine->RegisterObjectType("funccall", sizeof(FuncCall), asOBJ_VALUE | asOBJ_APP_CLASS_CDK); AS_ASSERT
		
	r = scriptEngine->RegisterObjectBehaviour("funccall", asBEHAVE_CONSTRUCT,  "void f()", asFUNCTION(ConstructDefault), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("funccall", asBEHAVE_CONSTRUCT,  "void f(const funccall &in)", asFUNCTION(CopyConstructFuncCall), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("funccall", asBEHAVE_CONSTRUCT,  "void f(const string &in)", asFUNCTION(ConstructFunction), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("funccall", asBEHAVE_CONSTRUCT,  "void f(?&in, const string &in)", asFUNCTION(ConstructMethod), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("funccall", asBEHAVE_CONSTRUCT,  "void f(?&in)", asFUNCTION(ConstructDelegate), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("funccall", asBEHAVE_DESTRUCT,  "void f()", asFUNCTION(Destruct), asCALL_CDECL_OBJLAST); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("funccall", "void setObject(?&in)", asMETHOD(FuncCall, setObject), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("funccall", "void setArg(const int, ?&in)", asMETHOD(FuncCall, setArg), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("funccall", "bool execute()", asMETHOD(FuncCall, execute), asCALL_THISCALL); AS_ASSERT
	//r = scriptEngine->RegisterObjectMethod("funccall", "void getReturn(?&out) const", asMETHOD(FuncCall, getReturn), asCALL_THISCALL); AS_ASSERT

	return r;
}