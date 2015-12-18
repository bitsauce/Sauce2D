#ifndef X2D_CONFIG_H
#define X2D_CONFIG_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

/*********************************************************************
**	Compiler preprocessor											**
**********************************************************************/
#if __WINDOWS__ && _MSC_VER >= 1500
	#define USE_CTR_SECURE
#endif
#if defined(_DEBUG) && !defined(X2D_DEBUG) 
	#define X2D_DEBUG
#endif

/*********************************************************************
**	Library export preprocessor										**
**********************************************************************/
#if defined(__WINDOWS__) && defined(X2D_EXPORT)
	#define XDAPI __declspec(dllexport)
#elif defined(__WINDOWS__) && defined(X2D_IMPORT)
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
#pragma warning(disable : 4458) // Declaration hides class member
#pragma warning(disable : 4456) // Declaration hides pevious local declaration

/*********************************************************************
**	Include	STL														**
**********************************************************************/
#ifdef __WINDOWS__
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
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
	#include <unordered_map>
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
	#include <queue>
	#include "..\3rdparty\gl3w\include\GL\gl3w.h"
	#include "..\3rdparty\gl3w\include\GL\wglext.h"
	#include "..\3rdparty\tinyxml2\tinyxml2.h"
#elif __LINUX__
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

typedef int VirtualKey;

#define BEGIN_CG_NAMESPACE namespace cg {
#define END_CG_NAMESPACE }

BEGIN_CG_NAMESPACE

/*********************************************************************
**	Engine return codes												**
**********************************************************************/
enum RetCode
{
	X2D_OK						= 0,
	X2D_NOT_INITIALIZED			= -1,
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
enum EngineFlag
{
	XD_EXPORT_LOG				= 1 << 0, ///< Export the output log to a log file.
	XD_RUN_IN_BACKGROUND		= 1 << 1, ///< This will allow the program to run while not focused.
	XD_BLOCK_BACKGROUND_INPUT	= 1 << 2, ///< If XD_RUN_IN_BACKGROUND is set, this will block input while program is out of focus. 
	XD_VERBOSE					= 1 << 4  ///< This will make the engine produce more verbose messages from engine calls.
};

/*********************************************************************
**	Message types													**
**********************************************************************/
enum MessageType
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

	enum UnicodeByteOrder
	{
		LITTLE_ENDIAN,
		BIG_ENDIAN,
	};

	// This function will attempt to decode a UTF-8 encoded character in the buffer.
	// If the encoding is invalid, the function returns -1.
	int decodeUTF8(const char *encodedBuffer, unsigned int *outCharLength);

	// This function will encode the value into the buffer.
	// If the value is invalid, the function returns -1, else the encoded length.
	int encodeUTF8(unsigned int value, char *outEncodedBuffer, unsigned int *outCharLength);

	// This function will attempt to decode a UTF-16 encoded character in the buffer.
	// If the encoding is invalid, the function returns -1.
	int decodeUTF16(const char *encodedBuffer, unsigned int *outCharLength, UnicodeByteOrder byteOrder = LITTLE_ENDIAN);

	// This function will encode the value into the buffer.
	// If the value is invalid, the function returns -1, else the encoded length.
	int encodeUTF16(unsigned int value, char *outEncodedBuffer, unsigned int *outCharLength, UnicodeByteOrder byteOrder = LITTLE_ENDIAN);
}

END_CG_NAMESPACE

/*********************************************************************
**	Macros															**
**********************************************************************/

#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

#define TUPLE_CMP2(a, b) \
	if(a < b) return false; \
	if(a > b) return true;

#endif // X2D_CONFIG_H