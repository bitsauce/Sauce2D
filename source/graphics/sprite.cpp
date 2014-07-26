//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "sprite.h"
#include <x2d/graphics/texture.h>
#include <x2d/graphics/textureRegion.h>
#include <x2d/graphics/batch.h>
#include "shape.h"

#include <x2d/graphics.h>

AS_REG_REF(Sprite)

int Sprite::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("Sprite", asBEHAVE_FACTORY, "Sprite @f(const TextureRegion &in)", asFUNCTIONPR(Factory, (const TextureRegion&), Sprite*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Sprite", asBEHAVE_FACTORY, "Sprite @f(Texture@)", asFUNCTIONPR(Factory, (Texture*), Sprite*), asCALL_CDECL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("Sprite", "void setPosition(const Vector2 &in)", asMETHOD(Sprite, setPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "void setSize(const Vector2 &in)", asMETHOD(Sprite, setSize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "void setOrigin(const Vector2 &in)", asMETHOD(Sprite, setOrigin), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "void setRotation(const float)", asMETHOD(Sprite, setRotation), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "void setRegion(const TextureRegion &in)", asMETHOD(Sprite, setRegion), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "void setColor(const Vector4 &in)", asMETHOD(Sprite, setColor), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("Sprite", "Shape @getAABB() const", asMETHOD(Sprite, getAABB), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "Vector2 getPosition() const", asMETHOD(Sprite, getPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "Vector2 getSize() const", asMETHOD(Sprite, getSize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "Vector2 getOrigin() const", asMETHOD(Sprite, getOrigin), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "Vector2 getCenter() const", asMETHOD(Sprite, getCenter), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "Vector4 getColor() const", asMETHOD(Sprite, getColor), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "TextureRegion getRegion() const", asMETHOD(Sprite, getRegion), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "Texture @getTexture() const", asMETHOD(Sprite, getTexture), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Sprite", "void move(const Vector2 &in)", asMETHOD(Sprite, move), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "void rotate(const float)", asMETHOD(Sprite, rotate), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("Sprite", "void scale(const float)", asMETHOD(Sprite, scale), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("Sprite", "void draw(Batch @batch)", asMETHOD(Sprite, draw), asCALL_THISCALL); AS_ASSERT

	return r;
}

Sprite *Sprite::Factory(Texture *texture)
{
	texture->addRef();
	return new Sprite(TextureRegion(texture, Vector2(0.0f), Vector2(1.0f)));
}

Sprite::Sprite(const TextureRegion &region) :
	m_textureRegion(region),
	m_position(0.0f),
	m_size(region.getSize()),
	m_origin(0.0f),
	m_angle(0.0f),
	m_color(1.0f)
{
}

Sprite::~Sprite()
{
}

void Sprite::setPosition(const Vector2 &pos)
{
	m_position = pos;
}

void Sprite::setSize(const Vector2 &size)
{
	m_size = size;
}

void Sprite::setOrigin(const Vector2 &origin)
{
	m_origin = origin;
}

void Sprite::setRotation(const float ang)
{
	m_angle = ang;
}

void Sprite::setRegion(const TextureRegion &textureRegion)
{
	m_textureRegion = textureRegion;
}

void Sprite::setColor(const Vector4 &color)
{
	m_color = color;
}

void Sprite::move(const Vector2 &dt)
{
	m_position += dt;
}

void Sprite::rotate(const float ang)
{
	m_angle += ang;
}

void Sprite::scale(const float scl)
{
	m_size *= scl;
}

Shape *Sprite::getAABB() const
{
	Vertex vertices[4];
	getVertices(vertices);

	Shape *shape = new Shape();
	shape->m_vertices.assign(vertices, vertices + 4);
	shape->m_indices.assign(QUAD_INDICES, QUAD_INDICES + 6);

	return shape;
}

Vector2 Sprite::getPosition() const
{
	return m_position;
}

Vector2 Sprite::getSize() const
{
	return m_size;
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

Vector4 Sprite::getColor() const
{
	return m_color;
}

TextureRegion Sprite::getRegion() const
{
	return m_textureRegion;
}

Texture *Sprite::getTexture() const
{
	return m_textureRegion.getTexture();
}

void Sprite::draw(Batch *batch) const
{
	Vertex vertices[4];
	getVertices(vertices);

	batch->setTexture(m_textureRegion.getTexture());
	batch->setPrimitive(Batch::PRIMITIVE_TRIANGLES);
	batch->addVertices(vertices, 4, QUAD_INDICES, 6);
	batch->release();
}

void Sprite::getVertices(Vertex *vertices) const
{
	Matrix4 mat;
	mat.scale(m_size.x, m_size.y, 1.0f);
	mat.translate(-m_origin.x, -m_origin.y, 0.0f);
	mat.rotateZ(m_angle);
	mat.translate(m_position.x+m_origin.x, m_position.y+m_origin.y, 0.0f);

	for(int i = 0; i < 4; i++)
	{
		vertices[i].position = mat * QUAD_VERTICES[i];
		vertices[i].color = m_color;
	}

	vertices[0].texCoord.set(m_textureRegion.uv0.x, m_textureRegion.uv1.y);
	vertices[1].texCoord.set(m_textureRegion.uv1.x, m_textureRegion.uv1.y);
	vertices[2].texCoord.set(m_textureRegion.uv1.x, m_textureRegion.uv0.y);
	vertices[3].texCoord.set(m_textureRegion.uv0.x, m_textureRegion.uv0.y);
}