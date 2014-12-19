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

namespace xd {

Sprite::Sprite(const TextureRegion &region) :
	m_textureRegion(region),
	m_position(0.0f),
	m_size(region.getSize()),
	m_origin(0.0f),
	m_angle(0.0f),
	m_color(255)
{
}

Sprite::~Sprite()
{
}

void Sprite::setPosition(const Vector2 &pos)
{
	m_position = pos;
}

void Sprite::setPosition(const float x, const float y)
{
	m_position.set(x, y);
}

void Sprite::setX(const float x)
{
	m_position.x = x;
}

void Sprite::setY(const float y)
{
	m_position.y = y;
}

void Sprite::setSize(const Vector2 &size)
{
	m_size = size;
}

void Sprite::setSize(const float w, const float h)
{
	m_size.set(w, h);
}

void Sprite::setWidth(const float w)
{
	m_size.x = w;
}

void Sprite::setHeight(const float h)
{
	m_size.y = h;
}

void Sprite::setOrigin(const Vector2 &origin)
{
	m_origin = origin;
}

void Sprite::setRotation(const float ang)
{
	m_angle = ang;
}

void Sprite::setRegion(const TextureRegion &textureRegion, const bool resize)
{
	m_textureRegion = textureRegion;
	if(resize) {
		m_size = m_textureRegion.getSize();
	}
}

void Sprite::setColor(const Color &color)
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

void Sprite::getAABB(Vector2 *points) const
{
	Matrix4 mat;
	mat.scale(m_size.x, m_size.y, 1.0f);
	mat.translate(-m_origin.x, -m_origin.y, 0.0f);
	mat.rotateZ(m_angle);
	mat.translate(m_position.x + m_origin.x, m_position.y + m_origin.y, 0.0f);

	for(uint i = 0; i < 4; i++)
	{
		points[i] = (mat * QUAD_VERTICES[i]).getXY();
	}
}

Vector2 Sprite::getPosition() const
{
	return m_position;
}

float Sprite::getX() const
{
	return m_position.x;
}

float Sprite::getY() const
{
	return m_position.y;
}

Vector2 Sprite::getSize() const
{
	return m_size;
}

float Sprite::getWidth() const
{
	return m_size.x;
}

float Sprite::getHeight() const
{
	return m_size.y;
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

Color Sprite::getColor() const
{
	return m_color;
}

TextureRegion Sprite::getRegion() const
{
	return m_textureRegion;
}

Texture2DPtr Sprite::getTexture() const
{
	return m_textureRegion.getTexture();
}

void Sprite::draw(Batch *batch) const
{
	if(batch)
	{
		Vertex vertices[4];
		getVertices(vertices);
		batch->setTexture(m_textureRegion.getTexture());
		batch->setPrimitive(Batch::PRIMITIVE_TRIANGLES);
		batch->addVertices(vertices, 4, QUAD_INDICES, 6);
	}
	else
	{
		LOG("void Sprite::draw(): Cannot draw to 'null' batch.");
	}
}

void Sprite::getVertices(Vertex *vertices) const
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
		vertices[i].set4ub(VERTEX_COLOR, m_color.r, m_color.g, m_color.b, m_color.a);
	}

	vertices[0].set4f(VERTEX_TEX_COORD, m_textureRegion.uv0.x, m_textureRegion.uv1.y);
	vertices[1].set4f(VERTEX_TEX_COORD, m_textureRegion.uv1.x, m_textureRegion.uv1.y);
	vertices[2].set4f(VERTEX_TEX_COORD, m_textureRegion.uv1.x, m_textureRegion.uv0.y);
	vertices[3].set4f(VERTEX_TEX_COORD, m_textureRegion.uv0.x, m_textureRegion.uv0.y);
}

}