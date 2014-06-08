#ifndef GFX_TEXTURE_REGION_H
#define GFX_TEXTURE_REGION_H

#include <x2d/math.h>
#include <x2d/base.h>

class Texture;

class TextureRegion
{
public:
	AS_DECL_REF
	
	TextureRegion(const Texture *texture, const Vector2 &uv0, const Vector2 &uv1);

	void setTexture(const Texture *texture);
	const Texture *getTexture() const;

	Vector2i getSize() const;

	void setRegion(const Vector2 &uv0, const Vector2 &uv1);
	void setRegion(const float u0, const float v0, const float u1, const float v1);

	Vector2 uv0;
	Vector2 uv1;

private:
	static TextureRegion *Factory(const Texture *texture) { return new TextureRegion(texture, Vector2(0.0f), Vector2(1.0f)); }
	static TextureRegion *Factory(const Texture *texture, const Vector2 &uv0, const Vector2 &uv1) { return new TextureRegion(texture, uv0, uv1); }
	static TextureRegion *Factory(const Texture *texture, const float u0, const float v0, const float u1, const float v1) { return new TextureRegion(texture, Vector2(u0, v0), Vector2(u1, v1)); }
	const Texture *texture;
};

#endif // GFX_TEXTUREREGION_H