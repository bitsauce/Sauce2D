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

void XSprite::setRegion(const XTextureRegion &textureRegion, const bool resize)
{
	m_textureRegion = textureRegion;
	if(resize) {
		m_size = m_textureRegion.getSize();
	}
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
	XVertex *vertices = new XVertex[4];
	getVertices(vertices);

	XShape *shape = new XShape(vertices, 4);
	delete[] vertices;
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
	if(batch)
	{
		XVertex *vertices = new XVertex[4];
		getVertices(vertices);

		batch->setTexture(m_textureRegion.getTexture());
		batch->setPrimitive(XBatch::PRIMITIVE_TRIANGLES);
		batch->addVertices(vertices, 4, QUAD_INDICES, 6);
		//batch->release();
		delete[] vertices;
	}
	else
	{
		LOG("void XSprite::draw(): Cannot draw to 'null' batch.");
	}
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
		Vector2 pos = (mat * QUAD_VERTICES[i]).getXY();
		vertices[i].set4f(VERTEX_POSITION, pos.x, pos.y);
		vertices[i].set4ub(VERTEX_COLOR, m_color.x*255, m_color.y*255, m_color.z*255, m_color.w*255);
	}

	vertices[0].set4f(VERTEX_TEX_COORD, m_textureRegion.uv0.x, m_textureRegion.uv1.y);
	vertices[1].set4f(VERTEX_TEX_COORD, m_textureRegion.uv1.x, m_textureRegion.uv1.y);
	vertices[2].set4f(VERTEX_TEX_COORD, m_textureRegion.uv1.x, m_textureRegion.uv0.y);
	vertices[3].set4f(VERTEX_TEX_COORD, m_textureRegion.uv0.x, m_textureRegion.uv0.y);
}