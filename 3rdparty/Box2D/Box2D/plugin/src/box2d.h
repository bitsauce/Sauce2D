#ifndef B2_BOX2D_H
#define B2_BOX2D_H

#include <x2d/console.h>
#include <x2d/util.h>
#include <x2d/graphics.h>
#include <x2d/graphics/batch.h>

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Common/b2Draw.h>

class Box2D
{
public:
	Box2D();
	~Box2D();

	void step(float timeStep);
	void draw(Batch *batch);
	void setDrawFlags(int flags);
	void setScale(float scale);
	float getScale() const;
	void setGravity(const Vector2 &gravity);
	Vector2 getGravity() const;
	b2World *getWorld() const;

	Batch *getDrawBatch() const;

	void destroyBody(b2Body **body);
	void destroyFixture(b2Body *body, b2Fixture **fixture);

private:
	float m_scale;
	b2World *m_world;
	b2Draw *m_debugDraw;
	b2ContactListener *m_contactListener;
	set<b2Body**> m_bodiesToDestroy;
	set<pair<b2Body*, b2Fixture**>> m_fixturesToDestroy;
	Batch *m_drawBatch;
};

extern Box2D *b2d;

extern b2Vec2 toB2Vec(const Vector2 &vec);
extern Vector2 toXDVec(const b2Vec2 &vec);

#endif // B2_BOX2D_H