#include <Sauce/Sauce.h>
#include "Shapes.h"
#include "Manifold.h"

using namespace sauce;

class PhysicsEngineGame : public Game
{
	vector<Shape*> shapes;
	Shape *selectedShape = 0;
	Vector2F m_lastMousePosition;

public:
	PhysicsEngineGame() :
		Game("PhysicsEngine")
	{
	}

	void onStart(GameEvent *e)
	{
		Circle *circle1 = new Circle;
		circle1->position.set(100.f, 100.f);
		circle1->radius = 25.0f;
		shapes.push_back(circle1);

		Circle *circle2 = new Circle;
		circle2->position.set(350.f, 200.f);
		circle2->radius = 15.0f;
		shapes.push_back(circle2);

		Box *box1 = new Box;
		box1->min.set(200.0f, 200.0f);
		box1->max.set(300.0f, 250.0f);
		shapes.push_back(box1);

		Box *box2 = new Box;
		box2->min.set(400.0f, 400.0f);
		box2->max.set(550.0f, 500.0f);
		shapes.push_back(box2);

		Game::onStart(e);
	}

	void onEnd(GameEvent *e)
	{
		Game::onEnd(e);
	}

	void onMouseDown(MouseEvent *e)
	{
		Vector2F inputPos = e->getPosition();
		for(Shape *shape : shapes)
		{
			switch(shape->getType())
			{
				case Shape::BOX:
				{
					Box *aabb = static_cast<Box*>(shape);
					if(inputPos.x >= aabb->min.x && inputPos.x <= aabb->max.x && inputPos.y >= aabb->min.y && inputPos.y <= aabb->max.y)
					{
						selectedShape = shape;
					}
				}
				break;

				case Shape::CIRCLE:
				{
					Circle *circle = static_cast<Circle*>(shape);
					if((circle->position - inputPos).lengthSquared() < circle->radius * circle->radius)
					{
						selectedShape = shape;
					}
				}
				break;
			}
		}
	}

	void onMouseUp(MouseEvent *e)
	{
		selectedShape = 0;
	}

	void onMouseMove(MouseEvent *e)
	{
		m_lastMousePosition = e->getPosition();
	}

	void onTick(TickEvent *e)
	{
		// Mouse drag movement
		if(selectedShape)
		{
			switch(selectedShape->getType())
			{
			case Shape::BOX:
			{
				Box *aabb = static_cast<Box*>(selectedShape);
				aabb->velocity = (getInputManager()->getPosition() - aabb->getCentroid()) * 0.25f;
			}
			break;

			case Shape::CIRCLE:
			{
				Circle *circle = static_cast<Circle*>(selectedShape);
				circle->velocity = (getInputManager()->getPosition() - circle->position) * 0.25f;
			}
			break;
			}
		}

		for(Shape *shape : shapes)
		{
			bool colliding = false;
			for(Shape *otherShape : shapes)
			{
				if(shape == otherShape) continue;

				// Circle to circle collision
				if(shape->getType() == Shape::CIRCLE && otherShape->getType() == Shape::CIRCLE)
				{
					Manifold manifold(shape, otherShape);
					CircleToCircle(&manifold);
					if(manifold.contactCount > 0)
					{
						ResolveCollision(&manifold, shape, otherShape);
						colliding = true;
						break;
					}
				}
				// Box to circle collision
				else if(shape->getType() == Shape::BOX && otherShape->getType() == Shape::CIRCLE)
				{
					Manifold manifold(shape, otherShape);
					if(AABBToCircle(&manifold))
					{
						colliding = true;
						break;
					}
				}
				// Circle to box collision
				else if(shape->getType() == Shape::CIRCLE && otherShape->getType() == Shape::BOX)
				{
					Manifold manifold(otherShape, shape);
					if(AABBToCircle(&manifold))
					{
						colliding = true;
						break;
					}
				}
				// Box to box collision
				else if(shape->getType() == Shape::BOX && otherShape->getType() == Shape::BOX)
				{
					Manifold manifold(shape, otherShape);
					if(AABBToAABB(&manifold))
					{
						colliding = true;
						break;
					}
				}
			}
			shape->colliding = colliding;
			
			switch(shape->getType())
			{
			case Shape::BOX:
			{
				Box *aabb = static_cast<Box*>(shape);
				aabb->min += aabb->velocity;
				aabb->max += aabb->velocity;
			}
			break;

			case Shape::CIRCLE:
			{
				Circle *circle = static_cast<Circle*>(shape);
				circle->position += circle->velocity;
			}
			break;
			}
		}
		Game::onTick(e);
	}

