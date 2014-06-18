#ifndef B2_BODY_WRAPPER_H
#define B2_BODY_WRAPPER_H

#include <x2d/base.h>
#include <x2d/math.h>
#include <Box2D/Dynamics/b2Body.h>

class AnyObject;
class b2Body;
class b2FixtureWrapper;

enum b2BodyTypeWrapper
{
	StaticBody = b2_staticBody,
	KinematicBody = b2_kinematicBody,
	DynamicBody = b2_dynamicBody,
	BulletBody
};

class b2BodyDefWrapper
{
public:
	b2BodyDefWrapper();
	b2BodyDef getBodyDef() const;

	b2BodyTypeWrapper type;
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

	static void Construct(b2BodyDefWrapper *self) { new (self) b2BodyDefWrapper; }
};

class b2BodyWrapper
{
	friend class b2ContactWrapper;
public:
	b2BodyWrapper(b2Body *body);
	~b2BodyWrapper();

	RefCounter refCounter;
	void addRef() { refCounter.add(); }
	void release() { if(refCounter.release() == 0) delete this; }

	b2FixtureWrapper *createFixture(const Rect &rect, float density);
	b2FixtureWrapper *createFixture(const Vector2 &center, const float radius, float density);
	b2FixtureWrapper *createFixture(Array *arr, float density);

	void removeFixture(b2FixtureWrapper *fixture);
	void setTransform(const Vector2 &position, float angle);
	void setPosition(const Vector2 &position);
	void setAngle(float angle);
	void setBeginContactCallback(void *func);
	void setEndContactCallback(void *func);
	void setPreSolveCallback(void *func);
	void setPostSolveCallback(void *func);
	void setObject(void *object, int typeId);
	bool getObject(void *object, int typeId);
	Vector2 getPosition() const;
	float getAngle() const;
	void applyImpulse(const Vector2 &impulse, const Vector2 &position);
	void setLinearVelocity(const Vector2 &velocity);

	static b2BodyWrapper *Factory(const b2BodyDefWrapper &def);

private:
	b2Body *m_body;
	vector<b2FixtureWrapper*> m_fixtures;
	//AnyObject *m_object;
	void *m_beginContactFunc;
	void *m_endContactFunc;
	void *m_preSolveFunc;
	void *m_postSolveFunc;
};

#endif // B2_BODY_WRAPPER_H