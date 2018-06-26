//     _____                        ______             _            
//    / ____|                      |  ____|           (_)           
//   | (___   __ _ _   _  ___ ___  | |__   _ __   __ _ _ _ __   ___ 
//    \___ \ / _` | | | |/ __/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \
//    ____) | (_| | |_| | (_|  __/ | |____| | | | (_| | | | | |  __/
//   |_____/ \__,_|\__,_|\___\___| |______|_| |_|\__, |_|_| |_|\___|
//                                                __/ |             
//                                               |___/              
// Made by Marcus "Bitsauce" Loo Vergara
// 2011-2018 (C)

#include <Sauce/Math.h>

BEGIN_SAUCE_NAMESPACE

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

Vector2F round(const Vector2F &v)
{
	return Vector2F(round(v.x), round(v.y));
}

float ceil(const float v)
{
	return ceilf(v);
}

Vector2F ceil(const Vector2F &v)
{
	return Vector2F(ceil(v.x), ceil(v.y));
}

float floor(const float v)
{
	return floorf(v);
}

Vector2F floor(const Vector2F &v)
{
	return Vector2F(floorf(v.x), floorf(v.y));
}

float abs(const float v)
{
	return ::abs(v);
}

Vector2F abs(const Vector2F &v)
{
	return Vector2F(::abs(v.x), ::abs(v.y));
}

float sqrt(const float v)
{
	return sqrtf(v);
}

Vector2F sqrt(const Vector2F &v)
{
	return Vector2F(sqrtf(v.x), sqrtf(v.y));
}

float step(float edge, float x)
{
	return x < edge ? 0.0f : 1.0f;
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

END_SAUCE_NAMESPACE
