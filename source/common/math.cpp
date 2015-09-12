//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/math.h>

BEGIN_XD_NAMESPACE

Color &Color::operator*(const float c)
{
	r = (uchar) (r * c);
	g = (uchar) (g * c);
	b = (uchar) (b * c);
	a = (uchar) (a * c);
	return *this;
}

namespace math {

float radToDeg(const float rad)
{
	return rad*(180.0f/PI);
}

float degToRad(const float deg)
{
	return deg*(PI/180.0f);
}

float clamp(const float x, const float min, const float max)
{
	return x < min ? min : (x > max ? max : x);
}

float maximum(const float a, const float b)
{
	return (a > b ? a : b);
}

float minimum(const float a, const float b)
{
	return (a < b ? a : b);
}

uint ror(const uint a, const uint b)
{
	return (a << b) | (a >> (sizeof(uint)*CHAR_BIT-b));
}

uint rol(const uint a, const uint b)
{
	return (a >> b) | (a << (sizeof(uint)*CHAR_BIT-b));
}

float round(const float v)
{
	return (float)int((v > 0.0f) ? (v + 0.5f) : (v - 0.5f));
}

Vector2 round(const Vector2 &v)
{
	return Vector2(round(v.x), round(v.y));
}

float ceil(const float v)
{
	return ceilf(v);
}

Vector2 ceil(const Vector2 &v)
{
	return Vector2(ceil(v.x), ceil(v.y));
}

float floor(const float v)
{
	return floorf(v);
}

Vector2 floor(const Vector2 &v)
{
	return Vector2(floorf(v.x), floorf(v.y));
}

float abs(const float v)
{
	return ::abs(v);
}

Vector2 abs(const Vector2 &v)
{
	return Vector2(::abs(v.x), ::abs(v.y));
}

float sqrt(const float v)
{
	return sqrtf(v);
}

Vector2 sqrt(const Vector2 &v)
{
	return Vector2(sqrtf(v.x), sqrtf(v.y));
}

float lerp(const float v0, const float v1, const float t)
{
	return v0 * (1.0f - t) + v1 * t;
}

Vector2 lerp(const Vector2 &v0, const Vector2 &v1, const float t)
{
	return v0 * (1.0f - t) + v1 * t;
}

float pow(const float a, const float b)
{
	return powf(a, b);
}

int mod(const int a, const int b)
{
	int r = a % b;
	return r < 0 ? r + b : r;
}

}

END_XD_NAMESPACE