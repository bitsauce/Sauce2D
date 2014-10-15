#ifndef X2D_CONFIG_H
#define X2D_CONFIG_H

/*********************************************************************
**	Compiler preprocessor											**
**********************************************************************/
#if !defined(X2D_WINDOWS) && !defined(X2D_LINUX) && !defined(X2D_OSX)
	#ifdef _MSC_VER
		#define X2D_WINDOWS
		#if _MSC_VER >= 1500
			#define USE_CTR_SECURE
		#endif
	#elif __GNUC__
		#define X2D_LINUX
	#elif __APPLE__
		#define X2D_OSX
	#endif
	#if defined(_DEBUG) && !defined(X2D_DEBUG) 
		#define X2D_DEBUG
	#endif
#endif

/*********************************************************************
**	Library export preprocessor										**
**********************************************************************/
#if defined(X2D_WINDOWS) && defined(X2D_EXPORT)
	#define XDAPI __declspec(dllexport)
#elif defined(X2D_WINDOWS) && defined(X2D_IMPORT)
	#define XDAPI __declspec(dllimport)
#else
	#define XDAPI 
#endif

/*********************************************************************
**	Pragma disable warnings											**
**********************************************************************/
#pragma warning(disable : 4512) // 
#pragma warning(disable : 4127) // 
#pragma warning(disable : 4100) // Unreferenced parameters
#pragma warning(disable : 4251) // DLL-export warning

/*********************************************************************
**	Include	STL														**
**********************************************************************/
#ifdef X2D_WINDOWS
	#include <windows.h>
	#include <windowsx.h>
	#include <string>
	#include <vector>
	#include <list>
	#include <cmath>
	#include <assert.h>
	#include <algorithm>
	#include <map>
	#include <set>
	#include <stack>
	#include <exception>
	#include <sstream>
	#include <thread>
	#include <mutex>
	#include <chrono>
#elif X2D_LINUX
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <fcntl.h>
	#include <stdlib.h>
	#include <math.h>
	#include <float.h>
	#include <string>
	#include <vector>
	#include <list>
	#include <assert.h>
	#include <algorithm>
	#include <map>
#endif

/*********************************************************************
**	Enable STL namespace											**
**********************************************************************/
using namespace std;

/*********************************************************************
**	Set typedefs													**
**********************************************************************/
typedef unsigned short ushort;
typedef unsigned int uint;
typedef uintptr_t uintptr;
typedef unsigned char uchar;
typedef unsigned long ulong;

/*********************************************************************
**	Engine return codes												**
**********************************************************************/
enum XRetCode
{
	X2D_OK						= 0,
	X2D_COMPILE_FAILED			= -1,
	X2D_MISSING_MAIN			= -2,
	X2D_PLUGIN_LOAD_ERROR		= -3,
	X2D_RUNTIME_EXCEPTION		= -4,
	X2D_INVALID_CONFIG			= -5,
	X2D_FILE_READ_ERROR			= -6,
	X2D_UNKNOWN_EXCEPTION		= -7
};

/*********************************************************************
**	Engine run flags												**
**********************************************************************/
enum XEngineFlag
{
	XD_EXPORT_LOG		=	1 << 0,
	XD_SHOW_WARININGS	=	1 << 1
};

/*********************************************************************
**	Message types													**
**********************************************************************/
enum XMessageType
{
	X2D_INFO_MSG,
	X2D_WARN_MSG,
	X2D_ERR_MSG
};

/*********************************************************************
**	Global util functions											**
**********************************************************************/
namespace util
{
	// Split string
	XDAPI vector<string> splitString(const string& src, const string& delim); // Maybe not very math-ish, but nevermind

	// Replace all
	XDAPI void replaceAll(string& str, string& from, string& to);
	
	// String case functions
	XDAPI string toLower(string &str, const int begin = 0, const int end = 0);
	XDAPI string toUpper(string &str, const int begin = 0, const int end = 0);

	// String convertering
	XDAPI int   strToInt(const string &str);
	XDAPI float strToFloat(const string &str);
	XDAPI bool  strToBool(const string &str);
	XDAPI uchar strToAscii(const string&);

