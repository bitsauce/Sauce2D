//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "sprite.h"
#include "texture.h"
#include "textureRegion.h"
#include "batch.h"

AS_REGISTER(Sprite)

int Sprite::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	AS_REGISTER_FACTORY(Sprite, "const TextureRegion @, const bool center = false", (const TextureRegion *, const bool))

	AS_REGISTER_METHOD(Sprite, "void setPosition(const Vector2 &in)", setPosition, (const Vector2 &), void)
	AS_REGISTER_METHOD(Sprite, "void setOrigin(const Vector2 &in)", setOrigin, (const Vector2 &), void)
	AS_REGISTER_METHOD(Sprite, "void setRotation(const float)", setRotation, (const float), void)
	AS_REGISTER_METHOD(Sprite, "void setScale(const Vector2 &in)", setScale, (const Vector2 &), void)

	return r;
}

Sprite::Sprite(const TextureRegion *region, const bool center) :
	refCounter(this),
	m_textureRegion(region),
	m_rectangle(0.0f, 0.0f, 0.0f, 0.0f),
	m_scale(0.0f, 0.0f),
	m_origin(0.0f, 0.0f),
	m_angle(0.0f)
{
	m_rectangle.size = m_textureRegion->getSize();
	if(center) {
		m_origin = m_rectangle.getCenter();
	}
}

void Sprite::setPosition(const Vector2 &pos)
{
	m_rectangle.position = pos;
}

void Sprite::setSize(const Vector2 &size)
{
	m_rectangle.size = size;
}

void Sprite::setScale(const Vector2 &scale)
{
	m_scale = scale;
}

void Sprite::setOrigin(const Vector2 &origin)
{
	m_origin = origin;
}

void Sprite::setRotation(const float ang)
{
	m_angle = ang;
}

void Sprite::move(const Vector2 &dt)
{
	m_rectangle.position += dt;
}

void Sprite::rotate(const float ang)
{
	m_angle += ang;
}

void Sprite::scale(const Vector2 &scl)
{
	m_scale += scl;
}

Rect Sprite::getRect() const
{
	return m_rectangle;
}

Vector2 Sprite::getPosition() const
{
	return m_rectangle.position;
}

Vector2 Sprite::getSize() const
{
	return m_rectangle.size;
}

Vector2 Sprite::getScale() const
{
	return m_scale;
}

Vector2 Sprite::getOrigin() const
{
	return m_origin;
}

Vector2 Sprite::getCenter() const
{
	return m_rectangle.getCenter();
}

float Sprite::getRotation() const
{
	return m_angle;
}

void Sprite::draw(const Batch *batch) const
{
	
}