	void onDraw(DrawEvent *e)
	{
		for(Shape *shape : shapes)
		{
			Color c = shape->colliding ? Color::Blue : Color::White;
			switch(shape->getType())
			{
				case Shape::BOX:
				{
					Box *aabb = static_cast<Box*>(shape);
					e->getGraphicsContext()->drawRectangle(aabb->min, aabb->max - aabb->min, c);
				}
				break;

				case Shape::CIRCLE:
				{
					Circle *circle = static_cast<Circle*>(shape);
					e->getGraphicsContext()->drawCircle(circle->position, circle->radius, 32, c);
				}
				break;
			}
			e->getGraphicsContext()->drawArrow(shape->getCentroid(), shape->getCentroid() + shape->velocity, Color::Red);
		}
		Game::onDraw(e);
	}

	void ResolveCollision(Manifold *m, Shape *a, Shape *b)
	{
		// Calculate relative velocity
		Vector2F relativeVel = b->velocity - a->velocity;

		// Calculate relative velocity in terms of the normal direction
		float velAlongNormal = relativeVel.dot(m->normal);

		// Do not resolve if velocities are separating
		if(velAlongNormal > 0)
			return;

		// Calculate restitution
		float e = min(a->restitution, b->restitution);

		// Calculate impulse scalar
		float j = -(1 + e) * velAlongNormal;
		j /= 1 / a->mass + 1 / b->mass;

		// Apply impulse
		Vector2F impulse = m->normal * j;
		a->velocity -= impulse * (1 / a->mass);
		b->velocity += impulse * (1 / b->mass);
	}

	void CircleToCircle(Manifold *m)
	{
		Circle *a = static_cast<Circle*>(m->a);
		Circle *b = static_cast<Circle*>(m->b);

		// Calculate vector from a to b
		Vector2F normal = b->position - a->position;
		float lengthSquared = normal.lengthSquared();
		float radius = a->radius + b->radius;

		// If their combined radius is less than the distance between,
		// there is no contact
		if(lengthSquared >= radius * radius)
		{
			m->contactCount = 0;
			return;
		}

		// There is contact, calculate the distance using sqrt
		float distance = std::sqrt(lengthSquared);
		m->contactCount = 1;
		if(distance == 0.0f)
		{
			// If the circles are at the exact same point,
			// just pick a pre-determined normal vector
			m->penetration = a->radius;
			m->normal = Vector2F(1, 0);
		}
		else
		{
			// Calculate the penetation and normal vector
			m->penetration = radius - distance;
			m->normal = normal / distance; // Normalize the vector forom a to b
			                               // (will be the collision normal)
		}
	}

	bool AABBToCircle(Manifold *m)
	{
		Box *a = static_cast<Box*>(m->a);
		Circle *b = static_cast<Circle*>(m->b);

		Vector2F pointOnCircle = b->position + ((a->getCentroid() - b->position).normalized() * b->radius);
		return RectF(a->min, a->max - a->min).contains(pointOnCircle);
	}

	bool AABBToAABB(Manifold *m)
	{
		Box *a = static_cast<Box*>(m->a);
		Box *b = static_cast<Box*>(m->b);

		// Exit with no intersection if found separated along an axis
		if(a->max.x < b->min.x || a->min.x > b->max.x) return false;
		if(a->max.y < b->min.y || a->min.y > b->max.y) return false;

		// No separating axis found, therefor there is at least one overlapping axis
		return true;
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	PhysicsEngineGame game;
	return game.run();
}