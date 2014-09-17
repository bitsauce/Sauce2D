//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/graphics.h>

AS_REG_REF(XSprite)

int XSprite::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("XSprite", asBEHAVE_FACTORY, "XSprite @f(const TextureRegion &in)", asFUNCTIONPR(Factory, (const XTextureRegion&), XSprite*), asCALL_CDECL); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("XSprite", asBEHAVE_FACTORY, "XSprite @f(Texture@)", asFUNCTIONPR(Factory, (XTexture*), XSprite*), asCALL_CDECL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("XSprite", "void setPosition(const Vector2 &in)", asMETHODPR(XSprite, setPosition, (const Vector2&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "void setPosition(const float, const float)", asMETHODPR(XSprite, setPosition, (const float, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "void setX(const float)", asMETHOD(XSprite, setX), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "void setY(const float)", asMETHOD(XSprite, setY), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "void setSize(const Vector2 &in)", asMETHODPR(XSprite, setSize, (const Vector2&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "void setSize(const float, const float)", asMETHODPR(XSprite, setSize, (const float, const float), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "void setWidth(const float)", asMETHOD(XSprite, setWidth), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "void setHeight(const float)", asMETHOD(XSprite, setHeight), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "void setOrigin(const Vector2 &in)", asMETHOD(XSprite, setOrigin), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "void setRotation(const float)", asMETHOD(XSprite, setRotation), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "void setRegion(const TextureRegion &in)", asMETHOD(XSprite, setRegion), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "void setColor(const Vector4 &in)", asMETHOD(XSprite, setColor), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("XSprite", "Shape @getAABB() const", asMETHOD(XSprite, getAABB), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "Vector2 getPosition() const", asMETHOD(XSprite, getPosition), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "float getX() const", asMETHOD(XSprite, getX), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "float getY() const", asMETHOD(XSprite, getY), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "Vector2 getSize() const", asMETHOD(XSprite, getSize), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "float getWidth() const", asMETHOD(XSprite, getWidth), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "float getHeight() const", asMETHOD(XSprite, getHeight), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "Vector2 getOrigin() const", asMETHOD(XSprite, getOrigin), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "Vector2 getCenter() const", asMETHOD(XSprite, getCenter), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "Vector4 getColor() const", asMETHOD(XSprite, getColor), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "TextureRegion getRegion() const", asMETHOD(XSprite, getRegion), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "Texture @getTexture() const", asMETHOD(XSprite, getTexture), asCALL_THISCALL); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("XSprite", "void move(const Vector2 &in)", asMETHOD(XSprite, move), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "void rotate(const float)", asMETHOD(XSprite, rotate), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XSprite", "void scale(const float)", asMETHOD(XSprite, scale), asCALL_THISCALL); AS_ASSERT
	
	r = scriptEngine->RegisterObjectMethod("XSprite", "void draw(Batch @batch)", asMETHOD(XSprite, draw), asCALL_THISCALL); AS_ASSERT

	return r;
}

XSprite *XSprite::Factory(XTexture *texture)
{
	return new XSprite(XTextureRegion(texture, Vector2(0.0f), Vector2(1.0f)));
}

XSprite::XSprite(const XTextureRegion &region) :
	m_textureRegion(region),
	m_position(0.0f),
	m_size(region.getSize()),
	m_origin(0.0f),
	m_angle(0.0f),
	m_color(1.0f)
{
}

XSprite::~XSprite()
{
}

void XSprite::setPosition(const Vector2 &pos)
{
	m_position = pos;
}

void XSprite::setPosition(const float x, const float y)
{
	m_position.set(x, y);
}

void XSprite::setX(const float x)
{
	m_position.x = x;
}

void XSprite::setY(const float y)
{
	m_position.y = y;
}

void XSprite::setSize(const Vector2 &size)
{
	m_size = size;
}

void XSprite::setSize(const float w, const float h)
{
	m_size.set(w, h);
}

void XSprite::setWidth(const float w)
{
	m_size.x = w;
}

void XSprite::setHeight(const float h)
{
	m_size.y = h;
}

void XSprite::setOrigin(const Vector2 &origin)
{
	m_origin = origin;
}

void XSprite::setRotation(const float ang)
{
	m_angle = ang;
}

void XSprite::setRegion(const XTextureRegion &textureRegion)
{
	m_textureRegion = textureRegion;
}

void XSprite::setColor(const Vector4 &color)
{
	m_color = color;
}

void XSprite::move(const Vector2 &dt)
{
	m_position += dt;
}

void XSprite::rotate(const float ang)
{
	m_angle += ang;
}

void XSprite::scale(const float scl)
{
	m_size *= scl;
}

XShape *XSprite::getAABB() const
{
	XVertex vertices[4];
	getVertices(vertices);

	XShape *shape = new XShape();
	shape->m_vertices.assign(vertices, vertices + 4);
	shape->m_indices.assign(QUAD_INDICES, QUAD_INDICES + 6);

	return shape;
}

Vector2 XSprite::getPosition() const
{
	return m_position;
}

float XSprite::getX() const
{
	return m_position.x;
}

float XSprite::getY() const
{
	return m_position.y;
}

Vector2 XSprite::getSize() const
{
	return m_size;
}

float XSprite::getWidth() const
{
	return m_size.x;
}

float XSprite::getHeight() const
{
	return m_size.y;
}

Vector2 XSprite::getOrigin() const
{
	return m_origin;
}

Vector2 XSprite::getCenter() const
{
	return m_position + (m_size/2.0f);
}

float XSprite::getRotation() const
{
	return m_angle;
}

Vector4 XSprite::getColor() const
{
	return m_color;
}

XTextureRegion XSprite::getRegion() const
{
	return m_textureRegion;
}

XTexture *XSprite::getTexture() const
{
	return m_textureRegion.getTexture();
}

void XSprite::draw(XBatch *batch) const
{
	XVertex vertices[4];
	getVertices(vertices);

	batch->setTexture(m_textureRegion.getTexture());
	batch->setPrimitive(XBatch::PRIMITIVE_TRIANGLES);
	batch->addVertices(vertices, 4, QUAD_INDICES, 6);
	batch->release();
}

void XSprite::getVertices(XVertex *vertices) const
{
	Matrix4 mat;
	mat.scale(m_size.x, m_size.y, 1.0f);
	mat.translate(-m_origin.x, -m_origin.y, 0.0f);
	mat.rotateZ(m_angle);
	mat.translate(m_position.x + m_origin.x, m_position.y + m_origin.y, 0.0f);

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