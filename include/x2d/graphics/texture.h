#ifndef X2D_TEXTURE_H
#define X2D_TEXTURE_H

#include "../engine.h"

class XPixmap;

enum XTextureFilter
{
	xdNearest,
	xdLinear
};

enum XTextureWrapping
{
	CLAMP_TO_BORDER,
	CLAMP_TO_EDGE,
	REPEAT,
	MIRRORED_REPEAT
};

class XDAPI XTexture
{
public:
	AS_DECL_REF

	XTexture();
	virtual ~XTexture() {}

	// Mipmapping
	virtual void enableMipmaps() = 0;
	virtual void disableMipmaps() = 0;
	virtual bool isMipmapsEnabled() const = 0;

	// Texture filtering
	virtual void setFiltering(const XTextureFilter filter) = 0;
	virtual XTextureFilter getFiltering() const = 0;
	
	// Texture wrapping
	virtual void setWrapping(const XTextureWrapping wrapping) = 0;
	virtual XTextureWrapping getWrapping() const = 0;

	// Size
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
	Vector2i getSize() const { return Vector2i(getWidth(), getHeight()); }

	// Pixmap (texture data)
	virtual XPixmap getPixmap() const = 0;
	virtual void updatePixmap(const XPixmap &pixmap) = 0;
	virtual void updatePixmap(const int x, const int y, const XPixmap &pixmap) = 0;
	virtual void clear() = 0;
};

#endif // X2D_TEXTURE_H