#include "plugin.h"
#include <Box2D/Box2D.h>
#include <Box2D/Common/b2Settings.h>

#include <x2d/config.h>
#include <x2d/engine.h>
#include <x2d/scripts.h>
#include <x2d/math.h>

#include <gl/GL.h>
#include <gl/GLU.h>


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
class ContactListener : public b2ContactListener
{
	void BeginContact(b2Contact* contact)
	{
		currentContact = contact;

		// Check for fixture callback
		int fixtureId = *(int*)contact->GetFixtureA()->GetUserData();
		if(fbccbs.find(fixtureId) != fbccbs.end()) {
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

	void EndContact(b2Contact* contact)
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

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		/*currentImpulse = impulse;

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

		currentImpulse = 0;*/
	}
};
ContactListener *contactListener = 0;

// Debug draw class
// TODO: All the gl commands should be replaced with x2d's rendering commands
class DebugDraw : public b2Draw
{
public:
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < vertexCount; ++i)
		{
			glVertex2f(vertices[i].x*worldScale, vertices[i].y*worldScale);
		}
		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
		glBegin(GL_TRIANGLE_FAN);
		for(int i = 0; i < vertexCount; ++i)
		{
			glVertex2f(vertices[i].x*worldScale, vertices[i].y*worldScale);
		}
		glEnd();

		glColor4f(color.r, color.g, color.b, 1.0f);
		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < vertexCount; ++i)
		{
			glVertex2f(vertices[i].x*worldScale, vertices[i].y*worldScale);
		}
		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
	{
		const float32 k_segments = 16.0f;
		const float32 k_increment = 2.0f * b2_pi / k_segments;
		float32 theta = 0.0f;
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < k_segments; ++i)
		{
			b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
			glVertex2f(v.x*worldScale, v.y*worldScale);
			theta += k_increment;
		}
		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
	{
		const float32 k_segments = 16.0f;
		const float32 k_increment = 2.0f * b2_pi / k_segments;
		float32 theta = 0.0f;
		glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
		glBegin(GL_TRIANGLE_FAN);
		for(int i = 0; i < k_segments; ++i)
		{
			b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
			glVertex2f(v.x*worldScale, v.y*worldScale);
			theta += k_increment;
		}
		glEnd();

		theta = 0.0f;
		glColor4f(color.r, color.g, color.b, 1.0f);
		glBegin(GL_LINE_LOOP);
		for (int32 i = 0; i < k_segments; ++i)
		{
			b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
			glVertex2f(v.x*worldScale, v.y*worldScale);
			theta += k_increment;
		}
		glEnd();

		b2Vec2 p = center + radius * axis;
		glBegin(GL_LINES);
		glVertex2f(center.x*worldScale, center.y*worldScale);
		glVertex2f(p.x*worldScale, p.y*worldScale);
		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_LINES);
		glVertex2f(p1.x*worldScale, p1.y*worldScale);
		glVertex2f(p2.x*worldScale, p2.y*worldScale);
		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void DrawTransform(const b2Transform& xf)
	{
		b2Vec2 p1 = xf.p, p2;
		const float32 k_axisScale = 0.4f;
		glBegin(GL_LINES);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(p1.x*worldScale, p1.y*worldScale);
		p2 = p1 + k_axisScale * xf.q.GetXAxis();
		glVertex2f(p2.x*worldScale, p2.y*worldScale);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(p1.x*worldScale, p1.y*worldScale);
		p2 = p1 + k_axisScale * xf.q.GetYAxis();
		glVertex2f(p2.x*worldScale, p2.y*worldScale);

		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
	{
		glPointSize(size);
		glBegin(GL_POINTS);
		glColor3f(color.r, color.g, color.b);
		glVertex2f(p.x*worldScale, p.y*worldScale);
		glEnd();
		glPointSize(1.0f);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void DrawString(int x, int y, const char* string, ...)
	{
	}

	void DrawAABB(b2AABB* aabb, const b2Color& color)
	{
		glColor3f(color.r, color.g, color.b);
		glBegin(GL_LINE_LOOP);
		glVertex2f(aabb->lowerBound.x*worldScale, aabb->lowerBound.y*worldScale);
		glVertex2f(aabb->upperBound.x*worldScale, aabb->lowerBound.y*worldScale);
		glVertex2f(aabb->upperBound.x*worldScale, aabb->upperBound.y*worldScale);
		glVertex2f(aabb->lowerBound.x*worldScale, aabb->upperBound.y*worldScale);
		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
};

// Debug draw global
DebugDraw *debugDraw = 0;

// Mapping
map<int, b2Body*> bodies;
int currBodyId = 0;

map<int, b2Fixture*> fixtures;
int currFixtureId = 0;

map<int, b2RevoluteJoint*> revoluteJoints;
int currRevoluteJointId = 0;

vector<int*> userData;
int *getUserDataIdPtr(int id)
{
	int *idPtr = new int;
	*idPtr = id;
	userData.push_back(idPtr);
	return idPtr;
}

//--------------------------------------------------------------
// Box2D Main
//--------------------------------------------------------------


//--------------------------------------------------------------
// Box2D Body
//--------------------------------------------------------------

int createBody()
{
	// Create body
	b2BodyDef bd;
	int id = currBodyId++;
	bodies[id] = world->CreateBody(&bd);
	bodies[id]->SetUserData(getUserDataIdPtr(id));
	return id;
}

void removeBody(int bodyId)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	world->DestroyBody(bodies[bodyId]);
	bodies.erase(bodyId);
}

void setBodyTransform(int bodyId, float x, float y, float ang)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	bodies[bodyId]->SetTransform(b2Vec2(x/worldScale, y/worldScale), ang);
}

