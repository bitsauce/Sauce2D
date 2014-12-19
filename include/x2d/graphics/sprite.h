#ifndef X2D_SPRITE_H
#define X2D_SPRITE_H

#include "../engine.h"
#include "textureRegion.h"

namespace xd
{

class Texture2D;
class Batch;
class Shape;

class XDAPI Sprite
{
	friend class SpriteBatch;
public:
	Sprite(const TextureRegion &region);
	~Sprite();
	
	void setPosition(const Vector2 &pos);
	void setPosition(const float x, const float y);
	void setX(const float x);
	void setY(const float y);

	void setSize(const Vector2 &size);
	void setSize(const float w, const float h);
	void setWidth(const float w);
	void setHeight(const float h);

	void setOrigin(const Vector2 &origin);
	void setRotation(const float ang);
	void setRegion(const TextureRegion &region, const bool resize = false);
	void setColor(const Color &color);

	void move(const Vector2 &dt);
	void rotate(const float ang);
	void scale(const float);
	
	void getAABB(Vector2 *points) const;

	Vector2 getPosition() const;
	float getX() const;
	float getY() const;

	Vector2 getSize() const;
	float getWidth() const;
	float getHeight() const;

	Vector2 getOrigin() const;
	Vector2 getCenter() const;
	float getRotation() const;
	Color getColor() const;
	TextureRegion getRegion() const;
	Texture2DPtr getTexture() const;

	void draw(Batch *batch) const;

private:
	TextureRegion m_textureRegion;
	Vector2 m_position;
	Vector2 m_size;
	Vector2 m_origin;
	float m_angle;
	Color m_color;

	// Returns the transformed vertices
	void getVertices(Vertex *vertices) const;
};

}

#endif // X2D_SPRITE_H