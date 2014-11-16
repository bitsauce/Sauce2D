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