void getBodyTransform(int bodyId, float &x, float &y, float &ang)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	b2Transform trans = bodies[bodyId]->GetTransform();
	x = trans.p.x*worldScale;
	y = trans.p.y*worldScale;
	ang = trans.q.GetAngle();
}

void setBodyPosition(int bodyId, float x, float y)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	bodies[bodyId]->SetTransform(b2Vec2(x/worldScale, y/worldScale), bodies[bodyId]->GetAngle());
}

void getBodyPosition(int bodyId, float &x, float &y)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	b2Vec2 pos = bodies[bodyId]->GetPosition();
	x = pos.x*worldScale;
	y = pos.y*worldScale;
}

void setBodyAngle(int bodyId, float ang)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	bodies[bodyId]->SetTransform(bodies[bodyId]->GetPosition(), ang);
}

void getBodyAngle(int bodyId, float &ang)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	b2Transform trans = bodies[bodyId]->GetTransform();
	ang = trans.q.GetAngle();
}

void getBodyCenter(int bodyId, float &x, float &y)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	b2Vec2 pos = bodies[bodyId]->GetWorldCenter();
	x = pos.x*worldScale;
	y = pos.y*worldScale;
}

void setLinearVelocity(int bodyId, float vx, float vy)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	bodies[bodyId]->SetLinearVelocity(b2Vec2(vx/worldScale, vy/worldScale));
}

void getLinearVelocity(int bodyId, float &vx, float &vy)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	b2Vec2 vel = bodies[bodyId]->GetLinearVelocity();
	vx = vel.x*worldScale;
	vy = vel.y*worldScale;
}

void setAngularVelocity(int bodyId, float v)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	bodies[bodyId]->SetAngularVelocity(v);
}

void getAngularVelocity(int bodyId, float &v)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	v = bodies[bodyId]->GetAngularVelocity();
}

enum BodyType
{
	StaticBody = 0,
	KinematicBody,
	DynamicBody,
	BulletBody
};

void setBodyType(int bodyId, BodyType type)
{
	if(bodies.find(bodyId) == bodies.end())
		return;

	if(type == BulletBody)
	{
		bodies[bodyId]->SetType(b2BodyType(DynamicBody));
		bodies[bodyId]->SetBullet(true);
	}else{
		bodies[bodyId]->SetType(b2BodyType(type));
	}
}

void setAllowSleep(int bodyId, bool val)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	bodies[bodyId]->SetSleepingAllowed(val);
}

void setFixedRotation(int bodyId, bool val)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	bodies[bodyId]->SetFixedRotation(val);
}

void setBodyGravityScale(int bodyId, float scale)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	bodies[bodyId]->SetGravityScale(scale);
}

