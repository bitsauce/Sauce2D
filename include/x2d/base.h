#ifndef X2D_BASE_H
#define X2D_BASE_H

// AngelScript
#include <angelscript.h>

// Standard libraries
#include <vector>
#include <list>
#include <string>
#include <utility>
#include <unordered_map>
#include <string>

using namespace std;


//-----------------------------------------------------------------------
// Script object base class
//-----------------------------------------------------------------------

class Base
{
public:
    virtual ~Base() { }

    typedef int (*RegisterScriptClassFunc)(asIScriptEngine*);
    typedef int (*DeclareScriptClassFunc)(asIScriptEngine*);

	static int Register(asIScriptEngine *scriptEngine)
    {
		// Error value
		int i = 0;
		
		// Register all script classes
		for(list<DeclareScriptClassFunc>::iterator itr = classesToDeclare().begin(); itr != classesToDeclare().end(); ++itr)
		{
			if((i = (*itr)(scriptEngine)) < 0)
			{
				// An error occured while declaring a class
				return i;
			}
		}

		// Register all script classes
		for(list<RegisterScriptClassFunc>::iterator itr = classesToRegister().begin(); itr != classesToRegister().end(); ++itr)
		{
			if((i = (*itr)(scriptEngine)) < 0)
			{
				// An error occured while registering a class
				return i;
			}
		}
		return i;
    }

    struct Registerer
    {
        explicit Registerer(DeclareScriptClassFunc declFunc, RegisterScriptClassFunc regFunc)
        {
			classesToDeclare().push_back(declFunc);
			classesToRegister().push_back(regFunc);
        }

	private:
		// Don't allow copying of registrars
		Registerer(const Registerer &) {}
    };

private:
    static list<DeclareScriptClassFunc> &classesToDeclare()
	{
		static list<DeclareScriptClassFunc> lst;
		return lst;
	}

    static list<RegisterScriptClassFunc> &classesToRegister()
	{
		static list<RegisterScriptClassFunc> lst;
		return lst;
	}
};

//-----------------------------------------------------------------------
// A simple refcounter
// We use this reference counter to force the user to initialize it.
//-----------------------------------------------------------------------

class RefCounter
{
public:
	RefCounter() :
		refCount(1)
	{
	}
	
	RefCounter(const RefCounter &) :
		refCount(1)
	{
	}

	void add()
	{
		refCount++;
	}

	int release()
	{
		return --refCount;
	}

	int get() const
	{
		return refCount;
	}

private:
	int refCount;
};

//-----------------------------------------------------------------------
// AngelScript macros
//-----------------------------------------------------------------------

#ifdef X2D_DEBUG
#include <assert.h>
#define AS_ASSERT assert(r >= 0);
#else
#define AS_ASSERT if(r < 0) return r;
#endif

#define AS_THROW(msg, ret)								\
	asIScriptContext *ctx = asGetActiveContext();		\
	if(ctx) {											\
		ctx->SetException(msg);							\
	}else{												\
		throw xdException(XD_RUNTIME_EXCEPTION, msg);	\
	} return ret

#define AS_DECL_REF																		\
	private:																			\
	RefCounter refCounter;																\
	static Base::Registerer s_basereg;													\
	static int Declare(asIScriptEngine *scriptEngine);									\
	static int Register(asIScriptEngine *scriptEngine);									\
	static int s_typeId;																\
	public:																				\
	virtual void addRef() { refCounter.add(); }											\
	virtual void release() { if(refCounter.release() == 0) delete this; }				\
	static int GetTypeId() { return s_typeId; }

#define AS_DECL_VALUE																	\
	private:																			\
	static Base::Registerer s_basereg;													\
	static int Declare(asIScriptEngine *scriptEngine);									\
	static int Register(asIScriptEngine *scriptEngine);									\
	static int s_typeId;																\
	public:																				\
	static int GetTypeId() { return s_typeId; }

#define AS_DECL_SINGLETON																\
	private:																			\
	static Base::Registerer s_basereg;													\
	static int Declare(asIScriptEngine *scriptEngine);									\
	static int Register(asIScriptEngine *scriptEngine);									\
	public:

#define AS_REG_REF(clazz)																\
	Base::Registerer clazz::s_basereg(&clazz::Declare, &clazz::Register);				\
	int clazz::s_typeId = 0;															\
	int clazz::Declare(asIScriptEngine *scriptEngine)									\
	{																					\
		int r = s_typeId = scriptEngine->RegisterObjectType(#clazz, 0, asOBJ_REF);		\
						AS_ASSERT														\
		r = scriptEngine->RegisterObjectBehaviour(#clazz, asBEHAVE_ADDREF, "void f()",  \
						asMETHOD(clazz, addRef), asCALL_THISCALL); AS_ASSERT			\
		r = scriptEngine->RegisterObjectBehaviour(#clazz, asBEHAVE_RELEASE, "void f()", \
						asMETHOD(clazz, release), asCALL_THISCALL); AS_ASSERT			\
		return r;																		\
	}

#define AS_REG_VALUE(clazz)																\
	Base::Registerer clazz::s_basereg(&clazz::Declare, &clazz::Register);				\
	int clazz::s_typeId = 0;															\
	int clazz::Declare(asIScriptEngine *scriptEngine)									\
	{																					\
		int r = s_typeId = scriptEngine->RegisterObjectType(#clazz, sizeof(clazz),		\
						asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK); AS_ASSERT		\
		return r;																		\
	}

#define AS_REG_SINGLETON(clazz, name)													\
	Base::Registerer clazz::s_basereg(&clazz::Declare, &clazz::Register);				\
	int clazz::Declare(asIScriptEngine *scriptEngine)									\
	{																					\
		int r = scriptEngine->RegisterObjectType(name, 0, asOBJ_REF | asOBJ_NOHANDLE);	\
		AS_ASSERT																		\
		return r;																		\
	}

#endif // X2D_BASE_H