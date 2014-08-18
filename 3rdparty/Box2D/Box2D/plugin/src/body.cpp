#include "body.h"
#include "fixture.h"
#include "box2d.h"
#include "plugin.h"
#include <x2d/scriptengine.h>
#include <x2d/scripts/array.h>
#include <Box2D/Box2D.h>

int b2BodyWrapper::TypeId = 0;

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
	m_object.value = 0;
	m_object.typeId = 0;
	m_body->SetUserData(this);
}

b2BodyWrapper::~b2BodyWrapper()
{
	freeObject();
	destroy();
	for(vector<b2FixtureWrapper*>::iterator itr = m_fixtures.begin(); itr != m_fixtures.end(); ++itr) {
		(*itr)->m_body = 0;
	}
}

void b2BodyWrapper::destroy()
{
	if(m_body)
	{
		b2d->destroyBody(&m_body);
		for(vector<b2FixtureWrapper*>::iterator itr = m_fixtures.begin(); itr != m_fixtures.end(); ++itr) {
			(*itr)->destroy();
		}
	}
}

b2FixtureWrapper *b2BodyWrapper::createFixture(const Rect &rect, float density)
{
	if(!m_body || b2d->getWorld()->IsLocked())
		return 0;

	b2PolygonShape shape;
	b2Vec2 halfSize = toB2Vec(rect.getSize()/2.0f);
	shape.SetAsBox(halfSize.x, halfSize.y, toB2Vec(rect.getCenter()), 0.0f);
	
	addRef();
	b2FixtureWrapper *fixture = new b2FixtureWrapper(this, m_body->CreateFixture(&shape, density));
	fixture->addRef();
	
	asIScriptEngine *scriptEngine = xdengine->getScriptEngine()->getASEngine();
	scriptEngine->NotifyGarbageCollectorOfNewObject(fixture, scriptEngine->GetObjectTypeById(b2FixtureWrapper::TypeId));

	m_fixtures.push_back(fixture);
	return fixture;
}
	
b2FixtureWrapper *b2BodyWrapper::createFixture(const Vector2 &center, const float radius, float density)
{
	if(!m_body || b2d->getWorld()->IsLocked())
		return 0;

	b2CircleShape shape;
	shape.m_p = toB2Vec(center);
	shape.m_radius = radius/b2d->getScale();
	
	addRef();
	b2FixtureWrapper *fixture = new b2FixtureWrapper(this, m_body->CreateFixture(&shape, density));
	fixture->addRef();
	
	asIScriptEngine *scriptEngine = xdengine->getScriptEngine()->getASEngine();
	scriptEngine->NotifyGarbageCollectorOfNewObject(fixture, scriptEngine->GetObjectTypeById(b2FixtureWrapper::TypeId));

	m_fixtures.push_back(fixture);
	return fixture;
}
	
b2FixtureWrapper *b2BodyWrapper::createFixture(Array *arr, float density)
{
	if(!m_body || b2d->getWorld()->IsLocked())
		return 0;

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
	addRef();
	b2FixtureWrapper *fixture = new b2FixtureWrapper(this, m_body->CreateFixture(&shape, density));
	fixture->addRef();
	
	asIScriptEngine *scriptEngine = xdengine->getScriptEngine()->getASEngine();
	scriptEngine->NotifyGarbageCollectorOfNewObject(fixture, scriptEngine->GetObjectTypeById(b2FixtureWrapper::TypeId));

	m_fixtures.push_back(fixture);
	return fixture;
}

void b2BodyWrapper::removeFixture(b2FixtureWrapper *fixture)
{
	if(!m_body || b2d->getWorld()->IsLocked())
		return;
	
	vector<b2FixtureWrapper*>::iterator itr;
	if((itr = find(m_fixtures.begin(), m_fixtures.end(), fixture)) != m_fixtures.end())
	{
		fixture->destroy();
		//fixture->release();
		//m_fixtures.erase(itr);
	}
	fixture->release();
}

void b2BodyWrapper::setTransform(const Vector2 &position, float angle)
{
	if(!m_body || b2d->getWorld()->IsLocked())
		return;

	m_body->SetTransform(toB2Vec(position), angle);
}

void b2BodyWrapper::setPosition(const Vector2 &position)
{
	if(!m_body || b2d->getWorld()->IsLocked())
		return;

	m_body->SetTransform(toB2Vec(position), m_body->GetAngle());
}

void b2BodyWrapper::setAngle(float angle)
{
	if(!m_body || b2d->getWorld()->IsLocked())
		return;

	m_body->SetTransform(m_body->GetPosition(), angle);
}

void b2BodyWrapper::setBeginContactCallback(asIScriptFunction *func)
{
	m_beginContactFunc = func;
}

void b2BodyWrapper::setEndContactCallback(asIScriptFunction *func)
{
	m_endContactFunc = func;
}

void b2BodyWrapper::setPreSolveCallback(asIScriptFunction *func)
{
	m_preSolveFunc = func;
}

