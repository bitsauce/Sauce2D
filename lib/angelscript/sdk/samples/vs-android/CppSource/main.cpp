#include <jni.h>
#include <errno.h>
#include <android/log.h>

#include "android_native_app_glue.h"

#include <angelscript.h>
#include "scriptbuilder.h"
#include "scriptstdstring.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "vsandroid", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "vsandroid", __VA_ARGS__))

#include <string>
#include <assert.h>

using namespace std;

string messageCallback;
void asMessageCallback(const asSMessageInfo *msg, void*)
{
	// Callback type
	const char *type = "ERR";
	if(msg->type == asMSGTYPE_WARNING) 
		type = "WARN";
	else if(msg->type == asMSGTYPE_INFORMATION) 
		type = "INFO";

	// Print to debug
	char out[512];
	sprintf(out, "%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message);
	LOGI(out);
}

#define MAIN_AS \
	"void main() {" \
	"print(\"Hello World 4\");" \
	"printA();" \
	"printB();" \
	"}"

asIScriptEngine *scriptEngine = 0;

// Compiles an angelscript module with name
int asCompileModule(string name)
{
	bool error = true;
	int r;
	while(error)
	{
		CScriptBuilder builder;
		r = builder.StartNewModule(scriptEngine, name.c_str()); 
		if(r < 0) 
		{
			// If the code fails here it is usually because there
			// is no more memory to allocate the module
			LOGW("Unable to start a new module. Usually do to lack of memory.");
			return -1;
		}

		r = builder.AddSectionFromMemory("main.as", MAIN_AS);
		if(r < 0)
		{
			// The builder wasn't able to load the file. Maybe the file
			// has been removed, or the wrong name was given, or some
			// preprocessing commands are incorrectly written.
			LOGW("Unable to load main.as. Make sure the file and other included files exist.");
			return -2;
		}

		r = builder.BuildModule();
		if(r < 0)
		{
			// An error occurred. Instruct the script writer to fix the 
			// compilation errors that were listed in the output stream.
			string debug = "AngelScript has encountered the following error:\n\n";
			debug += messageCallback;
			debug += "\nPlease fix the error(s) and press retry, or press cancel to exit the application.";
			messageCallback.clear();
			LOGW(debug.c_str());

			return -3;
		}else{
			error = false;
		}
	}
	return 0;
}

void print(const string &msg)
{
	LOGI(msg.c_str());
}

void badprint(string msg)
{
	LOGI(msg.c_str());
}

void badprint2(const string msg)
{
	LOGI(msg.c_str());
}

void olprint(const string &msg)
{
	LOGI(msg.c_str());
}

void olprint(const string &msg, const bool value)
{
	LOGI((msg + (value ? "true" : "false")).c_str());
}

void defprint(const string &msg)
{
	LOGI(msg.c_str());
}

class Base
{
public:
	virtual void printA()
	{
		LOGI("Base: PrintA called");
	}
	
	virtual void printB()
	{
		LOGI("Base: PrintB called");
	}
};

class CRef
{
public:
	void AddRef()
	{
		refCount++;
	}

	void Release()
	{
		if(--refCount == 0)
			delete this;
	}

	static CRef *Factory()
	{
		return new CRef();
	}

	virtual void printA()
	{
		LOGI("CRef: Print");
	}

private:
	int refCount;
};

class Derived : public Base
{
public:
	void printA()
	{
		LOGI("Derived: PrintA called");
	}
};

void android_main(struct android_app* app)
{
	// Create AS
	scriptEngine = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	scriptEngine->SetEngineProperty(asEP_COMPILER_WARNINGS, 0);

	// Set the message callback to receive information on errors in human readable form.
	int r = scriptEngine->SetMessageCallback(asFUNCTION(asMessageCallback), 0, asCALL_CDECL); assert( r >= 0 );

	// Register string
	RegisterStdString(scriptEngine);

	// Perform extensive testing
	scriptEngine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void badprint(string msg)", asFUNCTION(badprint), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void badprint2(const string msg)", asFUNCTION(badprint2), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void olprint(const string &in)", asFUNCTIONPR(olprint, (const string&), void), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void olprint(const string &in, const bool value)", asFUNCTIONPR(olprint, (const string&, const bool), void), asCALL_CDECL);
	scriptEngine->RegisterGlobalFunction("void defprint(const string &in defarg = \"This was a default arg\")", asFUNCTION(defprint), asCALL_CDECL);

	// Test enums
	scriptEngine->RegisterEnum("AEnum");
	scriptEngine->RegisterEnumValue("AEnum", "Enum1", 5);
	scriptEngine->RegisterEnumValue("AEnum", "Enum2", 2);
	scriptEngine->RegisterEnumValue("AEnum", "Enum3", 1);

	// Test asCALL_THISCALL_ASGLOBAL singletons
	Derived *derived = new Derived();
	Base *base = static_cast<Base*>(derived);
	scriptEngine->RegisterGlobalFunction("void printA()", asMETHOD(Base, printA), asCALL_THISCALL_ASGLOBAL, base);
	scriptEngine->RegisterGlobalFunction("void printB()", asMETHOD(Base, printB), asCALL_THISCALL_ASGLOBAL, base);
	
	// Test asCALL_THISCALL on a reference type
	/*r = scriptEngine->RegisterObjectType("ref", 0, asOBJ_REF); assert( r >= 0 );
	r = scriptEngine->RegisterObjectBehaviour("ref", asBEHAVE_FACTORY, "ref@ f()", asFUNCTION(CRef::Factory), asCALL_CDECL); assert( r >= 0 );
	r = scriptEngine->RegisterObjectBehaviour("ref", asBEHAVE_ADDREF, "void f()", asMETHOD(CRef, AddRef), asCALL_THISCALL); assert( r >= 0 );
	r = scriptEngine->RegisterObjectBehaviour("ref", asBEHAVE_RELEASE, "void f()", asMETHOD(CRef, Release), asCALL_THISCALL); assert( r >= 0 );
	r = scriptEngine->RegisterObjectMethod("ref", "void print()", asMETHOD(CRef, printA), asCALL_THISCALL);
	r = scriptEngine->RegisterGlobalProperty("ref Derived", base); assert( r >= 0 );*/

	// Test user defined types

	// Test functions which takes userdefined types as arguments

	// Compile
	r = asCompileModule("GameModule"); assert( r >= 0 );
	
	// Create the game module
	asIScriptModule *mod = scriptEngine->GetModule("GameModule");

	// Find the function that is to be called
	asIScriptFunction *mainFunc = mod->GetFunctionByDecl("void main()");
	assert(mainFunc != NULL);

	// Create our context and run main
	asIScriptContext* ctx = scriptEngine->CreateContext();
	ctx->Prepare(mainFunc);
	r = ctx->Execute();
	if(r != asEXECUTION_FINISHED)
	{
		// The execution didn't complete as expected. Determine what happened.
		if(r == asEXECUTION_EXCEPTION)
		{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			const char *sectionName;
			int line;
			ctx->GetExceptionLineNumber(&line, &sectionName);

			char out[2048];
			sprintf(out, "An exception '%s' occurred in function '%s' in file '%s' (%i). "
				"Please correct the code and try again", ctx->GetExceptionString(),
				ctx->GetExceptionFunction()->GetDeclaration(), sectionName, line);
			LOGW(out);
		}
	}
	ctx->Release();

	scriptEngine->Release();

	//delete derived;
}