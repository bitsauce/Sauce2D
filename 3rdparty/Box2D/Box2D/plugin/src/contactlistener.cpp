#include "contactlistener.h"

/*void ContactListener::BeginContact(b2Contact* contact)
{
	currentContact = contact;

	// Check for fixture callback
	int fixtureId = *(int*)contact->GetFixtureA()->GetUserData();
	if(fbccbs.find(fixtureId) != fbccbs.end())
	{
		scriptEngine->prepareFuncCall();
		startScriptFuncCall(fbccbs[fixtureId]);
		addScriptFuncArg(contact->GetFixtureB()->GetUserData(), 4); // 4 = int32 typeId
		endScriptFuncCall();
	}
	fixtureId = *(int*)contact->GetFixtureB()->GetUserData();
	if(fbccbs.find(fixtureId) != fbccbs.end()) {
		startScriptFuncCall(fbccbs[fixtureId]);
		addScriptFuncArg(contact->GetFixtureA()->GetUserData(), 4); // 4 = int32 typeId
		endScriptFuncCall();
	}

	// Check for body callback
	int bodyId = *(int*)contact->GetFixtureA()->GetBody()->GetUserData();
	if(bbccbs.find(bodyId) != bbccbs.end()) {
		startScriptFuncCall(bbccbs[bodyId]);
		addScriptFuncArg(contact->GetFixtureB()->GetBody()->GetUserData(), 4); // 4 = int32 typeId
		endScriptFuncCall();
	}
	bodyId = *(int*)contact->GetFixtureB()->GetBody()->GetUserData();
	if(bbccbs.find(bodyId) != bbccbs.end()) {
		startScriptFuncCall(bbccbs[bodyId]);
		addScriptFuncArg(contact->GetFixtureA()->GetBody()->GetUserData(), 4); // 4 = int32 typeId
		endScriptFuncCall();
	}

	currentContact = 0;
}

void ContactListener::EndContact(b2Contact* contact)
{
	currentContact = contact;

	// Check for fixture callback
	int fixtureId = *(int*)contact->GetFixtureA()->GetUserData();
	if(feccbs.find(fixtureId) != feccbs.end())
	{
		startScriptFuncCall(feccbs[fixtureId]);
		addScriptFuncArg(contact->GetFixtureB()->GetUserData(), 4); // 4 = int32 typeId
		endScriptFuncCall();
	}

	// Check for body callback
	int bodyId = *(int*)contact->GetFixtureA()->GetBody()->GetUserData();
	if(beccbs.find(bodyId) != beccbs.end())
	{
		startScriptFuncCall(beccbs[bodyId]);
		addScriptFuncArg(contact->GetFixtureB()->GetBody()->GetUserData(), 4); // 4 = int32 typeId
		endScriptFuncCall();
	}

	currentContact = 0;
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	currentImpulse = impulse;

	// Check for fixture callback
	int fixtureId = *(int*)contact->GetFixtureA()->GetUserData();
	if(fpscbs.find(fixtureId) != fpscbs.end())
	{
	startScriptFuncCall(fpscbs[fixtureId]);
	addScriptFuncArg(contact->GetFixtureB()->GetUserData(), 4); // 4 = int32 typeId
	endScriptFuncCall();
	}

	// Check for body callback
	int bodyId = *(int*)contact->GetFixtureA()->GetBody()->GetUserData();
	if(bpscbs.find(bodyId) != bpscbs.end())
	{
	startScriptFuncCall(bpscbs[bodyId]);
	addScriptFuncArg(contact->GetFixtureB()->GetBody()->GetUserData(), 4); // 4 = int32 typeId
	endScriptFuncCall();
	}
	
	currentImpulse = 0;
}*/