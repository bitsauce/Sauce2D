#ifndef SAUCE_RANDOM_H
#define SAUCE_RANDOM_H

#include <Sauce/Config.h>

BEGIN_SAUCE_NAMESPACE

class SAUCE_API Random
{
public:
	Random() :
		m_seed((uint32_t) time(0))
	{
	}

	Random(const uint32_t seed) :
		m_seed(seed)
	{
	}

	void setSeed(const uint32_t seed);
	uint32_t getSeed() const { return m_seed; }

	double nextDouble();
	double nextDouble(const double max);
	double nextDouble(const double min, const double max);

	uint32_t nextInt();
	uint32_t nextInt(const uint32_t max);
	uint32_t nextInt(const uint32_t min, const uint32_t max);

	bool chance(uint a, uint b); // has an 'a' out of 'b' chance to return true

private:
	uint32_t m_seed;
};

class XORHash
{
public:
	XORHash() :
		m_seed((uint32_t)time(0))
	{
	}

	XORHash(const uint32_t seed) :
		m_seed(seed)
	{
	}

	template <typename Head, typename ...Tail>
	uint32_t hash(Head head, Tail&&... tail)
	{
		head += m_seed;
		head ^= head << 13;
		head ^= head >> 17;
		head ^= head << 5;
		head *= 314159265;
		return xorhash_internal(head, std::forward<Tail>(tail)...) ^ head;
	}

	template <typename ...List>
	double prob(List&&... tail)
	{
		return hash(std::forward<List>(tail)...) / 4294967295.0;
	}

private:
	template <typename Head>
	uint32_t xorhash_internal(Head)
	{
		return 0;
	}

	template <typename Head, typename ...Tail>
	uint32_t xorhash_internal(Head prevHead, Head head, Tail&&... tail)
	{
		head += m_seed;
		head ^= prevHead;
		head ^= head << 13;
		head ^= head >> 17;
		head ^= head << 5;
		head *= 314159265;
		return xorhash_internal(head, std::forward<Tail>(tail)...) ^ head;
	}

	const uint32_t m_seed;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_RANDOM_H
