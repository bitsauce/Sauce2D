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
	#if defined(_DEBUG) && !defined(X2D_DEBUG) 
		#define X2D_DEBUG
	#endif
	#elif __GNUC__
		#define X2D_LINUX
	#elif __APPLE__
		#define X2D_OSX
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

#endif // X2D_CONFIG_H