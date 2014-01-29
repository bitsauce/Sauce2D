//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)

#include "x2d/math/rect.h"
#include "x2d/scripts.h"

void RegisterRect2()
{
	int r;

	// Register the value type
	AS_REGISTER_VALUE(rect2)
	rect2::TypeId = r;

	// Register the object properties
	AS_VALUE_PROPERTY(rect2, "vec2 pos", m_pos)
	AS_VALUE_PROPERTY(rect2, "vec2 size", m_size)

	// Register the constructors
	AS_VALUE_BEHAVIOURO(rect2, "void f()", asBEHAVE_CONSTRUCT, DefaultConstructor)
	AS_VALUE_BEHAVIOURO(rect2, "void f(const rect2 &in rect)", asBEHAVE_CONSTRUCT, CopyConstructor)
	AS_VALUE_BEHAVIOURO(rect2, "void f(const float x, const float y, const float width, const float height)", asBEHAVE_CONSTRUCT, InitConstructor)
	AS_VALUE_BEHAVIOURO(rect2, "void f(const vec2 &in pos, const vec2 &in size)", asBEHAVE_CONSTRUCT, InitConstructor2)

	// Register the methods
	AS_VALUE_METHODPR(rect2, "bool contains(const vec2 &in pos) const", contains, (const vec2&) const, bool)
	AS_VALUE_METHODPR(rect2, "bool contains(const rect2 &in pos) const", contains, (const rect2&) const, bool)

	// Register property accessors
	AS_VALUE_METHODPR(rect2, "void set(const vec2 &in, const vec2 &in)", set, (const vec2&, const vec2&), void)
	AS_VALUE_METHODPR(rect2, "void set(const float, const float, const float, const float)", set, (const float, const float, const float, const float), void)
	AS_VALUE_METHOD(rect2, "void set_x(const float x)", setX)
	AS_VALUE_METHOD(rect2, "float get_x() const", getX)
	AS_VALUE_METHOD(rect2, "void set_y(const float y)", setY)
	AS_VALUE_METHOD(rect2, "float get_y() const", getY)
	AS_VALUE_METHOD(rect2, "float get_width() const", getWidth)
	AS_VALUE_METHOD(rect2, "void set_width(const float width)", setWidth)
	AS_VALUE_METHOD(rect2, "float get_height() const", getHeight)
	AS_VALUE_METHOD(rect2, "void set_height(const float height)", setHeight)
	AS_VALUE_METHOD(rect2, "vec2 get_center() const", getCenter)
	AS_VALUE_METHOD(rect2, "float get_right() const", getRight)
	AS_VALUE_METHOD(rect2, "float get_bottom() const", getBottom)
	AS_VALUE_METHOD(rect2, "float get_left() const", getLeft)
	AS_VALUE_METHOD(rect2, "float get_top() const", getTop)
	AS_VALUE_METHOD(rect2, "float get_area() const", getArea)
	
	// Register the value type
	AS_REGISTER_VALUE(rect2i)
	rect2i::TypeId = r;

	// Register the object properties
	AS_VALUE_PROPERTY(rect2i, "vec2i pos", m_pos)
	AS_VALUE_PROPERTY(rect2i, "vec2i size", m_size)

	// Register the constructors
	AS_VALUE_BEHAVIOURO(rect2i, "void f()", asBEHAVE_CONSTRUCT, DefaultConstructor)
	AS_VALUE_BEHAVIOURO(rect2i, "void f(const rect2i &in rect)", asBEHAVE_CONSTRUCT, CopyConstructor)
	AS_VALUE_BEHAVIOURO(rect2i, "void f(const rect2 &in rect)", asBEHAVE_CONSTRUCT, CopyConstructor2)
	AS_VALUE_BEHAVIOURO(rect2, "void f(const rect2i &in rect)", asBEHAVE_CONSTRUCT, CopyConstructor2) // rect2 to rect2i
	AS_VALUE_BEHAVIOURO(rect2i, "void f(const int x, const int y, const int width, const int height)", asBEHAVE_CONSTRUCT, InitConstructor)
	AS_VALUE_BEHAVIOURO(rect2i, "void f(const vec2i &in pos, const vec2i &in size)", asBEHAVE_CONSTRUCT, InitConstructor2)

	// Register the methods
	AS_VALUE_METHODPR(rect2i, "bool contains(const vec2i &in pos) const", contains, (const vec2i&) const, bool)
	AS_VALUE_METHODPR(rect2i, "bool contains(const rect2i &in pos) const", contains, (const rect2i&) const, bool)

	// Register property accessors
	AS_VALUE_METHODPR(rect2i, "void set(const vec2i &in, const vec2i &in)", set, (const vec2i&, const vec2i&), void)
	AS_VALUE_METHODPR(rect2i, "void set(const int, const int, const int, const int)", set, (const int, const int, const int, const int), void)
	AS_VALUE_METHOD(rect2i, "void set_x(const int x)", setX)
	AS_VALUE_METHOD(rect2i, "int get_x() const", getX)
	AS_VALUE_METHOD(rect2i, "void set_y(const int y)", setY)
	AS_VALUE_METHOD(rect2i, "int get_y() const", getY)
	AS_VALUE_METHOD(rect2i, "int get_width() const", getWidth)
	AS_VALUE_METHOD(rect2i, "void set_width(const int width)", setWidth)
	AS_VALUE_METHOD(rect2i, "int get_height() const", getHeight)
	AS_VALUE_METHOD(rect2i, "void set_height(const int height)", setHeight)
	AS_VALUE_METHOD(rect2i, "vec2 get_center() const", getCenter)
	AS_VALUE_METHOD(rect2i, "int get_right() const", getRight)
	AS_VALUE_METHOD(rect2i, "int get_bottom() const", getBottom)
	AS_VALUE_METHOD(rect2i, "int get_left() const", getLeft)
	AS_VALUE_METHOD(rect2i, "int get_top() const", getTop)
	AS_VALUE_METHOD(rect2i, "int get_area() const", getArea)
}

