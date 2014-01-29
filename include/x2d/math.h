#ifndef MATH_MATH_H
#define MATH_MATH_H

#include "x2d/platform.h"
#include "math/vector.h"
#include "math/rect.h"
#include "math/matrix.h"
#include "math/array.h"
#include "math/rectPacker.h"

/*********************************************************************
**	STL math wrapper												**
**********************************************************************/
namespace math
{
	// Trigonometry

	// String converters
	X2DAPI int   strToInt(const string &str);
	X2DAPI float strToFloat(const string &str);
	X2DAPI bool  strToBool(const string &str);
	X2DAPI string asciiToStr(const uchar);

	X2DAPI string intToStr(const int);
	X2DAPI string floatToStr(const float);
	X2DAPI string boolToStr(const bool);
	X2DAPI uchar strToAscii(const string&);

	// Psuedo-random numbers
	X2DAPI void  seedRandom(const uint);
	X2DAPI float getRandomFloat();
	X2DAPI int   getRandomInt(int a, int b);
	X2DAPI int   getRandomInt();

	// Degrees/Radians
	X2DAPI float radToDeg(const float rad);
	X2DAPI float degToRad(const float deg);

	// String case functions
	X2DAPI string toLower(string &str, const int begin = 0, const int end = 0);
	X2DAPI string toUpper(string &str, const int begin = 0, const int end = 0);

	// Supplementary math funcs
	X2DAPI float clampValue(const float min, const float max, const float x);
	X2DAPI float maximum(const float a, const float b);
	X2DAPI float minimum(const float a, const float b);

	// AngelScipt
	X2DAPI void setEnumValue(int *out, int typeId, int val);
}

void RegisterMath();

#endif // MATH_MATH_H