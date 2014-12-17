#ifndef X2D_MATH_H
#define X2D_MATH_H

#include "config.h"
#include "math/matrix.h"
#include "math/rect.h"
#include "math/rectanglepacker.h"
#include "math/vector.h"
#include "math/random.h"

#define PI 3.14159265359f

namespace xd
{

struct XDAPI Color
{
	Color(uchar v = 0) { r = g = b = a = v; }
	Color(uchar r, uchar g, uchar b, uchar a = 255) { this->r = r; this->g = g; this->b = b; this->a = a; }
	
	Color &operator+(const Color &c)
	{
		r += c.r; g += c.g; b += c.b; a += c.a;
		return *this;
	}

	Color &operator*(const Color &c)
	{
		r *= c.r; g *= c.g; b *= c.b; a *= c.a;
		return *this;
	}

	Color &operator*(const float c);

	uchar r, g, b, a;
};

}

class XDAPI XMath
{
public:
	// Degrees/Radians
	static float radToDeg(const float rad);
	static float degToRad(const float deg);

	// String case functions
	static string toLower(string &str, const int begin = 0, const int end = 0);
	static string toUpper(string &str, const int begin = 0, const int end = 0);

	// Supplementary math funcs
	static float clamp(const float x, const float min, const float max);
	static float maximum(const float a, const float b);
	static float minimum(const float a, const float b);
	static float round(const float value) { return (float)int((value > 0.0f) ? (value + 0.5f) : (value - 0.5f)); }
	static float ceil(const float value) { return ceilf(value);  }
	static float floor(const float value) { return floorf(value); }
	static float abs(const float value) { return ::abs(value); }
	static float sqrt(const float value) { return sqrtf(value); }
	static float pow(const float a, const float b) { return powf(a, b); }
	static int mod(const int a, const int b) { int r = a % b; return r < 0 ? r + b : r; }
	static uint ror(const uint a, const uint b);
	static uint rol(const uint a, const uint b);
	static float lerp(const float v0, const float v1, const float t) { return (1.0f-t)*v0 + t*v1; }

	// Trigonometry
	static float cos(const float value) { return cosf(value); }
	static float sin(const float value) { return sinf(value); }
	static float tan(const float value) { return tanf(value); }
	static float acos(const float value) { return acosf(value); }
	static float asin(const float value) { return asinf(value); }
	static float atan(const float value) { return atanf(value); }
	static float atan2(const float y, const float x) { return atan2f(y, x); }
	static float getPI() { return PI; }
};

#endif // X2D_MATH_H