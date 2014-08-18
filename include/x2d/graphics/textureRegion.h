#ifndef GFX_TEXTURE_REGION_H
#define GFX_TEXTURE_REGION_H

#include <x2d/math.h>
#include <x2d/base.h>

class Texture;

class TextureRegion
{
public:
	AS_DECL_VALUE
	
	TextureRegion(Texture *texture, const Vector2 &uv0, const Vector2 &uv1);
	TextureRegion(Texture *texture, const float u0, const float v0, const float u1, const float v1);
	TextureRegion(const TextureRegion &other);
	~TextureRegion();

	void setTexture(Texture *texture);
	Texture *getTexture() const;

	Vector2i getSize() const;

	void setRegion(const Vector2 &uv0, const Vector2 &uv1);
	void setRegion(const float u0, const float v0, const float u1, const float v1);

	Vector2 uv0;
	Vector2 uv1;

	TextureRegion &operator=(const TextureRegion &other);

private:
	
	void serialize(StringStream &ss) const;
	void deserialize(StringStream &ss);

	static void Factory(Texture *texture, TextureRegion *self) { new (self) TextureRegion(texture, Vector2(0.0f), Vector2(1.0f)); }
	static void Factory(Texture *texture, const Vector2 &uv0, const Vector2 &uv1, TextureRegion *self) { new (self) TextureRegion(texture, uv0, uv1); }
	static void Factory(Texture *texture, const float u0, const float v0, const float u1, const float v1, TextureRegion *self) { new (self) TextureRegion(texture, u0, v0, u1, v1); }
	static void Factory(const TextureRegion &other, TextureRegion *self) { new (self) TextureRegion(other); }
	static void Destruct(TextureRegion *self) { self->~TextureRegion(); }
	Texture *texture;
};

#endif // GFX_TEXTUREREGION_H