//---------------------------------------------------------------------
// rect2 - 2D Rectangles
//---------------------------------------------------------------------

int rect2::TypeId;
rect2::rect2() :
	m_pos(0, 0),
	m_size(0, 0)
{
}

rect2::rect2(const rect2i &r) :
	m_pos(r.getPos()),
	m_size(r.getSize())
{
}

rect2::rect2(vec2 pos, vec2 size) :
	m_pos(pos),
	m_size(size)
{
}

rect2::rect2(float x, float y, float width, float height) :
	m_pos(x, y),
	m_size(width, height)
{
}

//------------------------------------------------------------------------
// Checkers
//------------------------------------------------------------------------

bool rect2::contains(const vec2 &pt) const
{
	return pt.x > getLeft() && pt.x < getRight() &&
		pt.y < getBottom() && pt.y > getTop();
}

// Technically, this is incorrect. This is a intersect
bool rect2::contains(const rect2 &rect) const
{
	return getLeft() < rect.getRight() &&
		getRight() > rect.getLeft() &&
		getTop() < rect.getTop() &&
		getBottom() > rect.getBottom();
}

//------------------------------------------------------------------------
// Getters/Setters
//------------------------------------------------------------------------

void rect2::set(const float x, const float y, const float width, const float height)
{
	m_pos.set(x, y);
	m_size.set(width, height);
}

void rect2::set(const vec2 &pos, const vec2 &size)
{
	m_pos = pos;
	m_size = size;
}

void rect2::setPos(const vec2 &pos)
{
	m_pos = pos;
}

vec2 rect2::getPos() const
{
	return m_pos;
}

void rect2::setPos(const float x, const float y)
{
	m_pos.set(x, y);
}

void rect2::setX(const float x)
{
	m_pos.x = x;
}

float rect2::getX() const
{
	return m_pos.x;
}

void rect2::setY(const float y)
{
	m_pos.y = y;
}

float rect2::getY() const
{
	return m_pos.y;
}

void rect2::setSize(const vec2 &size)
{
	m_size = size;
}

vec2 rect2::getSize() const
{
	return m_size;
}

void rect2::setSize(const float width, const float height)
{
	m_size.set(width, height);
}

void rect2::setWidth(const float width)
{
	m_size.x = width;
}

float rect2::getWidth() const
{
	return m_size.x;
}

void rect2::setHeight(const float height)
{
	m_size.y = height;
}

float rect2::getHeight() const
{
	return m_size.y;
}

vec2 rect2::getCenter() const
{
	return m_pos+m_size/2;
}

float rect2::getTop() const
{
	return m_pos.y;
}

float rect2::getLeft() const
{
	return m_pos.x;
}

float rect2::getRight() const
{
	return m_pos.x+m_size.x;
}

