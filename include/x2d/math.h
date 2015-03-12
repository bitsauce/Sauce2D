#ifndef X2D_MATH_H
#define X2D_MATH_H

#include "config.h"
#include "math/matrix.h"
#include "math/rect.h"
#include "math/rectanglepacker.h"
#include "math/vector.h"
#include "math/random.h"

#define PI 3.14159265359f

BEGIN_XD_NAMESPACE

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

namespace math
{
	// Degrees/Radians
	XDAPI float radToDeg(const float rad);
	XDAPI float degToRad(const float deg);

	// String case functions
	XDAPI string toLower(string &str, const int begin = 0, const int end = 0);
	XDAPI string toUpper(string &str, const int begin = 0, const int end = 0);

	// Supplementary math funcs
	XDAPI float clamp(const float x, const float min, const float max);
	XDAPI float maximum(const float a, const float b);
	XDAPI float minimum(const float a, const float b);

	XDAPI float round(const float v);
	XDAPI Vector2 round(const Vector2 &v);
	XDAPI float ceil(const float v);
	XDAPI Vector2 ceil(const Vector2 &v);
	XDAPI float floor(const float v);
	XDAPI Vector2 floor(const Vector2 &v);
	XDAPI float abs(const float v);
	XDAPI Vector2 abs(const Vector2 &v);
	XDAPI float sqrt(const float v);
	XDAPI Vector2 sqrt(const Vector2 &v);
	XDAPI float pow(const float a, const float b);
	XDAPI Vector2 pow(const Vector2 &v);
	XDAPI float lerp(const float v0, const float v1, const float t);
	XDAPI Vector2 lerp(const Vector2 &v0, const Vector2 &v1, const float t);

	XDAPI int mod(const int a, const int b);
	XDAPI uint ror(const uint a, const uint b);
	XDAPI uint rol(const uint a, const uint b);
}

END_XD_NAMESPACE

#endif // X2D_MATH_H