	XDAPI string intToStr(const int);
	XDAPI string floatToStr(const float);
	XDAPI string boolToStr(const bool);
	XDAPI string asciiToStr(const uchar);
	
	// File paths
	XDAPI bool fileExists(string filePath);
	XDAPI string getAbsoluteFilePath(const string &assetPath);
	XDAPI void toAbsoluteFilePath(string &assetPath);
	XDAPI void toDirectoryPath(string &path);
}

/*********************************************************************
**	AngelScript header												**
**********************************************************************/
#include <angelscript.h>

/*********************************************************************
**	A class register for automatic class-to-script registration		**
**********************************************************************/
class ClassRegister
{
public:
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

/*********************************************************************
**	Simple reference counter										**
**********************************************************************/
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
		asAtomicInc(refCount);
	}

	int release()
	{
		return asAtomicDec(refCount);
	}

	int get() const
	{
		return refCount;
	}

private:
	int refCount;
};

/*********************************************************************
**	AngelScript macros												**
**********************************************************************/

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
		throw XException(X2D_RUNTIME_EXCEPTION, msg);	\
	} return ret

#define AS_DECL_REF																		\
	private:																			\
	RefCounter refCounter;																\
	static ClassRegister::Registerer s_basereg;											\
	static int Declare(asIScriptEngine *scriptEngine);									\
	static int Register(asIScriptEngine *scriptEngine);									\
	static int s_typeId;																\
	public:																				\
	virtual void addRef() { refCounter.add(); }											\
	virtual void release() { if(refCounter.release() == 0) delete this; }				\
	static int GetTypeId() { return s_typeId; }

#define AS_REG_REF(clazz, name)														    \
	ClassRegister::Registerer clazz::s_basereg(&clazz::Declare, &clazz::Register);		\
	int clazz::s_typeId = 0;															\
	int clazz::Declare(asIScriptEngine *scriptEngine)									\
	{																					\
		int r = s_typeId = scriptEngine->RegisterObjectType(name, 0, asOBJ_REF);		\
						AS_ASSERT														\
		r = scriptEngine->RegisterObjectBehaviour(name, asBEHAVE_ADDREF, "void f()",    \
						asMETHOD(clazz, addRef), asCALL_THISCALL); AS_ASSERT			\
		r = scriptEngine->RegisterObjectBehaviour(name, asBEHAVE_RELEASE, "void f()",   \
						asMETHOD(clazz, release), asCALL_THISCALL); AS_ASSERT			\
		return r;																		\
	}

#define AS_DECL_VALUE																	\
	private:																			\
	static ClassRegister::Registerer s_basereg;											\
	static int Declare(asIScriptEngine *scriptEngine);									\
	static int Register(asIScriptEngine *scriptEngine);									\
	static int s_typeId;																\
	public:																				\
	static int GetTypeId() { return s_typeId; }

#define AS_REG_VALUE(clazz, name)														\
	ClassRegister::Registerer clazz::s_basereg(&clazz::Declare, &clazz::Register);		\
	int clazz::s_typeId = 0;															\
	int clazz::Declare(asIScriptEngine *scriptEngine)									\
	{																					\
		int r = s_typeId = scriptEngine->RegisterObjectType(name, sizeof(clazz),		\
						asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK); AS_ASSERT		\
		return r;																		\
	}

#define AS_DECL_SINGLETON																\
	private:																			\
	static ClassRegister::Registerer s_basereg;											\
	static int Declare(asIScriptEngine *scriptEngine);									\
	static int Register(asIScriptEngine *scriptEngine);									\
	public:

#define AS_REG_SINGLETON(clazz)															\
	ClassRegister::Registerer clazz::s_basereg(&clazz::Declare, &clazz::Register);		\
	int clazz::Declare(asIScriptEngine *scriptEngine)									\
	{																					\
		int r = scriptEngine->RegisterObjectType(#clazz, 0, asOBJ_REF | asOBJ_NOHANDLE);\
		AS_ASSERT																		\
		return r;																		\
	}

#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

#define TUPLE_CMP2(a, b) \
	if(a < b) return false; \
	if(a > b) return true;

#endif // X2D_CONFIG_H