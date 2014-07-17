#include "contact.h"
#include "box2d.h"
#include "body.h"
#include "fixture.h"
#include "plugin.h"
#include <Box2D/Box2D.h>
#include <x2d/scripts/funccall.h>

b2ContactWrapper::b2ContactWrapper(b2Contact *contact, b2FixtureWrapper *fixtureA, b2FixtureWrapper *fixtureB) :
	m_fixtureA(fixtureA),
	m_fixtureB(fixtureB),
	m_contact(contact)
{
	m_fixtureA->addRef();
	m_fixtureB->addRef();
}

b2ContactWrapper::~b2ContactWrapper()
{
	m_fixtureA->release();
	m_fixtureB->release();
}

void b2ContactWrapper::setEnabled(bool enabled)
{
	m_contact->SetEnabled(enabled);
}

bool b2ContactWrapper::isTouching() const
{
	return m_contact->IsTouching();
}

void b2ContactWrapper::setFriction(float friction)
{
	m_contact->SetFriction(friction);
}

float b2ContactWrapper::getFriction() const
{
	return m_contact->GetFriction();
}

void b2ContactWrapper::resetFriction()
{
	m_contact->ResetFriction();
}

void b2ContactWrapper::setRestitution(float restitution)
{
	m_contact->SetRestitution(restitution);
}

float b2ContactWrapper::getRestitution() const
{
	return m_contact->GetRestitution();
}

void b2ContactWrapper::resetRestitution()
{
	m_contact->ResetRestitution();
}

void b2ContactWrapper::swapAB()
{
	b2FixtureWrapper *tmpFixture = m_fixtureA;
	m_fixtureA = m_fixtureB;
	m_fixtureB = tmpFixture;
}

void b2ContactWrapper::call(ContactType type)
{
	b2BodyWrapper *bodyA = m_fixtureA->getBody();
	if(!bodyA)
		return;

	asIScriptFunction *func = 0;
	switch(type)
	{
	case BeginContact: func = bodyA->m_beginContactFunc; break;
	case EndContact: func = bodyA->m_endContactFunc; break;
	case PreSolve: func = bodyA->m_preSolveFunc; break;
	case PostSolve: func = bodyA->m_postSolveFunc; break;
	}

	if(func)
	{
		FunctionCall *funcCall = CreateFuncCall();
		funcCall->Prepare(func);
		b2ContactWrapper *self = this;
		addRef();
		funcCall->SetArgument(0, &self, 0x40000000);
		funcCall->Execute();
	}
	bodyA->release();
}

b2BodyWrapper *b2ContactWrapper::getBodyA() const
{
	return m_fixtureA->getBody();
}

b2BodyWrapper *b2ContactWrapper::getBodyB() const
{
	return m_fixtureB->getBody();
}

b2FixtureWrapper *b2ContactWrapper::getFixtureA() const
{
	m_fixtureA->addRef();
	return m_fixtureA;
}

b2FixtureWrapper *b2ContactWrapper::getFixtureB() const
{
	m_fixtureB->addRef();
	return m_fixtureB;
}