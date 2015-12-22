//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <CGF/math.h>

//---------------------------------------------------------------------
// Rect - 2D Rects
//---------------------------------------------------------------------

Rect::Rect() :
	position(0, 0),
	size(0, 0)
{
}

Rect::Rect(const Recti &r) :
	position(r.getPos()),
	size(r.getSize())
{
}

Rect::Rect(Vector2 pos, Vector2 size) :
	position(pos),
	size(size)
{
}

Rect::Rect(float x, float y, float width, float height) :
	position(x, y),
	size(width, height)
{
}

//------------------------------------------------------------------------
// Checkers
//------------------------------------------------------------------------

bool Rect::contains(const Vector2 &pt) const
{
	return pt.x > getLeft() && pt.x < getRight() &&
		pt.y < getBottom() && pt.y > getTop();
}

// Technically, this is incorrect. This is a intersect
bool Rect::contains(const Rect &rect) const
{
	return getLeft() < rect.getRight() &&
		getRight() > rect.getLeft() &&
		getTop() < rect.getBottom() &&
		getBottom() > rect.getTop();
}

//------------------------------------------------------------------------
// Getters/Setters
//------------------------------------------------------------------------

void Rect::set(const float x, const float y, const float width, const float height)
{
	position.set(x, y);
	size.set(width, height);
}

void Rect::set(const Vector2 &pos, const Vector2 &size)
{
	this->position = pos;
	this->size = size;
}

void Rect::setPos(const Vector2 &pos)
{
	position = pos;
}

Vector2 Rect::getPos() const
{
	return position;
}

void Rect::setPos(const float x, const float y)
{
	position.set(x, y);
}

void Rect::setX(const float x)
{
	position.x = x;
}

float Rect::getX() const
{
	return position.x;
}

void Rect::setY(const float y)
{
	position.y = y;
}

float Rect::getY() const
{
	return position.y;
}

void Rect::setSize(const Vector2 &size)
{
	this->size = size;
}

Vector2 Rect::getSize() const
{
	return size;
}

void Rect::setSize(const float width, const float height)
{
	size.set(width, height);
}

void Rect::setWidth(const float width)
{
	size.x = width;
}

float Rect::getWidth() const
{
	return size.x;
}

void Rect::setHeight(const float height)
{
	size.y = height;
}

float Rect::getHeight() const
{
	return size.y;
}

Vector2 Rect::getCenter() const
{
	return position+size/2;
}

float Rect::getTop() const
{
	return position.y;
}

float Rect::getLeft() const
{
	return position.x;
}

float Rect::getRight() const
{
	return position.x+size.x;
}

float Rect::getBottom() const
{
	return position.y+size.y;
}

float Rect::getArea() const
{
	return size.x*size.y;
}


//---------------------------------------------------------------------
// Recti - 2D Rects (int)
//---------------------------------------------------------------------

Recti::Recti() :
	position(0, 0),
	size(0, 0)
{
}

Recti::Recti(const Rect &r) :
	position(r.getPos()),
	size(r.getSize())
{
}

Recti::Recti(Vector2i pos, Vector2i size) :
	position(pos),
	size(size)
{
}

Recti::Recti(int x, int y, int width, int height) :
	position(x, y),
	size(width, height)
{
}

//------------------------------------------------------------------------
// Checkers
//------------------------------------------------------------------------

bool Recti::contains(const Vector2i &pt) const
{
	return getLeft() < pt.x &&
		getTop() < pt.y &&
		getRight() > pt.x &&
		getBottom() > pt.y;
}

bool Recti::contains(const Recti &rect) const
{
	return getLeft() < rect.getRight() &&
		getRight() > rect.getLeft() &&
		getTop() < rect.getTop() &&
		getBottom() > rect.getBottom();
}

//------------------------------------------------------------------------
// Getters/Setters
//------------------------------------------------------------------------

void Recti::set(const int x, const int y, const int width, const int height)
{
	position.set(x, y);
	size.set(width, height);
}

void Recti::set(const Vector2i &pos, const Vector2i &size)
{
	this->position = pos;
	this->size = size;
}

void Recti::setPos(const Vector2i &pos)
{
	position = pos;
}

Vector2i Recti::getPos() const
{
	return position;
}

void Recti::setPos(const int x, const int y)
{
	position.set(x, y);
}

void Recti::setX(const int x)
{
	position.x = x;
}

int Recti::getX() const
{
	return position.x;
}

void Recti::setY(const int y)
{
	position.y = y;
}

int Recti::getY() const
{
	return position.y;
}

void Recti::setSize(const Vector2i &size)
{
	this->size = size;
}

Vector2i Recti::getSize() const
{
	return size;
}

void Recti::setSize(const int width, const int height)
{
	size.set(width, height);
}

void Recti::setWidth(const int width)
{
	size.x = width;
}

int Recti::getWidth() const
{
	return size.x;
}

void Recti::setHeight(const int height)
{
	size.y = height;
}

int Recti::getHeight() const
{
	return size.y;
}

Vector2 Recti::getCenter() const
{
	return position+size/2;
}

int Recti::getTop() const
{
	return position.y;
}

int Recti::getLeft() const
{
	return position.x;
}

int Recti::getRight() const
{
	return position.x+size.x;
}

int Recti::getBottom() const
{
	return position.y+size.y;
}

int Recti::getArea() const
{
	return size.x*size.y;
}

//------------------------------------------------------------------------
// AngelScript
//------------------------------------------------------------------------

void Rect::DefaultConstructor(Rect *self)
{
	new(self) Rect();
}

void Rect::CopyConstructor1(const Rect &other, Rect *self)
{
	new(self) Rect(other);
}

void Rect::CopyConstructor2(const Recti &other, Rect *self)
{
	new(self) Rect(other);
}

void Rect::InitConstructor1(const float x, const float y, const float width, const float height, Rect *self)
{
	new(self) Rect(x, y, width, height);
}

void Rect::InitConstructor2(const Vector2 &pos, const Vector2 &size, Rect *self)
{
	new(self) Rect(pos, size);
}

void Recti::DefaultConstructor(Recti *self)
{
	new(self) Recti();
}

void Recti::CopyConstructor1(const Recti &other, Recti *self)
{
	new(self) Recti(other);
}

void Recti::CopyConstructor2(const Rect &other, Recti *self)
{
	new(self) Recti(other);
}

void Recti::InitConstructor1(const int x, const int y, const int width, const int height, Recti *self)
{
	new(self) Recti(x, y, width, height);
}

void Recti::InitConstructor2(const Vector2i &pos, const Vector2i &size, Recti *self)
{
	new(self) Recti(pos, size);
}