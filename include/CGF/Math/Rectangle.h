#ifndef CGF_RECT_H
#define CGF_RECT_H

#include <CGF/config.h>
#include <CGF/Math/Vector.h>

/** \brief A class which holds a 2 dimensional Rect
 *
 * The Rect class is a 2 dimensional (hence the 2) Rect.
 * It stores the x and y coordinates, aswell as the width and height, as floating-point values.
 *
 */

class Recti;

class CGF_API Rect
{
public:
	Rect();
	Rect(const Recti &r);
	Rect(Vector2 pos, Vector2 size);
	Rect(float x, float y, float width, float height);
	
	// Containers
	bool contains(const Vector2 &point) const;
	bool contains(const Rect &rect) const;

	// Getters/Setters
	void set(const Vector2 &pos, const Vector2 &size);
	void set(const float x, const float y, const float width, const float height);
	void setPos(const Vector2 &pos);
	void setPos(const float x, const float y);
	Vector2 getPos() const;
	void setX(const float x);
	float getX() const;
	void setY(const float y);
	float getY() const;
	void setSize(const Vector2 &size);
	void setSize(const float width, const float height);
	Vector2 getSize() const;
	void setWidth(const float width);
	float getWidth() const;
	void setHeight(const float height);
	float getHeight() const;

	// Getters only
	Vector2 getCenter() const;
	float getTop() const;
	float getBottom() const;
	float getLeft() const;
	float getRight() const;
	float getArea() const;

public:

	// AngelScript
	static void DefaultConstructor(Rect *self);
	static void CopyConstructor1(const Rect &other, Rect *self);
	static void CopyConstructor2(const Recti &other, Rect *self);
	static void InitConstructor1(const float x, const float y, const float width, const float height, Rect *self);
	static void InitConstructor2(const Vector2 &pos, const Vector2 &size, Rect *self);

public:
	Vector2 position;
	Vector2 size;
};

class CGF_API Recti
{
public:
	Recti();
	Recti(const Rect &r);
	Recti(Vector2i pos, Vector2i size);
	Recti(int x, int y, int width, int height);
	
	// Containers
	bool contains(const Vector2i &point) const;
	bool contains(const Recti &rect) const;

	// Getters/Setters
	void set(const Vector2i &pos, const Vector2i &size);
	void set(const int x, const int y, const int width, const int height);
	void setPos(const Vector2i &pos);
	void setPos(const int x, const int y);
	Vector2i getPos() const;
	void setX(const int x);
	int getX() const;
	void setY(const int y);
	int getY() const;
	void setSize(const Vector2i &size);
	void setSize(const int width, const int height);
	Vector2i getSize() const;
	void setWidth(const int width);
	int getWidth() const;
	void setHeight(const int height);
	int getHeight() const;

	// Getters only
	Vector2 getCenter() const;
	int getTop() const;
	int getBottom() const;
	int getLeft() const;
	int getRight() const;
	int getArea() const;

	bool operator<(const Recti &other) const
	{
		TUPLE_CMP(this->getPos(), other.getPos())
		TUPLE_CMP(this->getSize(), other.getSize())
		return false;
	}

	bool operator>(const Recti &other) const
	{
		TUPLE_CMP2(this->getPos(), other.getPos())
		TUPLE_CMP2(this->getSize(), other.getSize())
		return false;
	}

public:

	// AngelScript
	static void DefaultConstructor(Recti *self);
	static void CopyConstructor1(const Recti &other, Recti *self);
	static void CopyConstructor2(const Rect &other, Recti *self);
	static void InitConstructor1(const int x, const int y, const int width, const int height, Recti *self);
	static void InitConstructor2(const Vector2i &pos, const Vector2i &size, Recti *self);

public:
	Vector2i position;
	Vector2i size;
};

#endif // CGF_RECT_H