float rect2::getBottom() const
{
	return m_pos.y+m_size.y;
}

float rect2::getArea() const
{
	return m_size.x*m_size.y;
}


//---------------------------------------------------------------------
// rect2i - 2D Rectangles (int)
//---------------------------------------------------------------------

int rect2i::TypeId;
rect2i::rect2i() :
	m_pos(0, 0),
	m_size(0, 0)
{
}

rect2i::rect2i(const rect2 &r) :
	m_pos(r.getPos()),
	m_size(r.getSize())
{
}

rect2i::rect2i(vec2i pos, vec2i size) :
	m_pos(pos),
	m_size(size)
{
}

rect2i::rect2i(int x, int y, int width, int height) :
	m_pos(x, y),
	m_size(width, height)
{
}

//------------------------------------------------------------------------
// Checkers
//------------------------------------------------------------------------

bool rect2i::contains(const vec2i &pt) const
{
	return getLeft() < pt.x &&
		getTop() < pt.y &&
		getRight() > pt.x &&
		getBottom() > pt.y;
}

bool rect2i::contains(const rect2i &rect) const
{
	return getLeft() < rect.getRight() &&
		getRight() > rect.getLeft() &&
		getTop() < rect.getTop() &&
		getBottom() > rect.getBottom();
}

//------------------------------------------------------------------------
// Getters/Setters
//------------------------------------------------------------------------

void rect2i::set(const int x, const int y, const int width, const int height)
{
	m_pos.set(x, y);
	m_size.set(width, height);
}

void rect2i::set(const vec2i &pos, const vec2i &size)
{
	m_pos = pos;
	m_size = size;
}

void rect2i::setPos(const vec2i &pos)
{
	m_pos = pos;
}

vec2i rect2i::getPos() const
{
	return m_pos;
}

void rect2i::setPos(const int x, const int y)
{
	m_pos.set(x, y);
}

void rect2i::setX(const int x)
{
	m_pos.x = x;
}

int rect2i::getX() const
{
	return m_pos.x;
}

void rect2i::setY(const int y)
{
	m_pos.y = y;
}

int rect2i::getY() const
{
	return m_pos.y;
}

void rect2i::setSize(const vec2i &size)
{
	m_size = size;
}

vec2i rect2i::getSize() const
{
	return m_size;
}

void rect2i::setSize(const int width, const int height)
{
	m_size.set(width, height);
}

void rect2i::setWidth(const int width)
{
	m_size.x = width;
}

int rect2i::getWidth() const
{
	return m_size.x;
}

void rect2i::setHeight(const int height)
{
	m_size.y = height;
}

int rect2i::getHeight() const
{
	return m_size.y;
}

vec2 rect2i::getCenter() const
{
	return m_pos+m_size/2;
}

int rect2i::getTop() const
{
	return m_pos.y;
}

int rect2i::getLeft() const
{
	return m_pos.x;
}

int rect2i::getRight() const
{
	return m_pos.x+m_size.x;
}

int rect2i::getBottom() const
{
	return m_pos.y+m_size.y;
}

int rect2i::getArea() const
{
	return m_size.x*m_size.y;
}

//------------------------------------------------------------------------
// AngelScript
//------------------------------------------------------------------------

void rect2::DefaultConstructor(rect2 *self)
{
	new(self) rect2();
}

void rect2::CopyConstructor(const rect2 &other, rect2 *self)
{
	new(self) rect2(other);
}

void rect2::CopyConstructor2(const rect2i &other, rect2 *self)
{
	new(self) rect2(other);
}

void rect2::InitConstructor(const float x, const float y, const float width, const float height, rect2 *self)
{
	new(self) rect2(x, y, width, height);
}

void rect2::InitConstructor2(const vec2 &pos, const vec2 &size, rect2 *self)
{
	new(self) rect2(pos, size);
}

void rect2i::DefaultConstructor(rect2i *self)
{
	new(self) rect2i();
}

void rect2i::CopyConstructor(const rect2i &other, rect2i *self)
{
	new(self) rect2i(other);
}

void rect2i::CopyConstructor2(const rect2 &other, rect2i *self)
{
	new(self) rect2i(other);
}

void rect2i::InitConstructor(const int x, const int y, const int width, const int height, rect2i *self)
{
	new(self) rect2i(x, y, width, height);
}

void rect2i::InitConstructor2(const vec2i &pos, const vec2i &size, rect2i *self)
{
	new(self) rect2i(pos, size);
}