void b2BodyWrapper::setPostSolveCallback(asIScriptFunction *func)
{
	m_postSolveFunc = func;
}

void b2BodyWrapper::freeObject()
{
	// If it is a handle or a ref counted object, call release
	asIScriptEngine *scriptEngine = xdengine->getScriptEngine()->getASEngine();
	if(m_object.typeId & asTYPEID_MASK_OBJECT)
	{
		// Let the engine release the object
		asIObjectType *ot = scriptEngine->GetObjectTypeById(m_object.typeId);
		scriptEngine->ReleaseScriptObject(m_object.value, ot);

		// Release the object type info
		if(ot) ot->Release();

		m_object.value = 0;
		m_object.typeId = 0;
	}
}

void b2BodyWrapper::setObject(void *object, int typeId)
{
	// Hold on to the object type reference so it isn't destroyed too early
	asIScriptEngine *scriptEngine = xdengine->getScriptEngine()->getASEngine();
	if(*(void**)object && (typeId & asTYPEID_MASK_OBJECT))
	{
		asIObjectType *ot = scriptEngine->GetObjectTypeById(typeId);
		if(ot) ot->AddRef();
	}

	freeObject();

	m_object.typeId = typeId;
	if(typeId & asTYPEID_OBJHANDLE)
	{
		// We're receiving a reference to the handle, so we need to dereference it
		m_object.value = *(void**)object;
		scriptEngine->AddRefScriptObject(m_object.value, scriptEngine->GetObjectTypeById(typeId));
	}else{
		LOG("b2Body.setObject: Supports only object handles!");
	}
}

bool b2BodyWrapper::getObject(void *object, int typeId)
{
	asIScriptEngine *scriptEngine = xdengine->getScriptEngine()->getASEngine();
	if(typeId & asTYPEID_OBJHANDLE)
	{
		// Is the handle type compatible with the stored value?

		// A handle can be retrieved if the stored type is a handle of same or compatible type
		// or if the stored type is an object that implements the interface that the handle refer to.
		if((m_object.typeId & asTYPEID_MASK_OBJECT) && scriptEngine->IsHandleCompatibleWithObject(m_object.value, m_object.typeId, typeId))
		{
			scriptEngine->AddRefScriptObject(m_object.value, scriptEngine->GetObjectTypeById(m_object.typeId));
			*(void**)object = m_object.value;

			return true;
		}
	}
	return false;
}

void b2BodyWrapper::enumReferences(asIScriptEngine *engine)
{
	// If we're holding a reference, we'll notify the garbage collector of it
	if(m_object.value && (m_object.typeId & asTYPEID_MASK_OBJECT))
	{
		engine->GCEnumCallback(m_object.value);

		// The object type itself is also garbage collected
		asIObjectType *ot = engine->GetObjectTypeById(m_object.typeId);
		if(ot) engine->GCEnumCallback(ot);
	}
	if(m_beginContactFunc) {
		engine->GCEnumCallback(m_beginContactFunc);
	}
	if(m_endContactFunc) {
		engine->GCEnumCallback(m_endContactFunc);
	}
	if(m_preSolveFunc) {
		engine->GCEnumCallback(m_preSolveFunc);
	}
	if(m_postSolveFunc) {
		engine->GCEnumCallback(m_postSolveFunc);
	}
	for(vector<b2FixtureWrapper*>::iterator itr = m_fixtures.begin(); itr != m_fixtures.end(); ++itr) {
		engine->GCEnumCallback(*itr);
	}
}

void b2BodyWrapper::releaseReferences(asIScriptEngine *engine)
{
	freeObject();
	if(m_beginContactFunc) {
		m_beginContactFunc->Release();
	}
	if(m_endContactFunc) {
		m_endContactFunc->Release();
	}
	if(m_preSolveFunc) {
		m_preSolveFunc->Release();
	}
	if(m_postSolveFunc) {
		m_postSolveFunc->Release();
	}
	for(vector<b2FixtureWrapper*>::iterator itr = m_fixtures.begin(); itr != m_fixtures.end(); ++itr) {
		(*itr)->release();
	}
}

Vector2 b2BodyWrapper::getPosition() const
{
	if(!m_body)
		return Vector2(0.0f);

	return toXDVec(m_body->GetPosition());
}

float b2BodyWrapper::getAngle() const
{
	if(!m_body)
		return 0.0f;
	return m_body->GetAngle();
}

Vector2 b2BodyWrapper::getCenter() const
{
	if(!m_body)
		return Vector2(0.0f);

	return toXDVec(m_body->GetWorldCenter());
}

Vector2 b2BodyWrapper::getLinearVelocity() const
{
	if(!m_body)
		return Vector2(0.0f);

	return toXDVec(m_body->GetLinearVelocity());
}

float b2BodyWrapper::getMass() const
{
	if(!m_body)
		return 0.0f;

	return m_body->GetMass();
}

void b2BodyWrapper::applyImpulse(const Vector2 &impulse, const Vector2 &position)
{
	if(!m_body)
		return;

	m_body->ApplyLinearImpulse(toB2Vec(impulse), toB2Vec(position), true);
}

