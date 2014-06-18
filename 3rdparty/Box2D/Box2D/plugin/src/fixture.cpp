#include "fixture.h"
#include "box2d.h"
#include "plugin.h"
#include <Box2D/Box2D.h>

b2FixtureWrapper::b2FixtureWrapper(b2Fixture *fixture) :
	m_fixture(fixture)
{
}

void b2FixtureWrapper::setDensity(const float density)
{
	m_fixture->SetDensity(density);
}

void b2FixtureWrapper::setMaskBits(const uint maskBits)
{
	b2Filter data = m_fixture->GetFilterData();
	data.maskBits = maskBits;
	m_fixture->SetFilterData(data);
}

void b2FixtureWrapper::setCategoryBits(const uint categoryBits)
{
	b2Filter data = m_fixture->GetFilterData();
	data.categoryBits = categoryBits;
	m_fixture->SetFilterData(data);
}