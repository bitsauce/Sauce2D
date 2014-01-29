#ifndef MATH_RECT_H
#define MATH_RECT_H

#include "x2d/platform.h"
#include "vector.h"

/** \brief A class which holds a 2 dimentional rectangle
 *
 * The rect2 class is a 2 dimentional (hence the 2) rectangle.
 * It stores the x and y coordinates, aswell as the width and height, as floating-point values.
 *
 */

void RegisterRect2();
class rect2i;

class X2DAPI rect2
{
public:
	rect2();
	rect2(const rect2i &r);
	rect2(vec2 pos, vec2 size);
	rect2(float x, float y, float width, float height);
	
	// Containers
	bool contains(const vec2 &point) const;
	bool contains(const rect2 &rect) const;

	// Getters/Setters
	void set(const vec2 &pos, const vec2 &size);
	void set(const float x, const float y, const float width, const float height);
	void setPos(const vec2 &pos);
	void setPos(const float x, const float y);
	vec2 getPos() const;
	void setX(const float x);
	float getX() const;
	void setY(const float y);
	float getY() const;
	void setSize(const vec2 &size);
	void setSize(const float width, const float height);
	vec2 getSize() const;
	void setWidth(const float width);
	float getWidth() const;
	void setHeight(const float height);
	float getHeight() const;

	// Getters only
	vec2 getCenter() const;
	float getTop() const;
	float getBottom() const;
	float getLeft() const;
	float getRight() const;
	float getArea() const;

public:

	// AngelScript
	static void DefaultConstructor(rect2 *self);
	static void CopyConstructor(const rect2 &other, rect2 *self);
	static void CopyConstructor2(const rect2i &other, rect2 *self);
	static void InitConstructor(const float x, const float y, const float width, const float height, rect2 *self);
	static void InitConstructor2(const vec2 &pos, const vec2 &size, rect2 *self);

public:
	vec2 m_pos;
	vec2 m_size;
	
	// Type id
	static int TypeId;
};

#define TUPLE_CMP(a, b) \
	if(a < b) return true; \
	if(a > b) return false;

#define TUPLE_CMP2(a, b) \
	if(a < b) return false; \
	if(a > b) return true;

class X2DAPI rect2i
{
public:
	rect2i();
	rect2i(const rect2 &r);
	rect2i(vec2i pos, vec2i size);
	rect2i(int x, int y, int width, int height);
	
	// Containers
	bool contains(const vec2i &point) const;
	bool contains(const rect2i &rect) const;

	// Getters/Setters
	void set(const vec2i &pos, const vec2i &size);
	void set(const int x, const int y, const int width, const int height);
	void setPos(const vec2i &pos);
	void setPos(const int x, const int y);
	vec2i getPos() const;
	void setX(const int x);
	int getX() const;
	void setY(const int y);
	int getY() const;
	void setSize(const vec2i &size);
	void setSize(const int width, const int height);
	vec2i getSize() const;
	void setWidth(const int width);
	int getWidth() const;
	void setHeight(const int height);
	int getHeight() const;

	// Getters only
	vec2 getCenter() const;
	int getTop() const;
	int getBottom() const;
	int getLeft() const;
	int getRight() const;
	int getArea() const;

	bool operator<(const rect2i &other) const
	{
		TUPLE_CMP(this->getPos(), other.getPos())
		TUPLE_CMP(this->getSize(), other.getSize())
		return false;
	}

	bool operator>(const rect2i &other) const
	{
		TUPLE_CMP2(this->getPos(), other.getPos())
		TUPLE_CMP2(this->getSize(), other.getSize())
		return false;
	}

public:

	// AngelScript
	static void DefaultConstructor(rect2i *self);
	static void CopyConstructor(const rect2i &other, rect2i *self);
	static void CopyConstructor2(const rect2 &other, rect2i *self);
	static void InitConstructor(const int x, const int y, const int width, const int height, rect2i *self);
	static void InitConstructor2(const vec2i &pos, const vec2i &size, rect2i *self);

public:
	vec2i m_pos;
	vec2i m_size;
	
	// Type id
	static int TypeId;
};

#endif // MATH_RECT_H