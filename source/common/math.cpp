//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/math.h>

namespace xd {
Color &Color::operator*(const float c)
{
	r *= c; g *= c; b *= c; a *= c;
	return *this;
}
}

float XMath::radToDeg(const float rad)
{
	return rad*(180.0f/PI);
}

float XMath::degToRad(const float deg)
{
	return deg*(PI/180.0f);
}

float XMath::clamp(const float x, const float min, const float max)
{
	return x < min ? min : (x > max ? max : x);
}

float XMath::maximum(const float a, const float b)
{
	return (a > b ? a : b);
}

float XMath::minimum(const float a, const float b)
{
	return (a < b ? a : b);
}

uint XMath::ror(const uint a, const uint b)
{
	return (a << b) | (a >> (sizeof(uint)*CHAR_BIT-b));
}

uint XMath::rol(const uint a, const uint b)
{
	return (a >> b) | (a << (sizeof(uint)*CHAR_BIT-b));
}