#ifndef B2_BOX2D_H
#define B2_BOX2D_H

#include <x2d/console.h>
#include <x2d/util.h>

#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Common/b2Draw.h>

class Box2D
{
public:
	Box2D();
	~Box2D();

	void step(float timeStep);
	void draw();
	void setDrawFlags(int flags);
	void setScale(float scale);
	float getScale() const;
	b2World *getWorld() const;

private:
	float m_scale;
	b2World *m_world;
	b2Draw *m_debugDraw;
};

extern Box2D *b2d;

extern b2Vec2 toB2Vec(const Vector2 &vec);
extern Vector2 toXDVec(const b2Vec2 &vec);

#endif // B2_BOX2D_H