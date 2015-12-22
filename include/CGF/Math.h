#ifndef CGF_MATH_H
#define CGF_MATH_H

#include <CGF/math/Matrix.h>
#include <CGF/math/Rectangle.h>
#include <CGF/math/RectanglePacker.h>
#include <CGF/math/Vector.h>
#include <CGF/math/Random.h>

#define PI 3.14159265359f

BEGIN_CGF_NAMESPACE


/**
 * \brief A class for representing colors.
 *
 * This class represents a color with red, green, blue and alpha channels (RGBA). Each channel is a byte in size.
 */
struct CGF_API Color
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
	CGF_API float radToDeg(const float rad);
	CGF_API float degToRad(const float deg);

	// String case functions
	CGF_API string toLower(string &str, const int begin = 0, const int end = 0);
	CGF_API string toUpper(string &str, const int begin = 0, const int end = 0);

	// Supplementary math funcs
	CGF_API float clamp(const float x, const float min, const float max);
	CGF_API float maximum(const float a, const float b);
	CGF_API float minimum(const float a, const float b);

	CGF_API float round(const float v);
	CGF_API Vector2 round(const Vector2 &v);
	CGF_API float ceil(const float v);
	CGF_API Vector2 ceil(const Vector2 &v);
	CGF_API float floor(const float v);
	CGF_API Vector2 floor(const Vector2 &v);
	CGF_API float abs(const float v);
	CGF_API Vector2 abs(const Vector2 &v);
	CGF_API float sqrt(const float v);
	CGF_API Vector2 sqrt(const Vector2 &v);
	CGF_API float pow(const float a, const float b);
	CGF_API Vector2 pow(const Vector2 &v);
	CGF_API float lerp(const float v0, const float v1, const float t);
	CGF_API Vector2 lerp(const Vector2 &v0, const Vector2 &v1, const float t);

	CGF_API int mod(const int a, const int b);
	CGF_API uint ror(const uint a, const uint b);
	CGF_API uint rol(const uint a, const uint b);
}

END_CGF_NAMESPACE

#endif // CGF_MATH_H
