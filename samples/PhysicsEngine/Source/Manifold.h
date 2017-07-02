#pragma once

#include "Shapes.h"

class Manifold
{
public:
	Manifold(Shape *a, Shape *b) :
		a(a),
		b(b),
		penetration(0.0f),
		normal(0.0f),
		contactCount(0)
	{
	}

	Shape * const a;
	Shape * const b;
	float penetration;
	Vector2F normal;
	int contactCount;
};
