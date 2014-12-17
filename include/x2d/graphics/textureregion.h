#ifndef X2D_TEXTURE_REGION_H
#define X2D_TEXTURE_REGION_H

#include "../engine.h"
#include "texture.h"

namespace xd
{

class XDAPI TextureRegion
{
public:
	TextureRegion();
	TextureRegion(const Texture2DPtr &resource);
	TextureRegion(const Texture2DPtr &resource, const Vector2 &uv0, const Vector2 &uv1);
	TextureRegion(const Texture2DPtr &resource, const float u0, const float v0, const float u1, const float v1);
	TextureRegion(const TextureRegion &other);
	~TextureRegion();

	void setTexture(const Texture2DPtr &texture);
	Texture2DPtr getTexture() const;

	Vector2i getSize() const;

	void setRegion(const Vector2 &uv0, const Vector2 &uv1);
	void setRegion(const float u0, const float v0, const float u1, const float v1);

	Vector2 uv0;
	Vector2 uv1;

	TextureRegion &operator=(const TextureRegion &other);

private:
	Texture2DPtr texture;
};

}

#endif // X2D_TEXTURE_REGION_H