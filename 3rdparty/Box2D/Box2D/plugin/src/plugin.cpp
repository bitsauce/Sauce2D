#include "plugin.h"
#include <Box2D/Box2D.h>
#include <Box2D/Common/b2Settings.h>

#include <x2d/config.h>
#include <x2d/engine.h>
#include <x2d/scripts.h>
#include <x2d/math.h>
#include <x2d/math/array.h>
#include <x2d/scripts.h>


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

enum BodyType
{
	StaticBody = b2_staticBody,
	KinematicBody = b2_kinematicBody,
	DynamicBody = b2_dynamicBody,
	BulletBody
};

class BodyDef
{
public:
	BodyDef() :
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

	b2BodyDef getBodyDef() const
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

	BodyType type;
	Vector2 position;
	float angle;
	Vector2 linearVelocity;
	float angularVelocity;
	float linearDamping;
	float angularDamping;
	bool allowSleep;
	bool awake;
	bool fixedRotation;
	bool active;
	float gravityScale;

	static void Construct(BodyDef *self) { new (self) BodyDef; }
};

int fixCount = 0;
class Fixture
{
	friend class Body;
public:
	Fixture(b2Fixture *fixture) :
		m_fixture(fixture)
	{
	}

	RefCounter refCounter;
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

	void setDensity(const float density)
	{
		m_fixture->SetDensity(density);
	}

	void setMaskBits(const uint maskBits)
	{
		b2Filter data = m_fixture->GetFilterData();
		data.maskBits = maskBits;
		m_fixture->SetFilterData(data);
	}

	void setCategoryBits(const uint categoryBits)
	{
		b2Filter data = m_fixture->GetFilterData();
		data.categoryBits = categoryBits;
		m_fixture->SetFilterData(data);
	}

private:
	b2Fixture *m_fixture;
};

int bodyCount = 0;
class Body
{
public:
	Body(b2Body *body) :
		m_body(body)
	{
	}

	~Body()
	{
		for(vector<Fixture*>::iterator itr = m_fixtures.begin(); itr != m_fixtures.end(); ++itr) {
			(*itr)->release();
		}
		b2d->getWorld()->DestroyBody(m_body);
	}

	RefCounter refCounter;
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

	void setTransform(const Vector2 &position, float angle)
	{
		m_body->SetTransform(toB2Vec(position), angle);
	}

	Vector2 getPosition() const
	{
		return toXDVec(m_body->GetPosition());
	}

	float getAngle() const
	{
		return m_body->GetAngle();
	}

	Fixture *createFixture(const Rect &rect, float density)
	{
		b2PolygonShape shape;
		b2Vec2 halfSize = toB2Vec(rect.getSize()/2.0f);
		shape.SetAsBox(halfSize.x, halfSize.y, toB2Vec(rect.getCenter()), 0.0f);
		return new Fixture(m_body->CreateFixture(&shape, density));
	}
	
	Fixture *createFixture(const Vector2 &center, const float radius, float density)
	{
		b2CircleShape shape;
		shape.m_p = toB2Vec(center);
		shape.m_radius = radius/b2d->getScale();
		return new Fixture(m_body->CreateFixture(&shape, density));
	}
	
	Fixture *createFixture(Array *arr, float density)
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
		return new Fixture(m_body->CreateFixture(&shape, density));
	}

	void removeFixture(Fixture *fixture)
	{
		m_body->DestroyFixture(fixture->m_fixture);
		vector<Fixture*>::iterator itr;
		if((itr = find(m_fixtures.begin(), m_fixtures.end(), fixture)) != m_fixtures.end()) {
			(*itr)->release();
		}
	}

	void applyImpulse(const Vector2 &impulse, const Vector2 &position)
	{
		m_body->ApplyLinearImpulse(toB2Vec(impulse), toB2Vec(position), true);
	}

	void setLinearVelocity(const Vector2 &velocity)
	{
		m_body->SetLinearVelocity(toB2Vec(velocity));
	}

	static Body *Factory(const BodyDef &def)
	{
		b2BodyDef bodyDef = def.getBodyDef();
		return new Body(b2d->getWorld()->CreateBody(&bodyDef));
	}

private:
	b2Body *m_body;
	vector<Fixture*> m_fixtures;
};

