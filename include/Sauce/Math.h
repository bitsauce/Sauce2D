#ifndef SAUCE_MATH_H
#define SAUCE_MATH_H

#include <Sauce/Math/Vector.h>
#include <Sauce/Math/Matrix.h>
#include <Sauce/Math/Rectangle.h>
#include <Sauce/Math/RectanglePacker.h>
#include <Sauce/Math/Random.h>

#define PI 3.14159265359f

BEGIN_SAUCE_NAMESPACE

namespace math
{
	// Degrees/Radians
	SAUCE_API float radToDeg(const float rad);
	SAUCE_API float degToRad(const float deg);

	// String case functions
	SAUCE_API string toLower(string &str, const int begin = 0, const int end = 0);
	SAUCE_API string toUpper(string &str, const int begin = 0, const int end = 0);

	// Supplementary math funcs
	SAUCE_API float clamp(const float x, const float min, const float max);
	SAUCE_API float maximum(const float a, const float b);
	SAUCE_API float minimum(const float a, const float b);

	SAUCE_API float round(const float v);
	SAUCE_API Vector2F round(const Vector2F &v);
	SAUCE_API float ceil(const float v);
	SAUCE_API Vector2F ceil(const Vector2F &v);
	SAUCE_API float floor(const float v);
	SAUCE_API Vector2F floor(const Vector2F &v);
	SAUCE_API float abs(const float v);
	SAUCE_API Vector2F abs(const Vector2F &v);
	SAUCE_API float sqrt(const float v);
	SAUCE_API Vector2F sqrt(const Vector2F &v);
	SAUCE_API float pow(const float a, const float b);
	SAUCE_API Vector2F pow(const Vector2F &v);

	// Fast but inaccurate linear interpolation function
	/*template<typename T> T lerp(const T &v0, const T &v1, const float t)
	{
		return v0 * (1.0f - t) + v1 * t;
	}

	// Slow but accurate linear interpolation function
	// This will give correct values when v0 == v1, and it will give correct values when t = 0 or 1
	template<typename T> T lerp2(const T &v0, const T &v1, float t)
	{
		if(t < 0.5f)
		{
			return v0 + (v1 - v0) * t;
		}
		else
		{
			return v1 - (v1 - v0) * (1.0f - t);
		}
	}*/
	
	template<typename T> T lerp(const T &v0, const T &v1, float t)
	{
		if(t < 0.5f)
		{
			return v0 + (v1 - v0) * t;
		}
		else
		{
			return v1 - (v1 - v0) * (1.0f - t);
		}
	}

	SAUCE_API float step(const float a, const float x);

	SAUCE_API int mod(const int a, const int b);
	SAUCE_API uint ror(const uint a, const uint b);
	SAUCE_API uint rol(const uint a, const uint b);
}

END_SAUCE_NAMESPACE

#endif // SAUCE_MATH_H
