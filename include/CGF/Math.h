#ifndef CGF_MATH_H
#define CGF_MATH_H

#include <CGF/Math/Vector.h>
#include <CGF/Math/Matrix.h>
#include <CGF/Math/Rectangle.h>
#include <CGF/Math/RectanglePacker.h>
#include <CGF/Math/Random.h>

#define PI 3.14159265359f

BEGIN_CGF_NAMESPACE

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
	CGF_API Vector2F round(const Vector2F &v);
	CGF_API float ceil(const float v);
	CGF_API Vector2F ceil(const Vector2F &v);
	CGF_API float floor(const float v);
	CGF_API Vector2F floor(const Vector2F &v);
	CGF_API float abs(const float v);
	CGF_API Vector2F abs(const Vector2F &v);
	CGF_API float sqrt(const float v);
	CGF_API Vector2F sqrt(const Vector2F &v);
	CGF_API float pow(const float a, const float b);
	CGF_API Vector2F pow(const Vector2F &v);

	template<typename T> T lerp(const T &v0, const T &v1, const float t)
	{
		return v0 * (1.0f - t) + v1 * t;
	}
	
	CGF_API float step(const float a, const float x);

	CGF_API int mod(const int a, const int b);
	CGF_API uint ror(const uint a, const uint b);
	CGF_API uint rol(const uint a, const uint b);
}

END_CGF_NAMESPACE

#endif // CGF_MATH_H
