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
	r *= c; g *= c; b *= c; a *= c;
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

float round(const float value)
{
	return (float)int((value > 0.0f) ? (value + 0.5f) : (value - 0.5f));
}

float ceil(const float value)
{
	return ceilf(value);
}

float floor(const float value)
{
	return floorf(value);
}

float abs(const float value)
{
	return ::abs(value);
}

float sqrt(const float value)
{
	return sqrtf(value);
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

float lerp(const float v0, const float v1, const float t)
{
	return (1.0f-t)*v0 + t*v1;
}

}

END_XD_NAMESPACE