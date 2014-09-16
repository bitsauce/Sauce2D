#ifndef X2D_SPRITE_H
#define X2D_SPRITE_H

#include "common/engine.h"
#include "textureRegion.h"

class XTexture;
class XBatch;
class XShape;
struct Vertex;

class XSprite
{
	friend class XSpriteBatch;
public:
	AS_DECL_REF

	/**
	  * Constructor taking a texture for the sprite to use.
	  */
	XSprite(const XTextureRegion &region);
	~XSprite();
	
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
	void setRegion(const XTextureRegion &region);
	void setColor(const Vector4 &color);

	void move(const Vector2 &dt);
	void rotate(const float ang);
	void scale(const float);
	
	XShape *getAABB() const;

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
	XTextureRegion getRegion() const;
	XTexture *getTexture() const;

	void draw(XBatch *batch) const;

private:
	XTextureRegion m_textureRegion;
	Vector2 m_position;
	Vector2 m_size;
	Vector2 m_origin;
	float m_angle;
	Vector4 m_color;

	// Returns the transformed vertices
	void getVertices(Vertex *vertices) const;

	static XSprite *Factory(const XTextureRegion &region) { return new XSprite(region); }
	static XSprite *Factory(XTexture *texture);
};

#endif // X2D_SPRITE_H