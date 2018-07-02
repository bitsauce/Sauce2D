//     _____                        ______             _            
//    / ____|                      |  ____|           (_)           
//   | (___   __ _ _   _  ___ ___  | |__   _ __   __ _ _ _ __   ___ 
//    \___ \ / _` | | | |/ __/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \
//    ____) | (_| | |_| | (_|  __/ | |____| | | | (_| | | | | |  __/
//   |_____/ \__,_|\__,_|\___\___| |______|_| |_|\__, |_|_| |_|\___|
//                                                __/ |             
//                                               |___/              
// Made by Marcus "Bitsauce" Loo Vergara
// 2011-2018 (C)

#include <Sauce/Common.h>
#include <Sauce/graphics.h>

BEGIN_SAUCE_NAMESPACE

Sprite::Sprite(shared_ptr<Texture2D> texture, const Rect<float> &rectangle, const Vector2F &origin, const float angle, const TextureRegion &region, const Color &color, const float depth, const Vector2F scale) :
	m_texture(texture),
	m_textureRegion(region),
	m_position(rectangle.position),
	m_size(rectangle.size),
	m_origin(origin),
	m_angle(angle),
	m_color(color),
	m_depth(depth),
	m_scale(scale)
{
}

Sprite::~Sprite()
{
}

void Sprite::setPosition(const Vector2F &pos)
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

void Sprite::setSize(const Vector2F &size)
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

void Sprite::setOrigin(const Vector2F &origin)
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
	if(resize)
	{
		m_size = m_texture != 0 ? Vector2I(
			int(m_texture->getWidth()*m_textureRegion.uv1.x - m_texture->getWidth()*m_textureRegion.uv0.x),
			int(m_texture->getHeight()*m_textureRegion.uv1.y - m_texture->getHeight()*m_textureRegion.uv0.y)
			) : Vector2I(0);
	}
}

void Sprite::setColor(const Color &color)
{
	m_color = color;
}

void Sprite::setDepth(const float depth)
{
	m_depth = depth;
}

float Sprite::getDepth() const
{
	return m_depth;
}

void Sprite::move(const Vector2F &dt)
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

void Sprite::getAABB(Vector2F *points) const
{
	Matrix4 mat;
	mat.scale(m_size.x, m_size.y, 1.0f);
	mat.translate(-m_origin.x, -m_origin.y, 0.0f);
	mat.scale(m_scale.x, m_scale.y, 1.0f);
	mat.rotateZ(m_angle);
	mat.translate(m_position.x, m_position.y, 0.0f);

	for(int i = 0; i < 4; i++)
	{
		points[i] = (mat * QUAD_VERTICES[i]).getXY();
	}
}

Vector2F Sprite::getPosition() const
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

Vector2F Sprite::getSize() const
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

Vector2F Sprite::getOrigin() const
{
	return m_origin;
}

Vector2F Sprite::getCenter() const
{
	if(m_angle == 0.0f)
	{
		return m_position + (m_size / 2.0f);
	}

	// Get center from average of the AABB
	Vector2F points[4];
	getAABB(points);
	Vector2F center;
	for(int i = 0; i < 4; i++)
	{
		center += points[i];
	}
	return center * 0.25f;
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

shared_ptr<Texture2D> Sprite::getTexture() const
{
	return m_texture;
}

void Sprite::getVertices(Vertex *vertices, uint *indices, const uint indexOffset) const
{
	Matrix4 mat;
	mat.scale(m_size.x, m_size.y, 1.0f);
	mat.translate(-m_origin.x, -m_origin.y, 0.0f);
	mat.scale(m_scale.x, m_scale.y, 1.0f);
	mat.rotateZ(m_angle);
	mat.translate(m_position.x, m_position.y, 0.0f);

	for(int i = 0; i < 4; i++)
	{
		Vector2F pos = (mat * QUAD_VERTICES[i]).getXY();
		vertices[i].set2f(VERTEX_POSITION, pos.x, pos.y);
		vertices[i].set4ub(VERTEX_COLOR, m_color.getR(), m_color.getG(), m_color.getB(), m_color.getA());
	}

	vertices[0].set2f(VERTEX_TEX_COORD, m_textureRegion.uv0.x, m_textureRegion.uv0.y);
	vertices[1].set2f(VERTEX_TEX_COORD, m_textureRegion.uv1.x, m_textureRegion.uv0.y);
	vertices[2].set2f(VERTEX_TEX_COORD, m_textureRegion.uv0.x, m_textureRegion.uv1.y);
	vertices[3].set2f(VERTEX_TEX_COORD, m_textureRegion.uv1.x, m_textureRegion.uv1.y);
	
	indices[0] = indexOffset + QUAD_INDICES[0];
	indices[1] = indexOffset + QUAD_INDICES[1];
	indices[2] = indexOffset + QUAD_INDICES[2];
	indices[3] = indexOffset + QUAD_INDICES[3];
	indices[4] = indexOffset + QUAD_INDICES[4];
	indices[5] = indexOffset + QUAD_INDICES[5];
}

END_SAUCE_NAMESPACE
