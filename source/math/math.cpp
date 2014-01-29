//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/              
//		Macro#byte (C)

#include "x2d/math.h"

#include <angelscript.h>
#include <cmath>
#include <sstream>

#define PI 3.14159265359f

void RegisterMath()
{
	// Register value types
	RegisterVectors();
	RegisterRect2();
}

int math::strToInt(const string &str)
{
	return atoi(str.c_str());
}

float math::strToFloat(const string &str)
{
	return atof(str.c_str());
}

uchar math::strToAscii(const string &str)
{
	return str.size() != 0 ? str[0] : 0;
}

bool math::strToBool(const string &str)
{
	return (str == "true" || str == "TRUE" || str == "1") ? true : false;
}

string math::intToStr(const int value)
{
	stringstream ss;
	ss << value;    
	return ss.str();
}

string math::floatToStr(const float value)
{
	stringstream ss;
	ss << value;    
	return ss.str();
}

string math::boolToStr(const bool value)
{
	return value ? "true" : "false";
}

string math::asciiToStr(const uchar value)
{
	string s;
	s += value;
	return s;
}

float math::getRandomFloat()
{
#if defined(X2D_LINUX)
	return (float)drand48();
#elif defined(X2D_WINDOWS)
	return rand();
#endif
}

int math::getRandomInt()
{
#if defined(X2D_LINUX)
	return (int)lrand48();
#elif defined(X2D_WINDOWS)
	return rand();
#endif
}

int math::getRandomInt(int start, int end)
{
	// Start shouldn't be larger than end
	if(start > end)
	{
		int temp = end;
		end = start;
		start = temp;
	}
	
	// Only accept nubers small enought for our rand func
	if(end > RAND_MAX)
		return -1;

#if defined(X2D_WINDOWS)
	return (rand() % ((end+1)-start) + start);
#else defined(X2D_LINUX)
	return ((int)lrand48() % ((end+1)-start) + start);
#endif
}

void math::seedRandom(const uint seed)
{
#if defined(X2D_WINDOWS)
	srand(seed);
#else defined(X2D_LINUX)
	srand48(seed);
#endif
}

float math::radToDeg(const float rad)
{
	return rad*(180.0f/PI);
}

float math::degToRad(const float deg)
{
	return deg*(PI/180.0f);
}

string math::toUpper(string &str, const int begin, const int end)
{
	if(begin < 0 || end < 0 || begin >= end)
		transform(str.begin(), str.end(), str.begin(), ::toupper);
	else
		transform(str.begin()+begin, str.begin()+end, str.begin(), ::toupper);
	return str;
}

string math::toLower(string &str, const int begin, const int end)
{
	if(begin < 0 || end < 0 || begin >= end)
		transform(str.begin(), str.end(), str.begin(), ::tolower);
	else
		transform(str.begin()+begin, str.begin()+end, str.begin(), ::tolower);
	return str;
}

float math::clampValue(const float min, const float max, const float x)
{
	return x < min ? min : (x > max ? max : x);
}

float math::maximum(const float a, const float b)
{
	return (a > b ? a : b);
}

float math::minimum(const float a, const float b)
{
	return (a < b ? a : b);
}

void math::setEnumValue(int *out, int typeId, int val)
{
	// Enum check
	if(!(typeId & asTYPEID_MASK_SEQNBR))
		return;

	// Set out value
	*out = val;
}

#ifdef DEPRICATED

float round(const float r)
{
    return (r > 0.0f) ? floor(r + 0.5f) : ceil(r - 0.5f);
}

#endif