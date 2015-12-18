#ifndef X2D_TEXTURE_REGION_H
#define X2D_TEXTURE_REGION_H

#include <x2d/engine.h>
#include <x2d/graphics/texture.h>

BEGIN_CG_NAMESPACE

class XDAPI TextureRegion
{
public:
	TextureRegion();
	TextureRegion(const Vector2 &uv0, const Vector2 &uv1);
	TextureRegion(const float u0, const float v0, const float u1, const float v1);
	TextureRegion(const TextureRegion &other);
	~TextureRegion();

	TextureRegion &operator=(const TextureRegion &other);

	void setRegion(const Vector2 &uv0, const Vector2 &uv1);
	void setRegion(const float u0, const float v0, const float u1, const float v1);

	//TextureRegion getSubRegion(const Vector2 &uv0, const Vector2 &uv1);
	//TextureRegion getSubRegion(const float u0, const float v0, const float u1, const float v1);

	Vector2 uv0;
	Vector2 uv1;
};

END_CG_NAMESPACE

#endif // X2D_TEXTURE_REGION_H