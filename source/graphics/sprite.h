#ifndef GRAPHICS_SPRITE_H
#define GRAPHICS_SPRITE_H

#include <x2d/base.h>
#include <x2d/math.h>

#include <x2d/graphics/textureRegion.h>

class Texture;
class Batch;
class Shape;
struct Vertex;

class Sprite
{
	friend class SpriteBatch;
public:
	AS_DECL_REF

	/**
	  * Constructor taking a texture for the sprite to use.
	  */
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
	void setRegion(const TextureRegion &region);
	void setColor(const Vector4 &color);

	void move(const Vector2 &dt);
	void rotate(const float ang);
	void scale(const float);
	
	Shape *getAABB() const;

	Vector2 getPosition() const;
	float getX() const;
	float getY() const;

	Vector2 getSize() const;
	float getWidth() const;
	float getHeight() const;

	Vector2 getOrigin() const;
	Vector2 getCenter() const;
	float getRotation() const;
	Vector4 getColor() const;
	TextureRegion getRegion() const;
	Texture *getTexture() const;

	void draw(Batch *batch) const;

private:
	TextureRegion m_textureRegion;
	Vector2 m_position;
	Vector2 m_size;
	Vector2 m_origin;
	float m_angle;
	Vector4 m_color;

	// Returns the transformed vertices
	void getVertices(Vertex *vertices) const;

	void serialize(StringStream &ss);
	static Sprite *Factory(StringStream &ss);

	static Sprite *Factory(const TextureRegion &region) { return new Sprite(region); }
	static Sprite *Factory(Texture *texture);
};

#endif // GRAPHICS_SPRITE_H