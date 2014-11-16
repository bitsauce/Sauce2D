#ifndef X2D_RANDOM_H
#define X2D_RANDOM_H

#include "../config.h"

class XDAPI XRandom
{
public:
	XRandom() :
		m_seed((uint32_t)time(0))
	{
	}

	void setSeed(uint32_t seed);

	double nextDouble();
	double nextDouble(double max);
	double nextDouble(double min, double max);

	uint32_t nextInt();
	uint32_t nextInt(uint32_t max);
	uint32_t nextInt(uint32_t min, uint32_t max);

	double getDouble(uint32_t offset) const;
	uint32_t getInt(uint32_t offset) const;

	bool chance(uint a, uint b); // gives a out of b chance to return true

private:
	uint32_t m_seed;

	static void DefaultConstructor(XRandom *self) { new (self) XRandom(); }
};

#endif // X2D_RANDOM_H