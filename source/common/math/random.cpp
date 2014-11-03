//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)
// Inspired by: http://en.wikipedia.org/wiki/Xorshift

#include <x2d/math.h>

AS_REG_POD(XRandom, "Random")

int XRandom::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("Random", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor), asCALL_CDECL_OBJLAST); AS_ASSERT
		
	r = scriptEngine->RegisterObjectMethod("Random", "void setSeed(uint)", asMETHOD(XRandom, setSeed), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Random", "double nextDouble()", asMETHODPR(XRandom, nextDouble, (), double), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Random", "double nextDouble(double)", asMETHODPR(XRandom, nextDouble, (double), double), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Random", "double nextDouble(double, double)", asMETHODPR(XRandom, nextDouble, (double, double), double), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Random", "uint nextInt()", asMETHODPR(XRandom, nextInt, (), uint), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Random", "uint nextInt(uint)", asMETHODPR(XRandom, nextInt, (uint), uint), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Random", "uint nextInt(uint, uint)", asMETHODPR(XRandom, nextInt, (uint, uint), uint), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Random", "double getDouble(uint = 0) const", asMETHOD(XRandom, getDouble), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Random", "uint getInt(uint = 0) const", asMETHOD(XRandom, getInt), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Random", "bool chance(uint, uint)", asMETHOD(XRandom, chance), asCALL_THISCALL); AS_ASSERT

	return r;
}

void XRandom::setSeed(uint32_t seed)
{
	m_seed = seed;
}

double XRandom::nextDouble()
{
	// We have to cast to double because floats can only represent integers up to 2^23
	return nextInt() / 4294967295.0; // 2^32 - 1
}

double XRandom::nextDouble(double max)
{
	return nextDouble() * max;
}

double XRandom::nextDouble(double min, double max)
{
	double t = nextDouble();
	return (1.0f-t)*min + t*max;
}

uint32_t XRandom::nextInt()
{
	m_seed ^= m_seed << 13;
	m_seed ^= m_seed >> 17;
	m_seed ^= m_seed << 5;
	return m_seed * 314159265;
}

uint32_t XRandom::nextInt(uint32_t max)
{
	return nextInt() % (max+1);
}

uint32_t XRandom::nextInt(uint32_t min, uint32_t max)
{
	if(min > max)
		return nextInt() % (min-max+1) + max;
	return nextInt() % (max-min+1) + min;
}

bool XRandom::chance(uint a, uint b)
{
	return nextDouble() < (float)a/(float)b;
}

double XRandom::getDouble(uint32_t offset) const
{
	return getInt(offset) / 4294967295.0; // 2^32 - 1
}

uint32_t XRandom::getInt(uint32_t offset) const
{
	offset ^= offset << 13;
	offset ^= offset >> 17;
	offset ^= offset << 5;
	return offset * 314159265;
}