void bodyApplyImpulse(int bodyId, float vx, float vy, float px, float py)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	bodies[bodyId]->ApplyLinearImpulse(b2Vec2(vx/worldScale, vy/worldScale), b2Vec2(px/worldScale, py/worldScale), true);
}

void getBodyType(int bodyId, int &type)
{
	if(bodies.find(bodyId) == bodies.end()) return;
	type = bodies[bodyId]->GetType();
}

/*Array *getBodyFixtures(int bodyId)
{
// Create the array object
asIObjectType *arrayType = scriptEngine->GetObjectTypeById(scriptEngine->GetTypeIdByDecl("array<int>"));
CScriptArray *arr = new CScriptArray(m_boundTexture->width*m_boundTexture->height, arrayType);

if(bodies.find(bodyId) == bodies.end()) return;
}*/


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

#include <x2d/console.h>

class Box2D
{
public:
	Box2D()
	{
		m_world = new b2World(b2Vec2(0.0f, 10.0f));
		m_world->SetAllowSleeping(true);

		LOG("** Init Box2D %i.%i.%i **", b2_version.major, b2_version.minor, b2_version.revision);
		
		uint drawFlags = b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_aabbBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit;
		
#ifdef OLD
		m_debugDraw = new DebugDraw;
		m_debugDraw->SetFlags(drawFlags);
		m_world->SetDebugDraw(m_debugDraw);

		contactListener = new ContactListener;
		m_world->SetContactListener(contactListener);
#endif
	}

	~Box2D()
	{
		delete m_world;
		delete m_debugDraw;
		//delete m_contactListener;
	}

	void step(float timeStep)
	{
		m_world->Step(timeStep, 8, 3);
	}

	void draw()
	{
		m_world->DrawDebugData();
	}

	void setDrawFlags(int flags)
	{
		m_debugDraw->SetFlags(flags);
	}

	void setScale(float scale)
	{
		m_scale = scale;
	}

	float getScale() const
	{
		return m_scale;
	}

	b2World *getWorld() const
	{
		return m_world;
	}

private:
	float m_scale;
	b2World *m_world;
	b2Draw *m_debugDraw;
};

Box2D *b2d = 0;

class Body
{
public:
	Body(b2Body *body) :
		m_body(body)
	{
	}

	~Body()
	{
		b2d->getWorld()->DestroyBody(m_body);
	}

	RefCounter refCounter;

	void addRef()
	{
		refCounter.add();
	}

	void release()
	{
		if(refCounter.release() == 0)
			delete this;
	}

	void setTransform(const Vector2 &pos, float angle)
	{
		m_body->SetTransform(b2Vec2(pos.x, pos.y), angle);
	}

	static Body *Factory(const b2BodyDef *def)
	{
		new Body(b2d->getWorld()->CreateBody(def));
	}

private:
	b2Body *m_body;
};

#include <x2d/scripts.h>

int CreatePlugin(xdScriptEngine *scriptEngine)
{
	int r = 0;
	r = scriptEngine->registerFuncdef("void ContactCallback(int)"); AS_ASSERT

	r = scriptEngine->registerEnum("BodyType"); AS_ASSERT
	r = scriptEngine->registerEnumValue("BodyType", "B2_STATIC_BODY", b2_staticBody); AS_ASSERT
	r = scriptEngine->registerEnumValue("BodyType", "B2_KINEMATIC_BODY", b2_kinematicBody); AS_ASSERT
	r = scriptEngine->registerEnumValue("BodyType", "B2_DYNAMIC_BODY", b2_dynamicBody); AS_ASSERT
	r = scriptEngine->registerEnumValue("BodyType", "B2_BULLET_BODY", 3);  

	r = scriptEngine->registerSingletonType("ScriptBox2D");
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "void step(float)", asMETHOD(Box2D, step)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "void draw()", asMETHOD(Box2D, draw)); AS_ASSERT AS_ASSERT
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "void setDrawFlags(int)", asMETHOD(Box2D, setDrawFlags)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "void set_scale(float)", asMETHOD(Box2D, setScale)); AS_ASSERT
	r = scriptEngine->registerObjectMethod("ScriptBox2D", "float get_scale() const", asMETHOD(Box2D, getScale)); AS_ASSERT
	
	r = scriptEngine->registerRefType("b2Body", asMETHOD(Body, addRef), asMETHOD(Body, release));
	r = scriptEngine->registerObjectFactory("b2Body", "b2Body @f(const b2BodyDef &in)", asFUNCTION(Body::Factory));
	r = scriptEngine->registerObjectMethod("b2Body", "void setTransform(const Vector2 &in, float)", asMETHOD(Body, setTransform));

	b2d = new Box2D;
	r = scriptEngine->registerGlobalProperty("ScriptBox2D Box2D", b2d);
	
