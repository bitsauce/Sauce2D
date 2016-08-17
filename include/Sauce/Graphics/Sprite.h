#ifndef SAUCE_SPRITE_H
#define SAUCE_SPRITE_H

#include <Sauce/Common.h>
#include <Sauce/Graphics/TextureRegion.h>

BEGIN_SAUCE_NAMESPACE

class Texture2D;
class Shape;

class SAUCE_API Sprite
{
	friend class SpriteBatch;
public:
	Sprite(shared_ptr<Texture2D> texture = 0, const Rect<float> &rectangle = Rect<float>(0, 0, 0, 0), const Vector2F &origin = Vector2F(0.0f, 0.0f), const float angle = 0.0f, const TextureRegion &region = TextureRegion(), const Color &color = Color(255), const float depth = 0.0f, const Vector2F scale = Vector2F(1.0f, 1.0f));
	//Sprite(const Resource<Texture2D> texture, const Rect &rectangle, const TextureRegion &region = TextureRegion(), const Color &color = Color(255), const float depth = 0.0f);
	//Sprite(const Resource<Texture2D> texture, const Rect &rectangle, const TextureRegion &region = TextureRegion(), const Color &color = Color(255), const float depth = 0.0f);
	//Sprite(const Resource<Texture2D> texture, const Vector2F &center, const TextureRegion &region = TextureRegion(), const Color &color = Color(255), const float depth = 0.0f);
	~Sprite();
	
	void setPosition(const Vector2F &pos);
	void setPosition(const float x, const float y);
	void setX(const float x);
	void setY(const float y);

	void setSize(const Vector2F &size);
	void setSize(const float w, const float h);
	void setWidth(const float w);
	void setHeight(const float h);

	void setOrigin(const Vector2F &origin);
	void setRotation(const float ang);
	void setScale(const Vector2F &scale) { m_scale = scale; }
	void setScaleX(const float x) { m_scale.x = x; }
	void setScaleY(const float y) { m_scale.y = y; }
	void setRegion(const TextureRegion &region, const bool resize = false);
	void setColor(const Color &color);

	void setDepth(const float depth);
	float getDepth() const;

	void move(const Vector2F &dt);
	void rotate(const float ang);
	void scale(const float);
	
	void getAABB(Vector2F *points) const;

	Vector2F getPosition() const;
	float getX() const;
	float getY() const;

	Vector2F getSize() const;
	float getWidth() const;
	float getHeight() const;

	Vector2F getOrigin() const;
	Vector2F getCenter() const;
	Vector2F getScale() const { return m_scale; }
	float getRotation() const;
	Color getColor() const;
	TextureRegion getRegion() const;
	void setTexture(shared_ptr<Texture2D> texture) { m_texture = texture; }
	shared_ptr<Texture2D> getTexture() const;

private:
	shared_ptr<Texture2D> m_texture;
	TextureRegion m_textureRegion;
	Vector2F m_position;
	Vector2F m_size;
	Vector2F m_origin;
	Vector2F m_scale;
	float m_depth;
	float m_angle;
	Color m_color;

	// Returns the transformed vertices
	void getVertices(Vertex *vertices, uint *indices, const uint indexOffset = 0) const;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_SPRITE_H
