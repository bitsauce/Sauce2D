#ifndef PLATFORM_H
#define PLATFORM_H

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
#endif

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

/*********************************************************************
**	Library export preprocessor										**
**********************************************************************/
#if defined(X2D_WINDOWS) && defined(X2D_EXPORT)
	#define X2DAPI __declspec(dllexport)
#else
	#define X2DAPI 
#endif

/*********************************************************************
**	Pragma disable warnings											**
**********************************************************************/
#pragma warning(disable : 4512)
#pragma warning(disable : 4127)
#pragma warning(disable : 4100) // Unreferenced parameters
//#pragma warning (disable:4251)

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
enum X2DRetCode
{
	X2D_OK						= 0,
	X2D_CompileError			= -1,
	X2D_MissingMain				= -2,
	X2D_ScriptException			= -3,
	X2D_MissingComponent		= -4,
	X2D_AssetLoadError			= -5,
	X2D_InvalidImageFormat		= -6,
	X2D_UndefinedError			= -7
};

/*********************************************************************
**	Engine run flags												**
**********************************************************************/
enum X2DRunFlag
{
	X2D_ExportLog		=	1 << 0,
	X2D_Debug			=	1 << 1
};

/*********************************************************************
**	Application states												**
**********************************************************************/
enum X2DState
{
	X2D_Initialized		= 1 << 0,
	X2D_Paused			= 1 << 1,
	X2D_Closed			= 1 << 2
};

/*********************************************************************
**	Message types													**
**********************************************************************/
enum X2DMessageType
{
	X2D_PrintMsg,
	X2D_WarningMsg,
	X2D_SuccessMsg,
	X2D_ErrorMsg
};

/*********************************************************************
**	Not implemented macro											**
**********************************************************************/
#define NOT_IMPLEMENTED(func)			iosystem::print("%s does not have a implementation.", #func);
#define NOT_IMPLEMENTED_RET(func, ret)	iosystem::print("%s does not have a implementation.", #func); return ret;
namespace iosystem {
	X2DAPI void print(const string &msg, const X2DMessageType type);
	X2DAPI void print(const string &msg);
	X2DAPI void warn(const string &msg);
	X2DAPI void error(const string &msg);
	X2DAPI void success(const string &msg);
	X2DAPI void print(const char*, ...);
	X2DAPI void warn(const char*, ...);
	X2DAPI void success(const char*, ...);
	X2DAPI void error(const char*, ...);
}

#endif // PLATFORM_H