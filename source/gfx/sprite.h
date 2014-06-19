#ifndef GFX_SPRITE_H
#define GFX_SPRITE_H

#include <x2d/base.h>
#include <x2d/math.h>

class TextureRegion;
class Texture;
class Batch;
class Shape;
struct Vertex;

class Sprite
{
public:
	AS_DECL_REF

	/**
	  * Constructor taking a texture for the sprite to use.
	  */
	Sprite(TextureRegion *region);
	//Sprite(const TextureRegion &region, const Vector2 &size, const Vector2 &origin);
	~Sprite();
	
	void setPosition(const Vector2 &pos);
	void setSize(const Vector2 &size);
	void setOrigin(const Vector2 &origin);
	void setRotation(const float ang);
	void setRegion(TextureRegion *region);

	void move(const Vector2 &dt);
	void rotate(const float ang);
	void scale(const float);
	
	Shape *getAABB() const;
	Vector2 getPosition() const;
	Vector2 getSize() const;
	Vector2 getOrigin() const;
	Vector2 getCenter() const;
	float getRotation() const;

	void draw(Batch *batch) const;

private:
	TextureRegion *m_textureRegion;
	Vector2 m_position;
	Vector2 m_size;
	Vector2 m_origin;
	float m_angle;

	// Returns the transformed vertices
	void getVertices(Vertex *vertices) const;

	static Sprite *Factory(TextureRegion *region) { return new Sprite(region); }
	static Sprite *Factory(Texture *texture);
};

#endif // GFX_SPRITE_H