#ifndef SAUCE_CONFIG_H
#define SAUCE_CONFIG_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

/*********************************************************************
**	Compiler preprocessor											**
**********************************************************************/
#if __WINDOWS__ && _MSC_VER >= 1500
	#define USE_CTR_SECURE
#endif
#if defined(_DEBUG) && !defined(SAUCE_DEBUG) 
	#define SAUCE_DEBUG
#endif

/*********************************************************************
**	Library export preprocessor										**
**********************************************************************/
#if defined(__WINDOWS__) && defined(SAUCE_EXPORT)
	#define SAUCE_API __declspec(dllexport)
#elif defined(__WINDOWS__) && defined(SAUCE_IMPORT)
	#define SAUCE_API __declspec(dllimport)
#else
	#define SAUCE_API 
#endif

/*********************************************************************
**	Include	STL														**
**********************************************************************/
#ifdef __WINDOWS__
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
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#ifdef SAUCE_USE_FLOAT
typedef float  SFloat;
#else
typedef double SFloat;
#endif

#define BEGIN_SAUCE_NAMESPACE namespace sauce {
#define END_SAUCE_NAMESPACE }

#define SAUCE_DEFAULT_ORGANIZATION "SuperSauceEngine"

BEGIN_SAUCE_NAMESPACE

/*********************************************************************
**	Engine return codes												**
**********************************************************************/
enum RetCode
{
	SAUCE_OK					= 0,
	SAUCE_RUNTIME_EXCEPTION		= -1,
	SAUCE_INVALID_CONFIG		= -2,
	SAUCE_UNKNOWN_EXCEPTION		= -3
};

/*********************************************************************
**	Engine run flags												**
**********************************************************************/
enum EngineFlag
{
	SAUCE_EXPORT_LOG				= 1 << 0, ///< Export the output log to a log file.
	SAUCE_RUN_IN_BACKGROUND			= 1 << 1, ///< This will allow the program to run while not focused.
	SAUCE_BLOCK_BACKGROUND_INPUT	= 1 << 2, ///< If SAUCE_RUN_IN_BACKGROUND is set, this will block input while program is out of focus. 
	SAUCE_VERBOSE					= 1 << 4, ///< This will make the engine produce more verbose messages from engine calls.
	SAUCE_WINDOW_RESIZABLE			= 1 << 5
};

/*********************************************************************
**	Message types													**
**********************************************************************/
enum MessageType
{
	SAUCE_INFO_MSG,
	SAUCE_WARN_MSG,
	SAUCE_ERR_MSG
};

/*********************************************************************
**	Graphics backends												**
**********************************************************************/
struct GraphicsBackend
{
	enum Type
	{
		SAUCE_OPEN_GL,
		SAUCE_DIRECT_X,
		SAUCE_VULKAN
	};

	GraphicsBackend(const Type type = SAUCE_OPEN_GL, const int major = 3, const int minor = 1) :
		type(type),
		major(major),
		minor(minor)
	{
	}

	Type type;
	int major, minor;
};

/*********************************************************************
**	Global util functions											**
**********************************************************************/
namespace util
{
	// Split string
	SAUCE_API vector<string> splitString(const string& src, const string& delim);

	// Replace all
	SAUCE_API void replaceAll(string& str, string& from, string& to);
	
	// String case functions
	SAUCE_API string toLower(string &str, const int begin = 0, const int end = 0);
	SAUCE_API string toUpper(string &str, const int begin = 0, const int end = 0);

	// String convertering
	SAUCE_API int   strToInt(const string &str);
	SAUCE_API float strToFloat(const string &str);
	SAUCE_API bool  strToBool(const string &str);
	SAUCE_API uchar strToAscii(const string&);

	SAUCE_API string intToStr(const int);
	SAUCE_API string floatToStr(const float);
	SAUCE_API string boolToStr(const bool);
	SAUCE_API string asciiToStr(const uchar);
	
	// File paths
	SAUCE_API bool fileExists(string filePath);
	SAUCE_API string getAbsoluteFilePath(const string &assetPath);
	SAUCE_API void toAbsoluteFilePath(string &assetPath);
	SAUCE_API void toDirectoryPath(string &path);
	SAUCE_API string getWorkingDirectory();

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

END_SAUCE_NAMESPACE

/*********************************************************************
**	Macros															**
**********************************************************************/

#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

#define TUPLE_CMP2(a, b) \
	if(a < b) return false; \
	if(a > b) return true;

#endif // SAUCE_CONFIG_H
