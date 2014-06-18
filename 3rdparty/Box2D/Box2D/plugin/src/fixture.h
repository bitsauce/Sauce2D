#ifndef B2_FIXTURE_WRAPPER_H
#define B2_FIXTURE_WRAPPER_H

#include <x2d/base.h>
#include <x2d/math.h>

class b2Fixture;

class b2FixtureWrapper
{
	friend class b2BodyWrapper;
public:
	b2FixtureWrapper(b2Fixture *fixture);

	RefCounter refCounter;
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

	void setDensity(const float density);
	void setMaskBits(const uint maskBits);
	void setCategoryBits(const uint categoryBits);

private:
	b2Fixture *m_fixture;
};

#endif // B2_FIXTURE_WRAPPER_H