void b2BodyWrapper::setLinearVelocity(const Vector2 &velocity)
{
	if(!m_body)
		return;

	m_body->SetLinearVelocity(toB2Vec(velocity));
}

void b2BodyWrapper::serialize(StringStream &ss) const
{
	(stringstream&)ss << (m_body != 0) << endl;
	if(m_body)
	{
		(stringstream&)ss << (int32)m_body->GetType() << endl;
		(stringstream&)ss << m_body->GetPosition().x << endl;
		(stringstream&)ss << m_body->GetPosition().y << endl;
		(stringstream&)ss << m_body->GetAngle() << endl;
		(stringstream&)ss << m_body->GetLinearVelocity().x << endl;
		(stringstream&)ss << m_body->GetLinearVelocity().y << endl;
		(stringstream&)ss << m_body->GetAngularVelocity() << endl;
		(stringstream&)ss << m_body->GetLinearDamping() << endl;
		(stringstream&)ss << m_body->GetAngularDamping() << endl;
		(stringstream&)ss << m_body->IsSleepingAllowed() << endl;
		(stringstream&)ss << m_body->IsAwake() << endl;
		(stringstream&)ss << m_body->IsFixedRotation() << endl;
		(stringstream&)ss << m_body->IsBullet() << endl;
		(stringstream&)ss << m_body->IsActive() << endl;
		(stringstream&)ss << m_body->GetGravityScale() << endl;

		(stringstream&)ss << m_fixtures.size() << endl;
		for(int i = 0; i < m_fixtures.size(); i++)
		{
			xdengine->getScriptEngine()->serialize((void*)&m_fixtures[i], b2FixtureWrapper::TypeId | asTYPEID_OBJHANDLE, ss);
		}
	}
}

b2BodyWrapper *b2BodyWrapper::Factory(StringStream &ss)
{
	b2BodyWrapper *body = 0;
	b2BodyDef def;
	int32 i;
	float32 x, y, f;
	bool b;

	(stringstream&)ss >> b; ((stringstream&)ss).ignore();
	if(b)
	{
		(stringstream&)ss >> i; ((stringstream&)ss).ignore();
		def.type = b2BodyType(i);

		(stringstream&)ss >> x; ((stringstream&)ss).ignore();
		(stringstream&)ss >> y; ((stringstream&)ss).ignore();
		def.position.Set(x, y);

		(stringstream&)ss >> f; ((stringstream&)ss).ignore();
		def.angle = f;

		(stringstream&)ss >> x; ((stringstream&)ss).ignore();
		(stringstream&)ss >> y; ((stringstream&)ss).ignore();
		def.linearVelocity.Set(x, y);

		(stringstream&)ss >> f; ((stringstream&)ss).ignore();
		def.angularVelocity = f;
	
		(stringstream&)ss >> f; ((stringstream&)ss).ignore();
		def.linearDamping = f;
	
		(stringstream&)ss >> f; ((stringstream&)ss).ignore();
		def.angularDamping = f;
	
		(stringstream&)ss >> b; ((stringstream&)ss).ignore();
		def.allowSleep = b;
	
		(stringstream&)ss >> b; ((stringstream&)ss).ignore();
		def.awake = b;

		(stringstream&)ss >> b; ((stringstream&)ss).ignore();
		def.fixedRotation = b;
	
		(stringstream&)ss >> b; ((stringstream&)ss).ignore();
		def.bullet = b;
	
		(stringstream&)ss >> b; ((stringstream&)ss).ignore();
		def.active = b;
	
		(stringstream&)ss >> f; ((stringstream&)ss).ignore();
		def.gravityScale = f;

		body = new b2BodyWrapper(b2d->getWorld()->CreateBody(&def));
		asIScriptEngine *scriptEngine = xdengine->getScriptEngine()->getASEngine();
		scriptEngine->NotifyGarbageCollectorOfNewObject(body, scriptEngine->GetObjectTypeById(b2BodyWrapper::TypeId));
		xdengine->getScriptEngine()->notifySerializerOfNewObject(body);

		(stringstream&)ss >> i; ((stringstream&)ss).ignore();
		body->m_fixtures.resize(i);
		for(int i = 0; i < body->m_fixtures.size(); i++)
		{
			xdengine->getScriptEngine()->deserialize(&body->m_fixtures[i], b2FixtureWrapper::TypeId | asTYPEID_OBJHANDLE, ss);
		}
	}
	return body;
}

b2BodyWrapper *b2BodyWrapper::Factory(const b2BodyDefWrapper &def)
{
	b2BodyDef bodyDef = def.getBodyDef();
	b2BodyWrapper *body = new b2BodyWrapper(b2d->getWorld()->CreateBody(&bodyDef));
	asIScriptEngine *scriptEngine = xdengine->getScriptEngine()->getASEngine();
	scriptEngine->NotifyGarbageCollectorOfNewObject(body, scriptEngine->GetObjectTypeById(b2BodyWrapper::TypeId));
	return body;
}