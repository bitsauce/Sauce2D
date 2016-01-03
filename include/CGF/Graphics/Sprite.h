#ifndef CGF_SPRITE_H
#define CGF_SPRITE_H

#include <CGF/Common.h>
#include <cgf/graphics/textureRegion.h>

BEGIN_CGF_NAMESPACE

class Texture2D;
class Shape;

class CGF_API Sprite
{
	friend class SpriteBatch;
public:
	Sprite(const Texture2DPtr texture = nullptr, const Rect &rectangle = Rect(0, 0, 0, 0), const Vector2 &origin = Vector2(0.0f, 0.0f), const float angle = 0.0f, const TextureRegion &region = TextureRegion(), const Color &color = Color(255), const float depth = 0.0f, const Vector2 scale = Vector2(1.0f, 1.0f));
	//Sprite(const Texture2DPtr texture, const Rect &rectangle, const TextureRegion &region = TextureRegion(), const Color &color = Color(255), const float depth = 0.0f);
	//Sprite(const Texture2DPtr texture, const Rect &rectangle, const TextureRegion &region = TextureRegion(), const Color &color = Color(255), const float depth = 0.0f);
	//Sprite(const Texture2DPtr texture, const Vector2 &center, const TextureRegion &region = TextureRegion(), const Color &color = Color(255), const float depth = 0.0f);
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
	void setScale(const Vector2 &scale) { m_scale = scale; }
	void setScaleX(const float x) { m_scale.x = x; }
	void setScaleY(const float y) { m_scale.y = y; }
	void setRegion(const TextureRegion &region, const bool resize = false);
	void setColor(const Color &color);

	void setDepth(const float depth);
	float getDepth() const;

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
	Vector2 getScale() const { return m_scale; }
	float getRotation() const;
	Color getColor() const;
	TextureRegion getRegion() const;
	void setTexture(const Texture2DPtr texture) { m_texture = texture; }
	Texture2DPtr getTexture() const;

private:
	Texture2DPtr m_texture;
	TextureRegion m_textureRegion;
	Vector2 m_position;
	Vector2 m_size;
	Vector2 m_origin;
	Vector2 m_scale;
	float m_depth;
	float m_angle;
	Color m_color;

	// Returns the transformed vertices
	void getVertices(Vertex *vertices, uint *indices, const uint indexOffset = 0) const;
};

END_CGF_NAMESPACE

#endif // CGF_SPRITE_H
