#ifndef GFX_TEXTURE_H
#define GFX_TEXTURE_H

#include <x2d/base.h>

class Pixmap;
struct Color;

enum xdTextureFilter
{
	xdNearest,
	xdLinear
};

class Texture
{
public:
	AS_REF_CLASS(Texture)

	Texture();
	virtual ~Texture();

	// Mipmapping
	virtual void enableMipmaps() = 0;
	virtual void disableMipmaps() = 0;

	// Texture filtering
	virtual void setFiltering(const xdTextureFilter filter) = 0;

	// Size
	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;

	// Pixmap (texture data)
	virtual Pixmap getPixmap() const = 0;
	virtual void updatePixmap(const Pixmap &pixmap) = 0;
	virtual void updatePixmap(const int x, const int y, const Pixmap &pixmap) = 0;
	virtual void clear() = 0;
};

#endif // GFX_TEXTURE_H