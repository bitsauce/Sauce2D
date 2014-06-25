#include "plugin.h"
#include <Box2D/Box2D.h>
#include <Box2D/Common/b2Settings.h>

#include <x2d/config.h>
#include <x2d/engine.h>
#include <x2d/math.h>
#include <x2d/scriptengine.h>
#include <x2d/scripts/array.h>
#include <x2d/scripts/anyobject.h>


#ifdef OLD
//--------------------------------------------------------------
// Box2D Contact
// NOTE: There is HUGE potential for improvements to this class
//--------------------------------------------------------------

map<int, void*> bbccbs; // Body begin contact callbacks
map<int, void*> fbccbs; // Fixture begin contact callbacks
map<int, void*> beccbs; // Body end contact callbacks
map<int, void*> feccbs; // Fixture end contact callbacks
map<int, void*> bpscbs; // Body post solve callbacks
map<int, void*> fpscbs; // Fixture post solve callbacks
const b2ContactImpulse *currentImpulse = 0;
b2Contact *currentContact = 0;

// Mapping
map<int, b2Body*> bodies;
int currBodyId = 0;

map<int, b2Fixture*> fixtures;
int currFixtureId = 0;

map<int, b2RevoluteJoint*> revoluteJoints;
int currRevoluteJointId = 0;


//--------------------------------------------------------------
// Box2D Fixture
//--------------------------------------------------------------

int createFixtureBox(int bodyId, float w, float h, float cx, float cy, float ang, float density)
{
	if(bodies.find(bodyId) == bodies.end())
		return -1;
	b2PolygonShape shape;
	shape.SetAsBox(w/(2*worldScale), h/(2*worldScale), b2Vec2(cx/worldScale, cy/worldScale), ang);
	int id = currFixtureId++;
	fixtures[id] = bodies[bodyId]->CreateFixture(&shape, density);
	fixtures[id]->SetUserData(getUserDataIdPtr(id));
	return id;
}

int createFixtureCircle(int bodyId, float r, float cx, float cy, float density)
{
	if(bodies.find(bodyId) == bodies.end())
		return -1;
	b2CircleShape shape;
	shape.m_p.Set(cx/worldScale, cy/worldScale);
	shape.m_radius = r/worldScale;
	int id = currFixtureId++;
	fixtures[id] = bodies[bodyId]->CreateFixture(&shape, density);
	fixtures[id]->SetUserData(getUserDataIdPtr(id));
	return id;
}

int createFixture(int bodyId, Array &vertices, int vertCount, float density)
{
	if(bodies.find(bodyId) == bodies.end() || vertCount <= 0 || vertCount > b2_maxPolygonVertices)
		return -1;

	// Set vertex count
	b2Vec2 *verts = new b2Vec2[vertCount];
	for(int i = 0; i < vertCount; i++)
		verts[i].Set((*((float*)vertices.At(i*2)))/worldScale, (*((float*)vertices.At(i*2+1)))/worldScale);

	// Set shape
	b2PolygonShape shape;
	shape.Set(verts, vertCount);
	delete[] verts;

	// Add fixture
	int id = currFixtureId++;
	fixtures[id] = bodies[bodyId]->CreateFixture(&shape, density);
	fixtures[id]->SetUserData(getUserDataIdPtr(id));
	return id;
}

void removeFixture(int bodyId, int fixtureId)
{
	if(bodies.find(bodyId) == bodies.end() || fixtures.find(fixtureId) == fixtures.end())
		return;
	bodies[bodyId]->DestroyFixture(fixtures[fixtureId]);
	fixtures.erase(fixtureId);
}

void setFixtureDensity(int fixtureId, float density)
{
	if(fixtures.find(fixtureId) == fixtures.end()) return;
	fixtures[fixtureId]->SetDensity(density);
}

void setFixtureMaskBits(int fixtureId, int bits)
{
	if(fixtures.find(fixtureId) == fixtures.end()) return;
	b2Filter data = fixtures[fixtureId]->GetFilterData();
	data.maskBits = bits;
	fixtures[fixtureId]->SetFilterData(data);
}

void setFixtureCategoryBits(int fixtureId, int bits)
{
	if(fixtures.find(fixtureId) == fixtures.end()) return;
	b2Filter data = fixtures[fixtureId]->GetFilterData();
	data.categoryBits = bits;
	fixtures[fixtureId]->SetFilterData(data);
}

int getFixtureMaskBits(int fixtureId)
{
	if(fixtures.find(fixtureId) == fixtures.end()) return -1;
	return fixtures[fixtureId]->GetFilterData().maskBits;
}

int getFixtureCategoryBits(int fixtureId)
{
	if(fixtures.find(fixtureId) == fixtures.end()) return -1;
	return fixtures[fixtureId]->GetFilterData().categoryBits;
}

