//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/math.h>

AS_REG_SINGLETON(XMath)

int XMath::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	// Type Conversion
	/*r = scriptEngine->RegisterObjectMethod("ScriptMath", "string intToStr(const int)", asMETHOD(XMath, intToStr), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "string floatToStr(const float)", asMETHOD(XMath, floatToStr), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "string boolToStr(const bool)", asMETHOD(XMath, boolToStr), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "int strToInt(const string &in)", asMETHOD(XMath, strToInt), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float strToFloat(const string &in)", asMETHOD(XMath, strToFloat), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "bool strToBool(const string &in)", asMETHOD(XMath, strToBool), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "string strToAscii(const uint8)", asMETHOD(XMath, strToAscii), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "uint8 asciiToStr(const string &in)", asMETHOD(XMath, asciiToStr), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "void setEnumValue(?&out, const int value)", asMETHOD(XMath, setEnumValue), asCALL_THISCALL); AS_ASSERT*/

	// Supplementary math funcs
	/*r = scriptEngine->RegisterObjectMethod("ScriptMath", "string tolower(string &in, const int begin = 0, const int end = 0)", asMETHOD(XMath, toLower), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "string toupper(string &in, const int begin = 0, const int end = 0)", asMETHOD(XMath, toUpper), asCALL_THISCALL); AS_ASSERT*/
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float round(const float)", asMETHOD(XMath, round), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float ceil(const float)", asMETHOD(XMath, ceil), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float floor(const float)", asMETHOD(XMath, floor), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float clamp(const float, const float, const float)", asMETHOD(XMath, clamp), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float max(const float, const float)", asMETHOD(XMath, maximum), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float min(const float, const float)", asMETHOD(XMath, minimum), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float abs(const float)", asMETHOD(XMath, abs), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float sqrt(const float)", asMETHOD(XMath, sqrt), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float pow(const float, const float)", asMETHOD(XMath, pow), asCALL_THISCALL); AS_ASSERT
	
	// Trigonometry
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float cos(const float)", asMETHOD(XMath, cos), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float sin(const float)", asMETHOD(XMath, sin), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float tan(const float)", asMETHOD(XMath, tan), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float acos(const float)", asMETHOD(XMath, acos), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float asin(const float)", asMETHOD(XMath, asin), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float atan(const float)", asMETHOD(XMath, atan), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float atan2(const float, const float)", asMETHOD(XMath, atan2), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float get_PI() const", asMETHOD(XMath, getPI), asCALL_THISCALL); AS_ASSERT

	// Random
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "void seedRandom(const uint)", asMETHOD(XMath, seedRandom), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "int getRandomInt()", asMETHODPR(XMath, getRandomInt, (), int), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "int getRandomInt(const int, const int)", asMETHODPR(XMath, getRandomInt, (int, int), int), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptMath", "float getRandomFloat()", asMETHOD(XMath, getRandomFloat), asCALL_THISCALL); AS_ASSERT

	return r;
}

float XMath::getRandomFloat()
{
#if defined(X2D_LINUX)
	return (float)drand48();
#elif defined(X2D_WINDOWS)
	return (float)rand()/RAND_MAX;
#endif
}

int XMath::getRandomInt()
{
#if defined(X2D_LINUX)
	return (int)lrand48();
#elif defined(X2D_WINDOWS)
	return rand();
#endif
}

int XMath::getRandomInt(int start, int end)
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

void XMath::seedRandom(const uint seed)
{
#if defined(X2D_WINDOWS)
	srand(seed);
#else defined(X2D_LINUX)
	srand48(seed);
#endif
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