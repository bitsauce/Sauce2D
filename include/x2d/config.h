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
	#include <functional>
	#include <map>
	#include <set>
	#include <stack>
	#include <exception>
	#include <sstream>
	#include <thread>
	#include <mutex>
	#include <assert.h>
	#include <fstream>
	#include <sstream>
	#include <memory>
	#include <GL/glew.h>
	#include <GL/wglew.h>
#elif X2D_UNIX
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
#define delegate function

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
**	Macros															**
**********************************************************************/

#define SINGLETON_DECL(clazz) \
	private: \
		static clazz *s_this;

#define SINGLETON_DEF(clazz) \
	clazz *clazz::s_this = 0; \

#define SINGLETON_ASSERT \
		assert(s_this == 0); \
		s_this = this;

#define SINGLETON_OBJECT s_this

#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

#define TUPLE_CMP2(a, b) \
	if(a < b) return false; \
	if(a > b) return true;

#endif // X2D_CONFIG_H