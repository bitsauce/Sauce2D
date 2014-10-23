#ifndef X2D_TEXTURE_REGION_H
#define X2D_TEXTURE_REGION_H

#include "../engine.h"

class XTexture;

class XTextureRegion
{
public:
	AS_DECL_POD
	
	XTextureRegion();
	XTextureRegion(XTexture *texture, const Vector2 &uv0, const Vector2 &uv1);
	XTextureRegion(XTexture *texture, const float u0, const float v0, const float u1, const float v1);
	XTextureRegion(const XTextureRegion &other);
	~XTextureRegion();

	void setTexture(XTexture *texture);
	XTexture *getTexture() const;

	Vector2i getSize() const;

	void setRegion(const Vector2 &uv0, const Vector2 &uv1);
	void setRegion(const float u0, const float v0, const float u1, const float v1);

	Vector2 uv0;
	Vector2 uv1;

	XTextureRegion &operator=(const XTextureRegion &other);

private:
	
	static void Construct(XTextureRegion *self) { new (self) XTextureRegion(); }
	static void Construct(XTexture *texture, XTextureRegion *self) { new (self) XTextureRegion(texture, Vector2(0.0f), Vector2(1.0f)); }
	static void Construct(XTexture *texture, const Vector2 &uv0, const Vector2 &uv1, XTextureRegion *self) { new (self) XTextureRegion(texture, uv0, uv1); }
	static void Construct(XTexture *texture, const float u0, const float v0, const float u1, const float v1, XTextureRegion *self) { new (self) XTextureRegion(texture, u0, v0, u1, v1); }
	static void Construct(const XTextureRegion &other, XTextureRegion *self) { new (self) XTextureRegion(other); }
	static void Destruct(XTextureRegion *self) { self->~XTextureRegion(); }
	XTexture *texture;
};

#endif // X2D_TEXTURE_REGION_H