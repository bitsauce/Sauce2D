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
//
// Derived from: http://en.wikipedia.org/wiki/Xorshift

#include <Sauce/math.h>

BEGIN_SAUCE_NAMESPACE

void Random::setSeed(const uint32_t seed)
{
	m_seed = seed;
}

double Random::nextDouble()
{
	return nextInt() / 4294967295.0; // 2^32 - 1 = max int value
}

double Random::nextDouble(const double max)
{
	return nextDouble() * max;
}

double Random::nextDouble(const double min, double max)
{
	if(min > max)
	{
		return math::lerp(max, min, nextDouble());
	}
	return math::lerp(min, max, nextDouble());
}

uint32_t Random::nextInt()
{
	// xorshift
	m_seed ^= m_seed << 13;
	m_seed ^= m_seed >> 17;
	m_seed ^= m_seed << 5;
	return m_seed * 314159265;
}

uint32_t Random::nextInt(const uint32_t max)
{
	return nextInt() % (max + 1);
}

uint32_t Random::nextInt(const uint32_t min, const uint32_t max)
{
	if(min > max)
	{
		return nextInt() % (min - max + 1) + max;
	}
	return nextInt() % (max - min + 1) + min;
}

bool Random::chance(uint a, uint b)
{
	return nextDouble() < (float)a/(float)b;
}

END_SAUCE_NAMESPACE
