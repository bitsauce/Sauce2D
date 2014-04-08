#ifndef GFX_TEXTURE_REGION_H
#define GFX_TEXTURE_REGION_H

#include <x2d/math.h>
#include <x2d/base.h>

class Texture;

class TextureRegion
{
public:
	AS_REF_CLASS(TextureRegion)
	AS_FACTORY_ARG1(TextureRegion, const Texture*)
	AS_FACTORY_ARG3(TextureRegion, const Texture*, const Vector2&, const Vector2&)
	AS_FACTORY_ARG5(TextureRegion, const Texture*, const float, const float, const float, const float)

	TextureRegion(const Texture *texture);
	TextureRegion(const Texture *texture, const Vector2 &uv0, const Vector2 &uv1);
	TextureRegion(const Texture *texture, const float u0, const float v0, const float u1, const float v1);
	/*TextureRegion(const TextureRegion &other) :
		refCounter(this),
		uv0(other.uv0),
		uv1(other.uv1),
		texture(other.getTexture())
	{
	}*/
	
	void setTexture(const Texture *texture);
	const Texture *getTexture() const;

	Vector2i getSize() const;

	void setRegion(const Vector2 &uv0, const Vector2 &uv1);
	void setRegion(const float u0, const float v0, const float u1, const float v1);

	Vector2 uv0;
	Vector2 uv1;

private:
	const Texture *texture;
};

#endif // GFX_TEXTUREREGION_H