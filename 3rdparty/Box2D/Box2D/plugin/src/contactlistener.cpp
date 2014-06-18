#include "contactlistener.h"
#include "contact.h"
#include "body.h"

void ContactListener::BeginContact(b2Contact* b2c)
{
	b2ContactWrapper *contact = new b2ContactWrapper(b2c,
		(b2BodyWrapper*)b2c->GetFixtureA()->GetBody()->GetUserData(),
		(b2BodyWrapper*)b2c->GetFixtureB()->GetBody()->GetUserData());
	contact->call(b2ContactWrapper::BeginContact);
	contact->swapBodies();
	contact->call(b2ContactWrapper::BeginContact);
	contact->release();
}

void ContactListener::EndContact(b2Contact* b2c)
{
	/*b2ContactWrapper contact(b2c);
	contact.m_thisBody = (b2BodyWrapper*)b2c->GetFixtureA()->GetBody()->GetUserData();
	contact.m_otherBody = (b2BodyWrapper*)b2c->GetFixtureB()->GetBody()->GetUserData();
	contact.call(b2ContactWrapper::EndContact);
	contact.swapBodies();
	contact.call(b2ContactWrapper::EndContact);*/
}

void ContactListener::PreSolve(b2Contact* b2c, const b2Manifold* oldManifold)
{
	/*b2ContactWrapper contact(b2c);
	contact.m_thisBody = (b2BodyWrapper*)b2c->GetFixtureA()->GetBody()->GetUserData();
	contact.m_otherBody = (b2BodyWrapper*)b2c->GetFixtureB()->GetBody()->GetUserData();
	contact.call(b2ContactWrapper::PreSolve);
	contact.swapBodies();
	contact.call(b2ContactWrapper::PreSolve);*/
}

void ContactListener::PostSolve(b2Contact* b2c, const b2ContactImpulse* impulse)
{
	/*b2ContactWrapper contact(b2c);
	contact.m_thisBody = (b2BodyWrapper*)b2c->GetFixtureA()->GetBody()->GetUserData();
	contact.m_otherBody = (b2BodyWrapper*)b2c->GetFixtureB()->GetBody()->GetUserData();
	contact.call(b2ContactWrapper::PostSolve);
	contact.swapBodies();
	contact.call(b2ContactWrapper::PostSolve);*/
}