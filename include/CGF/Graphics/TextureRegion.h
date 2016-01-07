#ifndef CGF_TEXTURE_REGION_H
#define CGF_TEXTURE_REGION_H

#include <CGF/Common.h>

BEGIN_CGF_NAMESPACE

class CGF_API TextureRegion
{
public:
	TextureRegion();
	TextureRegion(const Vector2F &uv0, const Vector2F &uv1);
	TextureRegion(const float u0, const float v0, const float u1, const float v1);
	TextureRegion(const TextureRegion &other);
	~TextureRegion();

	TextureRegion &operator=(const TextureRegion &other);

	void setRegion(const Vector2F &uv0, const Vector2F &uv1);
	void setRegion(const float u0, const float v0, const float u1, const float v1);

	//TextureRegion getSubRegion(const Vector2 &uv0, const Vector2 &uv1);
	//TextureRegion getSubRegion(const float u0, const float v0, const float u1, const float v1);

	Vector2F uv0;
	Vector2F uv1;
};

END_CGF_NAMESPACE

#endif // CGF_TEXTURE_REGION_H
