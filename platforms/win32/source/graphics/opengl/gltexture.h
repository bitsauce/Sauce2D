#ifndef WIN_GLTEXTURE_H
#define WIN_GLTEXTURE_H

#include "glgraphics.h"

#include <x2d/graphics/pixmap.h>
#include <x2d/graphics/texture.h>
//#include "../source/gfx/color.h"

class GLtexture : public XTexture
{
	friend class OpenGL;
	friend class GLshader;
	friend class GLframebufferobject;
public:
	GLtexture(const XPixmap &pixmap);
	~GLtexture();

	// Mipmapping
	void enableMipmaps();
	void disableMipmaps();
	bool isMipmapsEnabled() const { return m_mipmaps; }

	// Texture filtering
	void setFiltering(const XTextureFilter filter);
	XTextureFilter getFiltering() const;

	// Texture wrapping
	void setWrapping(const XTextureWrapping wrapping);
	XTextureWrapping getWrapping() const;

	// Size
	int getWidth() const;
	int getHeight() const;

	// Pixmap (texture data)
	XPixmap getPixmap() const;
	void updatePixmap(const XPixmap &pixmap);
	void updatePixmap(const int x, const int y, const XPixmap &pixmap);
	void clear();

private:
	void updateFiltering();

	GLuint m_id;
	GLint m_filter;
	GLint m_wrapping;
	bool m_mipmaps;
	bool m_mipmapsGenerated;
	GLsizei m_width;
	GLsizei m_height;
};

#endif // WIN_GLTEXTURE_H