void getFixtrueType(int fixtureId, int &type)
{
	if(fixtures.find(fixtureId) == fixtures.end()) return;
	type = fixtures[fixtureId]->GetShape()->GetType();
}

void getFixtureAABB(int fixtureId, float &x, float &y, float &w, float &h)
{
	if(fixtures.find(fixtureId) == fixtures.end()) return;
	b2AABB aabb = fixtures[fixtureId]->GetAABB(0);
	x = aabb.lowerBound.x*worldScale;
	y = aabb.lowerBound.y*worldScale;
	w = (aabb.upperBound.x-aabb.lowerBound.x)*worldScale;
	h = (aabb.upperBound.y-aabb.lowerBound.y)*worldScale;
}

void getFixtrueRadius(int fixtureId, float &r)
{
	if(fixtures.find(fixtureId) == fixtures.end() && 
		fixtures[fixtureId]->GetShape()->GetType() != b2Shape::Type::e_circle)
		return;
	r = static_cast<b2CircleShape*>(fixtures[fixtureId]->GetShape())->m_radius*worldScale;
}

//--------------------------------------------------------------
// Box2D Revolute Joint
//--------------------------------------------------------------

int createRevoluteJoint(int bodyA, int bodyB, float cx, float cy)
{
	// Check for valid bodies
	if(bodies.find(bodyA) == bodies.end() || bodies.find(bodyB) == bodies.end())
		return -1;

	// Create revolute joint
	b2RevoluteJointDef jd;
	jd.Initialize(bodies[bodyA], bodies[bodyB], b2Vec2(cx/worldScale, cy/worldScale));
	int id = currRevoluteJointId++;
	revoluteJoints[id] = static_cast<b2RevoluteJoint*>(world->CreateJoint(&jd));
	revoluteJoints[id]->SetUserData(getUserDataIdPtr(id));
	return id;
}

void removeRevoluteJoint(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	world->DestroyJoint(revoluteJoints[jointId]);
	revoluteJoints.erase(jointId);
}

void setRevoluteJoinLimits(int jointId, float lower, float upper)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->SetLimits(lower, upper);
}

void enableRevoluteJointLimits(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->EnableLimit(true);
}

void disableRevoluteJointLimits(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->EnableLimit(false);
}

void setRevoluteJointMotorSpeed(int jointId, float speed)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->SetMotorSpeed(speed);
}

float getRevoluteJointMotorSpeed(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return 0.0f;
	return revoluteJoints[jointId]->GetMotorSpeed();
}

void setRevoluteJointMaxMotorTorque(int jointId, float torque)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->SetMaxMotorTorque(torque);
}

void enableRevoluteJointMotor(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->EnableMotor(true);
}

void disableRevoluteJointMotor(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return;
	revoluteJoints[jointId]->EnableMotor(false);
}

float getRevoluteJointAngle(int jointId)
{
	if(revoluteJoints.find(jointId) == revoluteJoints.end()) return 0.0f;
	return revoluteJoints[jointId]->GetJointAngle();
}


//--------------------------------------------------------------
// Box2D Contact
//--------------------------------------------------------------

void addBodyBeginContactCallback(int bodyId, void *func)
{
	bbccbs[bodyId] = func;
}

void removeBodyBeginContactCallback(int bodyId)
{
	if(bbccbs.find(bodyId) == bbccbs.end()) return;
	releaseScriptFunc(bbccbs[bodyId]);
	bbccbs.erase(bodyId);
}

void addFixtureBeginContactCallback(int fixtureId, void *func)
{
	fbccbs[fixtureId] = func;
}

void removeFixtureBeginContactCallback(int fixtureId)
{
	if(fbccbs.find(fixtureId) == fbccbs.end()) return;
	releaseScriptFunc(fbccbs[fixtureId]);
	fbccbs.erase(fixtureId);
}

void addBodyEndContactCallback(int bodyId, void *func)
{
	beccbs[bodyId] = func;
}

void addFixtureEndContactCallback(int fixtureId, void *func)
{
	feccbs[fixtureId] = func;
}

void addBodyPostSolveCallback(int bodyId, void *func)
{
	bpscbs[bodyId] = func;
}

void addFixturePostSolveCallback(int fixtureId, void *func)
{
	fpscbs[fixtureId] = func;
}

void getContactNormalVector(float &x, float &y)
{
	if(currentImpulse == 0) return;
	b2Vec2 normal = currentContact->GetManifold()->localNormal;
	normal *= currentContact->GetManifold()->points[0].normalImpulse;
	x = normal.x*worldScale;
	y = normal.y*worldScale;
}  
#endif // OLD

