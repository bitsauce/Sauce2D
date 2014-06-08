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
#include "shape.h"

#include <x2d/graphics.h>

AS_REG_REF(Sprite)

int Sprite::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("Sprite", asBEHAVE_FACTORY, "Sprite @f(const TextureRegion @, const bool center = false)", asFUNCTIONPR(Sprite::Factory, (const TextureRegion*, const bool), Sprite*), asCALL_CDECL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("Sprite", "void setPosition(const Vector2 &in)", asMETHODPR(Sprite, setPosition, (const Vector2 &), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "void setOrigin(const Vector2 &in)", asMETHODPR(Sprite, setOrigin, (const Vector2 &), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "void setRotation(const float)", asMETHODPR(Sprite, setRotation, (const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "void setScale(const Vector2 &in)", asMETHODPR(Sprite, setScale, (const Vector2 &), void), asCALL_THISCALL); AS_ASSERT

	return r;
}

Sprite::Sprite(const TextureRegion *region, const bool center) :
	refCounter(this),
	m_textureRegion(region),
	m_position(0.0f, 0.0f),
	m_size(0.0f, 0.0f),
	m_scale(0.0f, 0.0f),
	m_origin(0.0f, 0.0f),
	m_angle(0.0f)
{
	m_size = m_textureRegion->getSize();
	if(center) {
		m_origin = getCenter();
	}
}

void Sprite::setPosition(const Vector2 &pos)
{
	m_position = pos;
}

void Sprite::setSize(const Vector2 &size)
{
	m_size = size;
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
	m_position += dt;
}

void Sprite::rotate(const float ang)
{
	m_angle += ang;
}

void Sprite::scale(const Vector2 &scl)
{
	m_scale += scl;
}

Shape Sprite::getAABB() const
{
	return Shape(Rect(m_position, m_size)); // TODO: Fix me!
}

Vector2 Sprite::getPosition() const
{
	return m_position;
}

Vector2 Sprite::getSize() const
{
	return m_size;
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
	return m_position + (m_size/2.0f);
}

float Sprite::getRotation() const
{
	return m_angle;
}

int QUAD_INDICES[6] = { 0, 1, 3, 1, 2, 3 };
float QUAD_VERTICES[4][2] = { { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f } };
Matrix4 QUAD = Matrix4(	0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						1.0f, 1.0f, 0.0f, 0.0f,
						1.0f, 0.0f, 0.0f, 0.0f	);

void Sprite::draw(Batch &batch) const
{
	Matrix4 mat;
	mat.translate(m_position.x, m_position.y, 0);
	mat.rotate(m_angle, 0.0f, 0.0f, 1.0f);
	mat.scale(m_size.x, m_size.y, 1.0f);

	Matrix4 mat2 = mat * QUAD;

	//batch->addVertices(vertices, QUAD_INDICES, 4);
}