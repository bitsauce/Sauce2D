#ifndef X2D_UTIL_H
#define X2D_UTIL_H

#include <x2d/config.h>

/*********************************************************************
**	Include	C headers												**
**********************************************************************/
#ifdef X2D_WINDOWS
	// Windows and STL headers
	#include <winsock2.h>
	#include <windows.h>
	#include <windowsx.h>
	#include <string>
	#include <vector>
	#include <list>
	#include <assert.h>
	#include <algorithm>
	#include <map>
	#include <set>
	#include <stack>
	#include <exception>
	#include <sstream>
#elif X2D_LINUX
	// Android and STL headers
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

#include "../source/scripts/stringstream.h"
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
enum xdRetCode
{
	XD_OK						= 0,
	XD_COMPILE_FAILED			= -1,
	XD_MISSING_MAIN				= -2,
	XD_PLUGIN_LOAD_ERROR		= -3,
	XD_RUNTIME_EXCEPTION		= -4,
	XD_INVALID_CONFIG			= -5,
	XD_FILE_READ_ERROR			= -6,
	XD_UNKNOWN_EXCEPTION		= -7
};

/*********************************************************************
**	Engine run flags												**
**********************************************************************/
enum xdEngineFlag
{
	XD_EXPORT_LOG		=	1 << 0,
	XD_DEBUG			=	1 << 1
};

/*********************************************************************
**	Message types													**
**********************************************************************/
enum xdMessageType
{
	XD_INFO_MSG,
	XD_WARN_MSG,
	XD_ERR_MSG
};

/*********************************************************************
**	Not implemented macro											**
**********************************************************************/
#define NOT_IMPLEMENTED(func)			LOG("%s does not have a implementation.", #func);
#define NOT_IMPLEMENTED_RET(func, ret)	LOG("%s does not have a implementation.", #func); return ret;

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


	// Type conversion
	/*int strToInt(const string &str)		{ return math::strToInt(str); }
	float strToFloat(const string &str)	{ return math::strToFloat(str); }
	bool strToBool(const string &str)	{ return math::strToBool(str); }
	uchar strToAscii(const string &str)	{ return math::strToAscii(str); }
	
	string intToStr(const int i)		{ return math::intToStr(i); }
	string floatToStr(const float f)	{ return math::floatToStr(f); }
	string boolToStr(const bool b)		{ return math::boolToStr(b); }
	string asciiToStr(const uchar c)	{ return math::asciiToStr(c); }*/

#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

#endif // X2D_UTIL_H