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

}

#endif // X2D_TEXTURE_REGION_H