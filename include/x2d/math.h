#ifndef X2D_MATH_H
#define X2D_MATH_H

#include <x2d/config.h>
#include <x2d/util.h>
#include <x2d/base.h>
#include "math/vector.h"
#include "math/rect.h"
#include "math/matrix.h"
#include "math/rectanglepacker.h"

#include <cmath>

#define PI 3.14159265359f

class XDAPI xdMath
{
	AS_DECL_SINGLETON
public:
	
	void setEnumValue(int *out, int typeId, int val);

	// Psuedo-random numbers
	void  seedRandom(const uint seed);
	float getRandomFloat();
	int   getRandomInt(int a, int b);
	int   getRandomInt();

	// Degrees/Radians
	float radToDeg(const float rad);
	float degToRad(const float deg);

	// String case functions
	string toLower(string &str, const int begin = 0, const int end = 0);
	string toUpper(string &str, const int begin = 0, const int end = 0);

	// Supplementary math funcs
	float clamp(const float min, const float max, const float x);
	float maximum(const float a, const float b);
	float minimum(const float a, const float b);
	float round(const float value) { return (float)int((value > 0.0f) ? (value + 0.5f) : (value - 0.5f)); }
	float ceil(const float value) { return ceilf(value);  }
	float floor(const float value) { return floorf(value); }
	float abs(const float value) { return ::abs(value); }
	float sqrt(const float value) { return sqrtf(value); }
	float pow(const float a, const float b) { return powf(a, b); }

	// Trigonometry
	float cos(const float value) { return cosf(value); }
	float sin(const float value) { return sinf(value); }
	float tan(const float value) { return tanf(value); }
	float acos(const float value) { return acosf(value); }
	float asin(const float value) { return asinf(value); }
	float atan(const float value) { return atanf(value); }
	float atan2(const float y, const float x) { return atan2f(y, x); }
	float getPI() const { return PI; }
};

#endif // X2D_MATH_H