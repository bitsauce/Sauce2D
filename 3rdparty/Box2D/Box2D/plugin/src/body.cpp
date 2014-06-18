#include "body.h"
#include "fixture.h"
#include "box2d.h"
#include "plugin.h"
#include <x2d/scripts/anyobject.h>
#include <x2d/scripts.h>
#include <Box2D/Box2D.h>

b2BodyDefWrapper::b2BodyDefWrapper() :
	type(StaticBody),
	position(0.0f),
	angle(0.0f),
	linearVelocity(0.0f),
	angularVelocity(0.0f),
	linearDamping(0.0f),
	angularDamping(0.0f),
	allowSleep(true),
	awake(true),
	fixedRotation(false),
	active(true),
	gravityScale(1.0f)
{
}

b2BodyDef b2BodyDefWrapper::getBodyDef() const
{
	b2BodyDef def;
	def.type = (type != BulletBody ? b2BodyType(type) : b2_dynamicBody);
	def.position = toB2Vec(position);
	def.angle = angle;
	def.linearVelocity = toB2Vec(linearVelocity);
	def.angularVelocity = angularVelocity;
	def.linearDamping = linearDamping;
	def.angularDamping = angularDamping;
	def.allowSleep = allowSleep;
	def.awake = awake;
	def.fixedRotation = fixedRotation;
	def.bullet = (type == BulletBody);
	def.active = active;
	def.gravityScale = gravityScale;
	return def;
}

b2BodyWrapper::b2BodyWrapper(b2Body *body) :
	m_body(body),
	m_beginContactFunc(0),
	m_endContactFunc(0),
	m_preSolveFunc(0),
	m_postSolveFunc(0)
{
	//m_object = CreateAnyObject();
	m_body->SetUserData(this);
}

b2BodyWrapper::~b2BodyWrapper()
{
	//m_object->Release();
	for(vector<b2FixtureWrapper*>::iterator itr = m_fixtures.begin(); itr != m_fixtures.end(); ++itr) {
		(*itr)->release();
	}
	b2d->getWorld()->DestroyBody(m_body);
	scriptEngine->releaseScriptFunc(m_beginContactFunc);
	scriptEngine->releaseScriptFunc(m_endContactFunc);
	scriptEngine->releaseScriptFunc(m_preSolveFunc);
	scriptEngine->releaseScriptFunc(m_postSolveFunc);
}

b2FixtureWrapper *b2BodyWrapper::createFixture(const Rect &rect, float density)
{
	b2PolygonShape shape;
	b2Vec2 halfSize = toB2Vec(rect.getSize()/2.0f);
	shape.SetAsBox(halfSize.x, halfSize.y, toB2Vec(rect.getCenter()), 0.0f);
	return new b2FixtureWrapper(m_body->CreateFixture(&shape, density));
}
	
b2FixtureWrapper *b2BodyWrapper::createFixture(const Vector2 &center, const float radius, float density)
{
	b2CircleShape shape;
	shape.m_p = toB2Vec(center);
	shape.m_radius = radius/b2d->getScale();
	return new b2FixtureWrapper(m_body->CreateFixture(&shape, density));
}
	
b2FixtureWrapper *b2BodyWrapper::createFixture(Array *arr, float density)
{
	if(arr->GetSize() > b2_maxPolygonVertices)
		return 0;

	// Set vertex count
	b2Vec2 *verts = new b2Vec2[arr->GetSize()];
	for(uint i = 0; i < arr->GetSize(); i++) {
		verts[i] = toB2Vec(*(Vector2*)arr->At(i));
	}

	// Set shape
	b2PolygonShape shape;
	shape.Set(verts, arr->GetSize());
	delete[] verts;

	// Add fixture
	return new b2FixtureWrapper(m_body->CreateFixture(&shape, density));
}

void b2BodyWrapper::removeFixture(b2FixtureWrapper *fixture)
{
	m_body->DestroyFixture(fixture->m_fixture);
	vector<b2FixtureWrapper*>::iterator itr;
	if((itr = find(m_fixtures.begin(), m_fixtures.end(), fixture)) != m_fixtures.end()) {
		(*itr)->release();
	}
}

void b2BodyWrapper::setTransform(const Vector2 &position, float angle)
{
	m_body->SetTransform(toB2Vec(position), angle);
}

void b2BodyWrapper::setPosition(const Vector2 &position)
{
	m_body->SetTransform(toB2Vec(position), m_body->GetAngle());
}

void b2BodyWrapper::setAngle(float angle)
{
	m_body->SetTransform(m_body->GetPosition(), angle);
}

void b2BodyWrapper::setBeginContactCallback(void *func)
{
	m_beginContactFunc = func;
}

void b2BodyWrapper::setEndContactCallback(void *func)
{
	m_endContactFunc = func;
}

void b2BodyWrapper::setPreSolveCallback(void *func)
{
	m_preSolveFunc = func;
}

void b2BodyWrapper::setPostSolveCallback(void *func)
{
	m_postSolveFunc = func;
}

void b2BodyWrapper::setObject(void *object, int typeId)
{
	//m_object->Store(object, typeId);
}

bool b2BodyWrapper::getObject(void *object, int typeId)
{
	return false;//m_object->Retrieve(object, typeId);
}

Vector2 b2BodyWrapper::getPosition() const
{
	return toXDVec(m_body->GetPosition());
}

float b2BodyWrapper::getAngle() const
{
	return m_body->GetAngle();
}

void b2BodyWrapper::applyImpulse(const Vector2 &impulse, const Vector2 &position)
{
	m_body->ApplyLinearImpulse(toB2Vec(impulse), toB2Vec(position), true);
}

void b2BodyWrapper::setLinearVelocity(const Vector2 &velocity)
{
	m_body->SetLinearVelocity(toB2Vec(velocity));
}

b2BodyWrapper *b2BodyWrapper::Factory(const b2BodyDefWrapper &def)
{
	b2BodyDef bodyDef = def.getBodyDef();
	return new b2BodyWrapper(b2d->getWorld()->CreateBody(&bodyDef));
}