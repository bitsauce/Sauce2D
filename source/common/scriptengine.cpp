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

XScriptArray *CreateArray(const string &type, const uint size)
{
	// Create decl
	string decl = "array<";
	decl += type;
	decl += ">";

    // Obtain a pointer to the engine
	asIObjectType *arrayType = XScriptEngine::GetAngelScript()->GetObjectTypeById(XScriptEngine::GetAngelScript()->GetTypeIdByDecl(decl.c_str()));

    // Create the array object
    return new XScriptArray(size, arrayType);
}

asIScriptEngine *XScriptEngine::s_engine = 0;
asIScriptModule *XScriptEngine::s_module = 0;
XEngine *XScriptEngine::s_gameEngine = 0;

AS_REG_SINGLETON(XScriptEngine)

int XScriptEngine::Register(asIScriptEngine *scriptEngine)
{
	int r;

	// Scirpt classes
	r = scriptEngine->RegisterObjectMethod("XScriptEngine", "uint getClassCount() const", asMETHOD(XScriptEngine, classCount), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XScriptEngine", "int getClassIdByName(const string)", asMETHOD(XScriptEngine, classIdByName), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XScriptEngine", "string getClassNameById(const uint)", asMETHOD(XScriptEngine, classNameById), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XScriptEngine", "bool classExists(const string)", asMETHOD(XScriptEngine, isClassName), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XScriptEngine", "bool classDerivesFromName(const string, const string)", asMETHOD(XScriptEngine, classDerivesFromName), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XScriptEngine", "bool classDerivesFromId(const uint, const uint)", asMETHOD(XScriptEngine, classDerivesFromId), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterInterface("Serializable"); AS_ASSERT
	r = scriptEngine->RegisterInterfaceMethod("Serializable", "void serialize(StringStream&)"); AS_ASSERT
	r = scriptEngine->RegisterInterfaceMethod("Serializable", "void deserialize(StringStream&)"); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("XScriptEngine", "void serialize(Serializable@, string &in)", asMETHODPR(XScriptEngine, serialize, (asIScriptObject*, string&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XScriptEngine", "Serializable @deserialize(string &in)", asMETHODPR(XScriptEngine, deserialize, (string&), asIScriptObject*), asCALL_THISCALL); AS_ASSERT

	return r;
}

XScriptEngine::XScriptEngine()
{
}

XScriptEngine::~XScriptEngine()
{
	// Clean up AngelScript
	asIScriptContext* ctx = asGetActiveContext();
	if(ctx) assert("AngelScript: There is an active context running!");
	if(s_engine) assert(s_engine->Release() == 0);
}

asIScriptContext *XScriptEngine::CreateContext()
{
	asIScriptContext *ctx = s_engine->CreateContext();
	if(s_gameEngine->getDebugger())
	{
		ctx->SetLineCallback(asMETHOD(XDebugger, lineCallback), s_gameEngine->getDebugger(), asCALL_THISCALL);
		if(s_gameEngine->getDebugger()->getProfiler()->isEnabled())
		{
			ctx->SetFuncCallback(asMETHOD(XDebugger, funcBeginCallback), asMETHOD(XDebugger, funcEndCallback), s_gameEngine->getDebugger(), asCALL_THISCALL);
		}
	}
	return ctx;
}

asIScriptModule *XScriptEngine::GetModule()
{
	return s_module;
}

asIScriptEngine *XScriptEngine::GetAngelScript()
{
	return s_engine;
}

uint XScriptEngine::classCount() const
{
	return s_module->GetObjectTypeCount();
}

int XScriptEngine::classIdByName(const string name) const
{
	for(uint i = 0; i < s_module->GetObjectTypeCount(); i++) {
		if(name == string(s_module->GetObjectTypeByIndex(i)->GetName()))
			return i;
	}
	return -1;
}

string XScriptEngine::classNameById(const uint id) const
{
	asIObjectType *objType = s_module->GetObjectTypeByIndex(id);
	if(!objType) return ""; // Invalid id
	return objType->GetName();
}

bool XScriptEngine::isClassName(const string name) const
{
	for(uint i = 0; i < s_module->GetObjectTypeCount(); i++) {
		if(name == string(s_module->GetObjectTypeByIndex(i)->GetName()))
			return true;
	}
	return false;
}

bool XScriptEngine::classDerivesFromName(const string name1, const string name2) const
{
	int id1 = classIdByName(name1);
	if(id1 < 0) return false;
	int id2 = classIdByName(name2);
	if(id2 < 0) return false;
	return classDerivesFromId(id1, id2);
}

bool XScriptEngine::classDerivesFromId(const uint id1, const uint id2) const
{
	asIObjectType *objType = s_module->GetObjectTypeByIndex(id1);
	if(objType->DerivesFrom(s_module->GetObjectTypeByIndex(id2)) && id1 != id2) return true;
	return false;
}

string XScriptEngine::objectClassName(void *obj, int typeId) const
{
	// Check if the provied object
	// is a script object
	if(!(typeId & asTYPEID_SCRIPTOBJECT)) {
		asIScriptContext *ctx = asGetActiveContext();
		if(ctx) {
			// Set a script exception
			ctx->SetException("XScriptEngine::objectClassName() should only be called on a script object!");
		}
		return "";
	}

	// Get the provided script object
	asIScriptObject* scriptObject = NULL;
	scriptObject = static_cast<asIScriptObject*>(*(void**)obj); // Reference-to-pointer

	// Return name
	return scriptObject->GetObjectType()->GetName();
}

ScriptValue::ScriptValue() :
	valid(false)
{
}

ScriptValue::~ScriptValue()
{
	clear();
}

void ScriptValue::set(void *value, int typeId)
{
	// Set argument value
	asIScriptEngine *scriptEngine = XScriptEngine::GetAngelScript();
	asIObjectType *objectType = scriptEngine->GetObjectTypeById(typeId);
	if(typeId == asTYPEID_VOID || typeId & asTYPEID_OBJHANDLE)
	{
		// Add handle reference and store address
		scriptEngine->AddRefScriptObject(*(void**)value, objectType);
		this->value = *(void**)value;
	}else if(typeId & asTYPEID_MASK_OBJECT)
	{
		// Construct and copy script object
		this->value = scriptEngine->CreateScriptObject(objectType);
		scriptEngine->AssignScriptObject(this->value, value, objectType);
	}else{
		// Copy primitive value
		int primitiveSize = scriptEngine->GetSizeOfPrimitiveType(typeId);
		this->value = malloc(primitiveSize);
		memcpy(this->value, value, primitiveSize);
	}
	this->valid = true;
	this->typeId = typeId;
}

void ScriptValue::clear()
{
	// Clear the argument data
	asIScriptEngine *scriptEngine = XScriptEngine::GetAngelScript();
	if(valid) {
		if(typeId == asTYPEID_VOID || typeId & asTYPEID_OBJHANDLE)
			scriptEngine->ReleaseScriptObject(value, scriptEngine->GetObjectTypeById(typeId));
		else
			free(value);
	}
	valid = false;
}

//----------------------------------------------------------------------------
// AngelScript
//----------------------------------------------------------------------------

// AngelScript callback function
string messageCallback;
void XConsole::messageCallback(const asSMessageInfo *msg)
{
	// Callback type
	const char *type = "ERR";
	if(msg->type == asMSGTYPE_WARNING) 
		type = "WARN";
	else if(msg->type == asMSGTYPE_INFORMATION) 
		type = "INFO";

	// Print to debug
	char out[512];
#ifdef USE_CTR_SECURE
	sprintf_s(out, "%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message);
#else
	sprintf(out, "%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message);
#endif
	LOG("%s: %s", type, out);
	
	// Store or send message info
	if(m_engine->getDebugger())
	{
		m_engine->getDebugger()->sendPacket(XD_COMPILE_ERROR_PACKET, out);
	}
	else
	{
		::messageCallback += string(out);
	}
}

// Compiles an angelscript module with name
int asCompileModule(const string &name)
{
	asIScriptEngine *scriptEngine = XScriptEngine::GetAngelScript();
	bool error = true;
	int r;
	while(error)
	{
		CScriptBuilder builder;
		r = builder.StartNewModule(scriptEngine, name.c_str()); 
		if(r < 0)
		{
			// If the code fails here it is usually because there is no more memory to allocate the module
			throw XException(X2D_COMPILE_FAILED, "Unable to start a new module. Usually do to lack of memory.");
		}

		r = builder.AddSectionFromFile("main.as");
		if(r < 0)
		{
			// The builder wasn't able to load the file. Maybe the file
			// has been removed, or the wrong name was given, or some
			// preprocessing commands are incorrectly written.
			//if(XEngine::IsEnabled(XD_DEBUG))
			//	XEngine::GetDebugger()->sendPacket(XD_MESSAGE_PACKET, "Unable to load main.as. Make sure the file and other included files exist.");
			//else
				throw XException(X2D_COMPILE_FAILED, "Unable to load main.as. Make sure the file and other included files exist.");
			return X2D_COMPILE_FAILED;
		}

		r = builder.BuildModule();
		if(r < 0)
		{
			/*if(!XEngine::IsEnabled(XD_DEBUGGER))
			{
				// An error occurred. Instruct the script writer to fix the 
				// compilation errors that were listed in the output stream.
				string debug = "AngelScript has encountered the following error:\n\n";
				debug += messageCallback;
				debug += "\nPlease fix the error(s) and press retry, or press cancel to exit the application.";
				messageCallback.clear();

				// TODO: Implement retry
				// Show retry dialog
				throw XException(X2D_COMPILE_FAILED, debug);
			}*/

			return X2D_COMPILE_FAILED;
		}else{
			error = false;
		}
	}
	return 0;
}

// Execute string
void XScriptEngine::executeString(const string &str) const
{
	ExecuteString(s_engine, str.c_str(), s_module);
}

void XScriptEngine::serialize(asIScriptObject *object, string &path)
{
	// Make sure we got a valid object
	if(object)
	{
		// Store object type
		XStringStream ss;
		serialize(object, ss);
		XFileSystem::WriteFile(path, ss.str());

		// Release handle
		object->Release();
	}else
	{
		LOG("Cannot serialize 'null' objects");
	}
}

void XScriptEngine::serialize(asIScriptObject *object, XStringStream &ss)
{
	// Get script object type and it's serialization function
	asIObjectType *type = object->GetObjectType(); // We need to do this to get the 'actual' object type (not a base type)
	asIScriptFunction *func = type->GetMethodByDecl("void serialize(StringStream&)");

	// Store object type name
	ss.write(string(type->GetName()));
	
	// Call serialization function on the object
	asIScriptContext *ctx = CreateContext();
	int r = ctx->Prepare(func); assert(r >= 0);
	r = ctx->SetObject(object); assert(r >= 0);
	r = ctx->SetArgAddress(0, &ss); assert(r >= 0);
	r = ctx->Execute();

	// Clean up
	r = ctx->Release();
}

asIScriptObject *XScriptEngine::deserialize(string &path)
{
	// Out script object
	asIScriptObject *object = 0;

	// Read file
	string content;
	if(XFileSystem::ReadFile(path, content))
	{
		// Deserialize object
		XStringStream ss(content);
		object = deserialize(ss);
	}
	else
	{
		LOG("Unable to read serialized file '%s'", path);
	}

	// Return object
	return object;
}

asIScriptObject *XScriptEngine::deserialize(XStringStream &ss)
{
	// Get type name
	string typeName;
	ss.read(typeName);
	
	// Create uninitialized object
	asIObjectType *type = s_module->GetObjectTypeByName(typeName.c_str());
	asIScriptObject *object = (asIScriptObject*)s_engine->CreateUninitializedScriptObject(type);

	if(object)
	{
		// Get deserialization function
		asIScriptFunction *func = type->GetMethodByDecl("void deserialize(StringStream&)");

		// Call deserialization function on the object
		asIScriptContext *ctx = CreateContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		r = ctx->SetObject(object); assert(r >= 0);
		r = ctx->SetArgAddress(0, &ss); assert(r >= 0);
		r = ctx->Execute();

		// Clean up
		r = ctx->Release();
	}
	else
	{
		LOG("Unable to create script object of type '%s'", typeName.c_str());
	}

	// Return script object
	return object;
}