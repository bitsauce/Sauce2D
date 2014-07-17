//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/math.h>
#include <angelscript.h>
#include <cmath>

AS_REG_SINGLETON(xdMath, "ScriptMath")

int xdMath::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	// Type Conversion
	/*r = scriptEngine->RegisterObjectMethod("ScriptMath", "string intToStr(const int)", asMETHOD(xdMath, intToStr), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "string floatToStr(const float)", asMETHOD(xdMath, floatToStr), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "string boolToStr(const bool)", asMETHOD(xdMath, boolToStr), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "int strToInt(const string &in)", asMETHOD(xdMath, strToInt), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float strToFloat(const string &in)", asMETHOD(xdMath, strToFloat), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "bool strToBool(const string &in)", asMETHOD(xdMath, strToBool), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "string strToAscii(const uint8)", asMETHOD(xdMath, strToAscii), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "uint8 asciiToStr(const string &in)", asMETHOD(xdMath, asciiToStr), asCALL_THISCALL); AS_ASSERT*/
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "void setEnumValue(?&out, const int value)", asMETHOD(xdMath, setEnumValue), asCALL_THISCALL); AS_ASSERT

	// Supplementary math funcs
	/*r = scriptEngine->RegisterObjectMethod("ScriptMath", "string tolower(string &in, const int begin = 0, const int end = 0)", asMETHOD(xdMath, toLower), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "string toupper(string &in, const int begin = 0, const int end = 0)", asMETHOD(xdMath, toUpper), asCALL_THISCALL); AS_ASSERT*/
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float round(const float)", asMETHOD(xdMath, round), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float ceil(const float)", asMETHOD(xdMath, ceil), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float floor(const float)", asMETHOD(xdMath, floor), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float clamp(const float, const float, const float)", asMETHOD(xdMath, clamp), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float max(const float, const float)", asMETHOD(xdMath, maximum), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float min(const float, const float)", asMETHOD(xdMath, minimum), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float abs(const float)", asMETHOD(xdMath, abs), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float sqrt(const float)", asMETHOD(xdMath, sqrt), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float pow(const float, const float)", asMETHOD(xdMath, pow), asCALL_THISCALL); AS_ASSERT
	
	// Trigonometry
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float cos(const float)", asMETHOD(xdMath, cos), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float sin(const float)", asMETHOD(xdMath, sin), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float tan(const float)", asMETHOD(xdMath, tan), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float acos(const float)", asMETHOD(xdMath, acos), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float asin(const float)", asMETHOD(xdMath, asin), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float atan(const float)", asMETHOD(xdMath, atan), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float get_PI() const", asMETHOD(xdMath, getPI), asCALL_THISCALL); AS_ASSERT

	// Random
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "void seedRandom(const uint)", asMETHOD(xdMath, seedRandom), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "int getRandomInt()", asMETHODPR(xdMath, getRandomInt, (), int), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "int getRandomInt(const int, const int)", asMETHODPR(xdMath, getRandomInt, (int, int), int), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float getRandomFloat()", asMETHOD(xdMath, getRandomFloat), asCALL_THISCALL); AS_ASSERT

	return r;
}

void xdMath::setEnumValue(int *out, int typeId, int val) 
{
	if(typeId & asTYPEID_MASK_SEQNBR)
		*out = val;
}

float xdMath::getRandomFloat()
{
#if defined(X2D_LINUX)
	return (float)drand48();
#elif defined(X2D_WINDOWS)
	return (float)rand()/RAND_MAX;
#endif
}

int xdMath::getRandomInt()
{
#if defined(X2D_LINUX)
	return (int)lrand48();
#elif defined(X2D_WINDOWS)
	return rand();
#endif
}

int xdMath::getRandomInt(int start, int end)
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
	return (rand() % (end-start+1) + start);
#else defined(X2D_LINUX)
	return ((int)lrand48() % (end-start+1) + start);
#endif
}

void xdMath::seedRandom(const uint seed)
{
#if defined(X2D_WINDOWS)
	srand(seed);
#else defined(X2D_LINUX)
	srand48(seed);
#endif
}

float xdMath::radToDeg(const float rad)
{
	return rad*(180.0f/PI);
}

float xdMath::degToRad(const float deg)
{
	return deg*(PI/180.0f);
}

float xdMath::clamp(const float min, const float max, const float x)
{
	return x < min ? min : (x > max ? max : x);
}

float xdMath::maximum(const float a, const float b)
{
	return (a > b ? a : b);
}

float xdMath::minimum(const float a, const float b)
{
	return (a < b ? a : b);
}