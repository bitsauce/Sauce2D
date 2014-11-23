#ifndef X2D_TEXTURE_REGION_H
#define X2D_TEXTURE_REGION_H

#include "../engine.h"

class XTexture;

class XDAPI XTextureRegion
{
public:
	XTextureRegion();
	XTextureRegion(const shared_ptr<XTexture> &resource);
	XTextureRegion(const shared_ptr<XTexture> &resource, const Vector2 &uv0, const Vector2 &uv1);
	XTextureRegion(const shared_ptr<XTexture> &resource, const float u0, const float v0, const float u1, const float v1);
	XTextureRegion(const XTextureRegion &other);
	~XTextureRegion();

	void setTexture(const shared_ptr<XTexture> &texture);
	shared_ptr<XTexture> getTexture() const;

	Vector2i getSize() const;

	void setRegion(const Vector2 &uv0, const Vector2 &uv1);
	void setRegion(const float u0, const float v0, const float u1, const float v1);

	Vector2 uv0;
	Vector2 uv1;

	XTextureRegion &operator=(const XTextureRegion &other);

private:
	shared_ptr<XTexture> texture;
};

#endif // X2D_TEXTURE_REGION_H