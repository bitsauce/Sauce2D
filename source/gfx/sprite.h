#ifndef GFX_SPRITE_H
#define GFX_SPRITE_H

#include <x2d/base.h>
#include <x2d/math.h>

class TextureRegion;
class Batch;
class Animation;

class Sprite
{
public:
	AS_REF_CLASS(Sprite)
	AS_FACTORY_ARG2(Sprite, const TextureRegion *, const bool)

	/**
	  * Constructor taking a texture for the sprite to use.
	  */
	Sprite(const TextureRegion *region, const bool center = false);
	//Sprite(const TextureRegion &region, const Vector2 &size, const Vector2 &origin);
	
	void setPosition(const Vector2 &pos);
	void setSize(const Vector2 &size);
	void setScale(const Vector2 &scl);
	void setOrigin(const Vector2 &origin);
	void setRotation(const float ang);
	void setAnimation(const Animation *anim);

	void move(const Vector2 &dt);
	void rotate(const float ang);
	void scale(const Vector2 &scl);
	
	Rect getRect() const;
	Vector2 getPosition() const;
	Vector2 getSize() const;
	Vector2 getScale() const;
	Vector2 getOrigin() const;
	Vector2 getCenter() const;
	float getRotation() const;

	void draw(const Batch *batch) const;


private:
	const TextureRegion *m_textureRegion;
	Rect m_rectangle;
	Vector2 m_scale;
	Vector2 m_origin;
	float m_angle;
};

#endif // GFX_SPRITE_H