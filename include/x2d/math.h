#ifndef X2D_MATH_H
#define X2D_MATH_H

#include <x2d/config.h>
#include <x2d/math/matrix.h>
#include <x2d/math/rect.h>
#include <x2d/math/rectanglepacker.h>
#include <x2d/math/vector.h>
#include <x2d/math/random.h>

#define PI 3.14159265359f

BEGIN_XD_NAMESPACE


/**
 * \brief A class for representing colors.
 *
 * This class represents a color with red, green, blue and alpha channels (RGBA). Each channel is a byte in size.
 */
struct XDAPI Color
{
	/**
	 * Constructs a color setting all its components to equal \p v
	 * \param v Value of RGBA
	 */
	Color(uchar v = 0)
	{
		r = g = b = a = v;
	}

	/**
	 * Constructs a color setting all the components
	 * \param r Red component
	 * \param g Green component
	 * \param b Blue component
	 * \param a Alpha component
	 */
	Color(uchar r, uchar g, uchar b, uchar a = 255)
	{
		this->r = r; this->g = g; this->b = b; this->a = a;
	}
	
	/**
	 * Adds this color to another and claps the values of the components in the range [0-255]
	 * \param c Other color object
	 */
	Color &operator+(const Color &c)
	{
		r = (uchar) min(r + c.r, 255);
		g = (uchar) min(g + c.g, 255);
		b = (uchar) min(b + c.b, 255);
		a = (uchar) min(a + c.a, 255);
		return *this;
	}

	/**
	 * Multiplies all components of this color with a scalar and claps the values of the components in the range [0-255]
	 * \param scale Scalar
	 */
	Color &operator*(const float scale);

	/**
	 * Red component
	 */
	uchar r;

	/**
	 * Green component
	 */
	uchar g;

	/**
	 * Blue component
	 */
	uchar b;

	/**
	 * Alpha component
	 */
	uchar a;
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