#include "box2d.h"
#include "body.h"
#include "fixture.h"
#include "contact.h"

xdScriptEngine *scriptEngine = 0;

int CreatePlugin(xdScriptEngine *scriptEngine)
{
	::scriptEngine = scriptEngine;
	asIScriptEngine *asEngine = scriptEngine->getASEngine();

	int r = 0;
	
	r = scriptEngine->registerEnum("BodyType"); AS_ASSERT
	r = scriptEngine->registerSingletonType("ScriptBox2D");
	r = scriptEngine->registerValueType("b2BodyDef", sizeof(b2BodyDefWrapper)); AS_ASSERT
	r = scriptEngine->registerRefType("b2Fixture", asMETHOD(b2FixtureWrapper, addRef), asMETHOD(b2FixtureWrapper, release)); AS_ASSERT
	r = asEngine->RegisterObjectType("b2Body", 0, asOBJ_REF | asOBJ_GC); AS_ASSERT
	r = scriptEngine->registerRefType("b2Contact", asMETHOD(b2ContactWrapper, addRef), asMETHOD(b2ContactWrapper, release)); AS_ASSERT
	r = scriptEngine->registerFuncdef("void ContactFunc(b2Contact@)"); AS_ASSERT

	r = scriptEngine->registerEnumValue("BodyType", "b2_staticBody", StaticBody); AS_ASSERT
	r = scriptEngine->registerEnumValue("BodyType", "b2_kinematicBody", KinematicBody); AS_ASSERT
	r = scriptEngine->registerEnumValue("BodyType", "b2_dynamicBody", DynamicBody); AS_ASSERT
	r = scriptEngine->registerEnumValue("BodyType", "b2_bulletBody", BulletBody);  AS_ASSERT

	r = scriptEngine->registerObjectMethod("ScriptBox2D", "void step(float)", asMETHOD(Box2D, step)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "void draw()", asMETHOD(Box2D, draw)); AS_ASSERT AS_ASSERT
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "void setDrawFlags(int)", asMETHOD(Box2D, setDrawFlags)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "void set_scale(float)", asMETHOD(Box2D, setScale)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "float get_scale() const", asMETHOD(Box2D, getScale)); AS_ASSERT

	r = scriptEngine->registerObjectConstructor("b2BodyDef", "void f()", asFUNCTION(b2BodyDefWrapper::Construct)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "BodyType type", offsetof(b2BodyDefWrapper, type)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "Vector2 position", offsetof(b2BodyDefWrapper, position)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "float angle", offsetof(b2BodyDefWrapper, angle)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "Vector2 linearVelocity", offsetof(b2BodyDefWrapper, linearVelocity)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "float angularVelocity", offsetof(b2BodyDefWrapper, angularVelocity)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "float linearDamping", offsetof(b2BodyDefWrapper, linearDamping)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "float angularDamping", offsetof(b2BodyDefWrapper, angularDamping)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "bool allowSleep", offsetof(b2BodyDefWrapper, allowSleep)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "bool awake", offsetof(b2BodyDefWrapper, awake)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "bool fixedRotation", offsetof(b2BodyDefWrapper, fixedRotation)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "bool active", offsetof(b2BodyDefWrapper, active)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "bool gravityScale", offsetof(b2BodyDefWrapper, gravityScale)); AS_ASSERT
	
	r = scriptEngine->registerObjectMethod("b2Fixture", "void setDensity(const float)", asMETHOD(b2FixtureWrapper, setDensity)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Fixture", "void setMaskBits(const uint)", asMETHOD(b2FixtureWrapper, setMaskBits)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Fixture", "void setCategoryBits(const uint)", asMETHOD(b2FixtureWrapper, setCategoryBits)); AS_ASSERT
	
	r = asEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_ADDREF, "void f()", asMETHOD(b2BodyWrapper, addRef), asCALL_THISCALL); AS_ASSERT
	r = asEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_RELEASE, "void f()", asMETHOD(b2BodyWrapper, release), asCALL_THISCALL); AS_ASSERT
	r = asEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_SETGCFLAG, "void f()", asMETHOD(b2BodyWrapper, setGCFlag), asCALL_THISCALL); AS_ASSERT
	r = asEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(b2BodyWrapper, getGCFlag), asCALL_THISCALL); AS_ASSERT
	r = asEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(b2BodyWrapper, getRefCount), asCALL_THISCALL); AS_ASSERT
	r = asEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(b2BodyWrapper, enumReferences), asCALL_THISCALL); AS_ASSERT
	r = asEngine->RegisterObjectBehaviour("b2Body", asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(b2BodyWrapper, releaseReferences), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->registerObjectFactory("b2Body", "b2Body @f(const b2BodyDef &in)", asFUNCTION(b2BodyWrapper::Factory)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void destroy()", asMETHOD(b2BodyWrapper, destroy)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "b2Fixture @createFixture(const Rect &in, float)", asMETHODPR(b2BodyWrapper, createFixture, (const Rect&, float), b2FixtureWrapper*)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "b2Fixture @createFixture(const Vector2 &in, const float, float)", asMETHODPR(b2BodyWrapper, createFixture, (const Vector2&, const float, float), b2FixtureWrapper*)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "b2Fixture @createFixture(array<Vector2> &in, float density)", asMETHODPR(b2BodyWrapper, createFixture, (Array*, float), b2FixtureWrapper*)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void removeFixture(b2Fixture @)", asMETHOD(b2BodyWrapper, removeFixture)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void setTransform(const Vector2 &in, float)", asMETHOD(b2BodyWrapper, setTransform)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void setPosition(const Vector2 &in)", asMETHOD(b2BodyWrapper, setPosition)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void setAngle(const Vector2 &in)", asMETHOD(b2BodyWrapper, setAngle)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void setBeginContactCallback(ContactFunc@)", asMETHOD(b2BodyWrapper, setBeginContactCallback)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void setEndContactCallback(ContactFunc@)", asMETHOD(b2BodyWrapper, setEndContactCallback)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void setPreSolveCallback(ContactFunc@)", asMETHOD(b2BodyWrapper, setPreSolveCallback)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void setPostSolveCallback(ContactFunc@)", asMETHOD(b2BodyWrapper, setPostSolveCallback)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void setObject(?&in)", asMETHOD(b2BodyWrapper, setObject)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "bool getObject(?&out)", asMETHOD(b2BodyWrapper, getObject)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "Vector2 getPosition() const", asMETHOD(b2BodyWrapper, getPosition)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "Vector2 getCenter() const", asMETHOD(b2BodyWrapper, getPosition)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "Vector2 getLinearVelocity() const", asMETHOD(b2BodyWrapper, getLinearVelocity)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "float getAngle() const", asMETHOD(b2BodyWrapper, getAngle)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void applyImpulse(const Vector2 &in, const Vector2 &in)", asMETHOD(b2BodyWrapper, applyImpulse)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void setLinearVelocity(const Vector2 &in)", asMETHOD(b2BodyWrapper, setLinearVelocity)); AS_ASSERT

	r = scriptEngine->registerObjectMethod("b2Contact", "void setEnabled(bool)", asMETHOD(b2ContactWrapper, setEnabled)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Contact", "b2Body @get_other() const", asMETHOD(b2ContactWrapper, getOtherBody)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Contact", "b2Body @get_this() const", asMETHOD(b2ContactWrapper, getThisBody)); AS_ASSERT

	b2d = new Box2D;
	r = scriptEngine->registerGlobalProperty("ScriptBox2D Box2D", b2d);
	
#ifdef OLD
	// b2d revolute joint
	registerGlobalFunction("int  b2dCreateRevoluteJoint(int bodyA, int bodyB, float cx, float cy)", asFUNCTION(createRevoluteJoint));
	registerGlobalFunction("void b2dRemoveRevoluteJoint(int jointId)", asFUNCTION(removeRevoluteJoint));
	registerGlobalFunction("void b2dSetRevoluteJoinLimits(int jointId, float lower, float upper)", asFUNCTION(setRevoluteJoinLimits));
	registerGlobalFunction("void b2dEnableRevoluteJointLimits(int jointId)", asFUNCTION(enableRevoluteJointLimits));
	registerGlobalFunction("void b2dDisableRevoluteJointLimits(int jointId)", asFUNCTION(disableRevoluteJointLimits));
	registerGlobalFunction("void b2dSetRevoluteJointMotorSpeed(int jointId, float speed)", asFUNCTION(setRevoluteJointMotorSpeed));
	registerGlobalFunction("float b2dGetRevoluteJointMotorSpeed(int jointId)", asFUNCTION(getRevoluteJointMotorSpeed));
	registerGlobalFunction("void b2dSetRevoluteJointMaxMotorTorque(int jointId, float speed)", asFUNCTION(setRevoluteJointMaxMotorTorque));
	registerGlobalFunction("void b2dEnableRevoluteJointMotor(int jointId)", asFUNCTION(enableRevoluteJointMotor));
	registerGlobalFunction("void b2dDisableRevoluteJointMotor(int jointId)", asFUNCTION(disableRevoluteJointMotor));
	registerGlobalFunction("float b2dGetRevoluteJointAngle(int jointId)", asFUNCTION(getRevoluteJointAngle));
#endif

	return r;
}

void ReleasePlugin()
{
	delete b2d;
}