#ifdef OLD
	// Register function defs
	registerFunctionDef("void ContactCallback(int)");

	// Register enums
	registerEnum("BodyType");
	registerEnumValue("BodyType", "StaticBody", StaticBody);
	registerEnumValue("BodyType", "KinematicBody", KinematicBody);
	registerEnumValue("BodyType", "DynamicBody", DynamicBody);
	registerEnumValue("BodyType", "BulletBody", BulletBody);

	// Box2D main
	registerGlobalFunction("void b2dInit()", asFUNCTION(init));
	registerGlobalFunction("void b2dStep(float timeStep)", asFUNCTION(step));
	registerGlobalFunction("void b2dDebugDraw()", asFUNCTION(draw));
	registerGlobalFunction("void b2dSetDebugDrawFlags(int flags)", asFUNCTION(setDebugDrawFlags));
	registerGlobalFunction("void b2dSetWorldScale(float worldScale)", asFUNCTION(setWorldScale));
	registerGlobalFunction("float b2dGetWorldScale()", asFUNCTION(getWorldScale));

	// Box2D contact handlers
	registerGlobalFunction("void b2dAddBodyBeginContactCallback(int, ContactCallback @callback)", asFUNCTION(addBodyBeginContactCallback));
	registerGlobalFunction("void b2dRemoveBodyBeginContactCallback(int)", asFUNCTION(removeBodyBeginContactCallback));
	registerGlobalFunction("void b2dAddFixtureBeginContactCallback(int, ContactCallback @callback)", asFUNCTION(addFixtureBeginContactCallback));
	registerGlobalFunction("void b2dRemoveFixtureBeginContactCallback(int)", asFUNCTION(removeFixtureBeginContactCallback));
	registerGlobalFunction("void b2dAddBodyEndContactCallback(int, ContactCallback @callback)", asFUNCTION(addBodyEndContactCallback));
	registerGlobalFunction("void b2dAddFixtureEndContactCallback(int, ContactCallback @callback)", asFUNCTION(addFixtureEndContactCallback));
	//registerGlobalFunction("void b2dAddBodyPostSolveCallback(int, ContactCallback @callback)", asFUNCTION(addBodyPostSolveCallback));
	//registerGlobalFunction("void b2dAddFixturePostSolveCallback(int, ContactCallback @callback)", asFUNCTION(addFixturePostSolveCallback));
	registerGlobalFunction("void b2dGetContactNormalVector(float &out, float &out)", asFUNCTION(getContactNormalVector));

	// Box2D body
	registerGlobalFunction("int  b2dCreateBody()", asFUNCTION(createBody));
	registerGlobalFunction("void b2dRemoveBody(int bodyId)", asFUNCTION(removeBody));
	registerGlobalFunction("void b2dSetBodyType(int bodyId, BodyType type)", asFUNCTION(setBodyType));
	registerGlobalFunction("void b2dSetBodyAllowSleep(int bodyId, bool sleep)", asFUNCTION(setAllowSleep));
	registerGlobalFunction("void b2dSetBodyFixedRotation(int bodyId, bool fixed)", asFUNCTION(setFixedRotation));
	registerGlobalFunction("void b2dSetBodyLinearVelocity(int bodyId, float vx, float vy)", asFUNCTION(setLinearVelocity));
	registerGlobalFunction("void b2dSetBodyAngularVelocity(int bodyId, float v)", asFUNCTION(setAngularVelocity));
	registerGlobalFunction("void b2dSetBodyTransform(int bodyId, float x, float y, float ang)", asFUNCTION(setBodyTransform));
	registerGlobalFunction("void b2dSetBodyPosition(int bodyId, float x, float y)", asFUNCTION(setBodyPosition));
	registerGlobalFunction("void b2dSetBodyAngle(int bodyId, float ang)", asFUNCTION(setBodyAngle));
	registerGlobalFunction("void b2dSetBodyGravityScale(int bodyId, float scale)", asFUNCTION(setBodyGravityScale));
	registerGlobalFunction("void b2dGetBodyType(int bodyId, int &out type)", asFUNCTION(getBodyType));
	registerGlobalFunction("void b2dGetBodyCenter(int bodyId, float &out x, float &out y)", asFUNCTION(getBodyCenter));
	registerGlobalFunction("void b2dGetBodyLinearVelocity(int bodyId, float &out x, float &out y)", asFUNCTION(getLinearVelocity));
	registerGlobalFunction("void b2dGetBodyAngularVelocity(int bodyId, float &out x)", asFUNCTION(getAngularVelocity));
	//registerGlobalFunction("array<int> @b2dGetBodyFixtures(int bodyId)", asFUNCTION(getBodyFixtures));
	registerGlobalFunction("void b2dBodyApplyImpulse(int bodyId, float vx, float vy, float px, float py)", asFUNCTION(bodyApplyImpulse));
	registerGlobalFunction("void b2dGetBodyTransform(int bodyId, float &out x, float &out y, float &out ang)", asFUNCTION(getBodyTransform));
	registerGlobalFunction("void b2dGetBodyPosition(int bodyId, float &out x, float &out y)", asFUNCTION(getBodyPosition));
	registerGlobalFunction("void b2dGetBodyAngle(int bodyId, float &out)", asFUNCTION(getBodyAngle));
	
	// b2d fixture
	registerGlobalFunction("int  b2dCreateFixtureBox(int bodyId, float w, float h, float cx, float cy, float ang, float density)", asFUNCTION(createFixtureBox));
	registerGlobalFunction("int  b2dCreateFixtureCircle(int bodyId, float r, float cx, float cy, float density)", asFUNCTION(createFixtureCircle));
	registerGlobalFunction("int  b2dCreateFixture(int bodyId, array<float> &in vertices, int vertCount, float density)", asFUNCTION(createFixture));
	registerGlobalFunction("void b2dRemoveFixture(int bodyId, int fixtureId)", asFUNCTION(removeFixture));
	registerGlobalFunction("void b2dSetFixtureDensity(int fixtureId, float density)", asFUNCTION(setFixtureDensity));
	registerGlobalFunction("void b2dSetFixtureMaskBits(int fixtureId, int bits)", asFUNCTION(setFixtureMaskBits));
	registerGlobalFunction("void b2dSetFixtureCategoryBits(int fixtureId, int bits)", asFUNCTION(setFixtureCategoryBits));
	registerGlobalFunction("int  b2dGetFixtureMaskBits(int fixtureId)", asFUNCTION(getFixtureMaskBits));
	registerGlobalFunction("int  b2dGetFixtureCategoryBits(int fixtureId)", asFUNCTION(getFixtureCategoryBits));
	registerGlobalFunction("void b2dGetFixtureAABB(int fixtureId, float &out x, float &out y, float &out w, float &out h)", asFUNCTION(getFixtureAABB));
	registerGlobalFunction("void b2dGetFixtrueType(int fixtureId, int &out type)", asFUNCTION(getFixtrueType));
	registerGlobalFunction("void b2dGetFixtrueRadius(int fixtureId, float &out r)", asFUNCTION(getFixtrueRadius));

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
}

void ReleasePlugin()
{
	delete b2d;
	//delete world;
	//delete debugDraw;
	//delete contactListener;
	//for(uint i = 0; i < userData.size(); i++)
	//	delete userData[i];
	//for(map<int, void*>::iterator itr = bbccbs.begin(); itr != bbccbs.end(); ++itr)
	//	releaseScriptFunc(itr->second);
	//for(map<int, void*>::iterator itr = fbccbs.begin(); itr != fbccbs.end(); ++itr)
	//	releaseScriptFunc(itr->second);
}