int CreatePlugin(xdScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->registerEnum("BodyType"); AS_ASSERT
	r = scriptEngine->registerEnumValue("BodyType", "b2_staticBody", StaticBody); AS_ASSERT
	r = scriptEngine->registerEnumValue("BodyType", "b2_kinematicBody", KinematicBody); AS_ASSERT
	r = scriptEngine->registerEnumValue("BodyType", "b2_dynamicBody", DynamicBody); AS_ASSERT
	r = scriptEngine->registerEnumValue("BodyType", "b2_bulletBody", BulletBody);  AS_ASSERT

	r = scriptEngine->registerSingletonType("ScriptBox2D");
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "void step(float)", asMETHOD(Box2D, step)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "void draw()", asMETHOD(Box2D, draw)); AS_ASSERT AS_ASSERT
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "void setDrawFlags(int)", asMETHOD(Box2D, setDrawFlags)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "void set_scale(float)", asMETHOD(Box2D, setScale)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "float get_scale() const", asMETHOD(Box2D, getScale)); AS_ASSERT

	r = scriptEngine->registerValueType("b2BodyDef", sizeof(BodyDef)); AS_ASSERT
	r = scriptEngine->registerObjectConstructor("b2BodyDef", "void f()", asFUNCTION(BodyDef::Construct)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "BodyType type", offsetof(BodyDef, type)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "Vector2 position", offsetof(BodyDef, position)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "float angle", offsetof(BodyDef, angle)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "Vector2 linearVelocity", offsetof(BodyDef, linearVelocity)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "float angularVelocity", offsetof(BodyDef, angularVelocity)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "float linearDamping", offsetof(BodyDef, linearDamping)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "float angularDamping", offsetof(BodyDef, angularDamping)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "bool allowSleep", offsetof(BodyDef, allowSleep)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "bool awake", offsetof(BodyDef, awake)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "bool fixedRotation", offsetof(BodyDef, fixedRotation)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "bool active", offsetof(BodyDef, active)); AS_ASSERT
	r = scriptEngine->registerObjectProperty("b2BodyDef", "bool gravityScale", offsetof(BodyDef, gravityScale)); AS_ASSERT
	
	r = scriptEngine->registerRefType("b2Fixture", asMETHOD(Fixture, addRef), asMETHOD(Fixture, release)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Fixture", "void setDensity(const float)", asMETHOD(Fixture, setDensity)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Fixture", "void setMaskBits(const uint)", asMETHOD(Fixture, setMaskBits)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Fixture", "void setCategoryBits(const uint)", asMETHOD(Fixture, setCategoryBits)); AS_ASSERT

	r = scriptEngine->registerRefType("b2Body", asMETHOD(Body, addRef), asMETHOD(Body, release)); AS_ASSERT
	r = scriptEngine->registerObjectFactory("b2Body", "b2Body @f(const b2BodyDef &in)", asFUNCTION(Body::Factory)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "b2Fixture @createFixture(const Rect &in, float)", asMETHODPR(Body, createFixture, (const Rect&, float), Fixture*)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "b2Fixture @createFixture(const Vector2 &in, const float, float)", asMETHODPR(Body, createFixture, (const Vector2&, const float, float), Fixture*)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "b2Fixture @createFixture(array<Vector2> &in, float density)", asMETHODPR(Body, createFixture, (Array*, float), Fixture*)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void removeFixture(b2Fixture @)", asMETHOD(Body, removeFixture)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void setTransform(const Vector2 &in, float)", asMETHOD(Body, setTransform)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "Vector2 getPosition() const", asMETHOD(Body, getPosition)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "float getAngle() const", asMETHOD(Body, getAngle)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void applyImpulse(const Vector2 &in, const Vector2 &in)", asMETHOD(Body, applyImpulse)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("b2Body", "void setLinearVelocity(const Vector2 &in)", asMETHOD(Body, setLinearVelocity)); AS_ASSERT

	b2d = new Box2D;
	r = scriptEngine->registerGlobalProperty("ScriptBox2D Box2D", b2d);
	
	r = scriptEngine->registerFuncdef("void ContactCallback(b2Fixture@)"); AS_ASSERT
	
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