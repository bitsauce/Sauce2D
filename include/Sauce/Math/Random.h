#ifndef SAUCE_RANDOM_H
#define SAUCE_RANDOM_H

#include <Sauce/config.h>

BEGIN_SAUCE_NAMESPACE

class SAUCE_API Random
{
public:
	Random() :
		m_seed((uint32_t)time(0))
	{
	}

	void setSeed(uint32_t seed);
	uint32_t getSeed() const { return m_seed; }

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
};

END_SAUCE_NAMESPACE

#endif // SAUCE_RANDOM_H
