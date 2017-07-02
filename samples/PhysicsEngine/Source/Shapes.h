#pragma once

#include <Sauce/Sauce.h>

class Shape
{
public:
	enum Type
	{
		BOX,
		CIRCLE
	};

protected:
	Shape(Type type) : m_type(type) { }

public:
	virtual Vector2F getCentroid() const = 0;

	Type getType() const { return m_type; }

private:
	const Type m_type; public:
	bool colliding = false;
	float mass = 2.0;
	Vector2F velocity;
	float restitution = 1.0f;
};

class Box : public Shape
{
public:
	Box() : Shape(BOX) { }
	Vector2F getCentroid() const { return (min + max) / 2; }


//private:
	Vector2F min, max;
};

class Circle : public Shape
{
public:
	Circle() : Shape(CIRCLE) {}
	Vector2F getCentroid() const { return position; }

//private:
	Vector2F position;
	float radius;
};