#ifndef GFX_TEXTURE_H
#define GFX_TEXTURE_H

#include <x2d/config.h>
#include <x2d/base.h>
#include <x2d/math.h>

class Pixmap;

enum xdTextureFilter
{
	xdNearest,
	xdLinear
};

enum TextureWrapping
{
	CLAMP_TO_BORDER,
	CLAMP_TO_EDGE,
	REPEAT,
	MIRRORED_REPEAT
};

class XDAPI Texture
{
public:
	AS_DECL_REF

	Texture();
	virtual ~Texture() {}

	// Mipmapping
	virtual void enableMipmaps() = 0;
	virtual void disableMipmaps() = 0;
	virtual bool isMipmapsEnabled() const = 0;

	// Texture filtering
	virtual void setFiltering(const xdTextureFilter filter) = 0;
	virtual xdTextureFilter getFiltering() const = 0;
	
	// Texture wrapping
	virtual void setWrapping(const TextureWrapping wrapping) = 0;
	virtual TextureWrapping getWrapping() const = 0;

	// Size
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;
	Vector2i getSize() const { return Vector2i(getWidth(), getHeight()); }

	// Pixmap (texture data)
	virtual Pixmap getPixmap() const = 0;
	virtual void updatePixmap(const Pixmap &pixmap) = 0;
	virtual void updatePixmap(const int x, const int y, const Pixmap &pixmap) = 0;
	virtual void clear() = 0;
};

#endif // GFX_TEXTURE_H