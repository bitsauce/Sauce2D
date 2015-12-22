#ifndef CGF_CONFIG_H
#define CGF_CONFIG_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

/*********************************************************************
**	Compiler preprocessor											**
**********************************************************************/
#if __WINDOWS__ && _MSC_VER >= 1500
	#define USE_CTR_SECURE
#endif
#if defined(_DEBUG) && !defined(CGF_DEBUG) 
	#define CGF_DEBUG
#endif

/*********************************************************************
**	Library export preprocessor										**
**********************************************************************/
#if defined(__WINDOWS__) && defined(CGF_EXPORT)
	#define CGF_API __declspec(dllexport)
#elif defined(__WINDOWS__) && defined(CGF_IMPORT)
	#define CGF_API __declspec(dllimport)
#else
	#define CGF_API 
#endif

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

#define BEGIN_CGF_NAMESPACE namespace cgf {
#define END_CGF_NAMESPACE }

BEGIN_CGF_NAMESPACE

/*********************************************************************
**	Engine return codes												**
**********************************************************************/
enum RetCode
{
	CGF_OK						= 0,
	CGF_RUNTIME_EXCEPTION		= -1,
	CGF_INVALID_CONFIG			= -2,
	CGF_UNKNOWN_EXCEPTION		= -3
};

/*********************************************************************
**	Engine run flags												**
**********************************************************************/
enum EngineFlag
{
	CGF_EXPORT_LOG				= 1 << 0, ///< Export the output log to a log file.
	CGF_RUN_IN_BACKGROUND		= 1 << 1, ///< This will allow the program to run while not focused.
	CGF_BLOCK_BACKGROUND_INPUT	= 1 << 2, ///< If CGF_RUN_IN_BACKGROUND is set, this will block input while program is out of focus. 
	CGF_VERBOSE					= 1 << 4  ///< This will make the engine produce more verbose messages from engine calls.
};

/*********************************************************************
**	Message types													**
**********************************************************************/
enum MessageType
{
	CGF_INFO_MSG,
	CGF_WARN_MSG,
	CGF_ERR_MSG
};

/*********************************************************************
**	Global util functions											**
**********************************************************************/
namespace util
{
	// Split string
	CGF_API vector<string> splitString(const string& src, const string& delim);

	// Replace all
	CGF_API void replaceAll(string& str, string& from, string& to);
	
	// String case functions
	CGF_API string toLower(string &str, const int begin = 0, const int end = 0);
	CGF_API string toUpper(string &str, const int begin = 0, const int end = 0);

	// String convertering
	CGF_API int   strToInt(const string &str);
	CGF_API float strToFloat(const string &str);
	CGF_API bool  strToBool(const string &str);
	CGF_API uchar strToAscii(const string&);

	CGF_API string intToStr(const int);
	CGF_API string floatToStr(const float);
	CGF_API string boolToStr(const bool);
	CGF_API string asciiToStr(const uchar);
	
	// File paths
	CGF_API bool fileExists(string filePath);
	CGF_API string getAbsoluteFilePath(const string &assetPath);
	CGF_API void toAbsoluteFilePath(string &assetPath);
	CGF_API void toDirectoryPath(string &path);

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

END_CGF_NAMESPACE

/*********************************************************************
**	Macros															**
**********************************************************************/

#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

#define TUPLE_CMP2(a, b) \
	if(a < b) return false; \
	if(a > b) return true;

#endif // CGF_CONFIG_H
