#ifndef CGF_RECTANGLE_H
#define CGF_RECTANGLE_H

#include <CGF/Config.h>
#include <CGF/Math/Vector.h>

/** \brief A class which holds a 2 dimensional Rect
 *
 * The Rect class is a 2 dimensional (hence the 2) Rect.
 * It stores the x and y coordinates, aswell as the width and height, as floating-point values.
 *
 */

template<typename T>
class CGF_API Rect
{
public:
	inline explicit Rect() :
		position(0, 0),
		size(0, 0)
	{
	}

	inline Rect(const Vector2<T> &pos, const Vector2<T> &size) :
		position(pos),
		size(size)
	{
	}

	inline Rect(T x, T y, T width, T height) :
		position(x, y),
		size(width, height)
	{
	}
	
	template<typename U>
	inline Rect(const Rect<U> &other) :
		position(other.position),
		size(other.size)
	{
	}

	inline bool contains(const Vector2<T> &point) const
	{
		return point.x > getLeft() &&
			point.x < getRight() &&
			point.y < getBottom() &&
			point.y > getTop();
	}

	inline bool intersect(const Rect<T> &rect) const
	{
		return getLeft() < rect.getRight() &&
			getRight() > rect.getLeft() &&
			getTop() < rect.getBottom() &&
			getBottom() > rect.getTop();
	}

	inline void set(const Vector2<T> &pos, const Vector2<T> &size)
	{
		this->position = pos;
		this->size = size;
	}


	inline void set(const T x, const T y, const T width, const T height)
	{
		position.set(x, y);
		size.set(width, height);
	}

	inline void setPosition(const Vector2<T> &pos)
	{
		position = pos;
	}

	inline void setPosition(const T x, const T y)
	{
		position.set(x, y);
	}

	inline Vector2<T> getPosition() const
	{
		return position;
	}

	inline void setX(const T x)
	{
		position.x = x;
	}

	inline T getX() const
	{
		return position.x;
	}

	inline void setY(const T y)
	{
		position.y = y;
	}

	inline T getY() const
	{
		return position.y;
	}

	inline void setSize(const Vector2<T> &size)
	{
		this->size = size;
	}

	inline void setSize(const T width, const T height)
	{
		size.set(width, height);
	}

	inline Vector2<T> getSize() const
	{
		return size;
	}

	inline void setWidth(const T width)
	{
		size.x = width;
	}

	inline T getWidth() const
	{
		return size.x;
	}

	inline void setHeight(const T height)
	{
		size.y = height;
	}
	
	inline T getHeight() const
	{
		return size.y;
	}

	inline Vector2<T> getCenter() const
	{
		return position + size / 2;
	}

	inline T getTop() const
	{
		return position.y;
	}

	inline T getBottom() const
	{
		return position.y + size.y;
	}

	inline T getLeft() const
	{
		return position.x;
	}

	inline T getRight() const
	{
		return position.x + size.x;
	}
	
	inline T getArea() const
	{
		return size.x * size.y;
	}

	template<typename U>
	inline Rect<T> &operator=(const Rect<U> &v2)
	{
		position = v2.position;
		size = v2.size;
		return *this;
	}

public:
	Vector2<T> position;
	Vector2<T> size;
};

typedef Rect<bool> RectB;
typedef Rect<float> RectF;
typedef Rect<double> RectD;
typedef Rect<int> RectI;
typedef Rect<uint> RectU;

#endif // CGF_RECTANGLE_H
