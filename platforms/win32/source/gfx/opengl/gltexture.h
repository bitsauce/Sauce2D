#ifndef GFX_GLTEXTURE_H
#define GFX_GLTEXTURE_H

#include "glgraphics.h"

#include <x2d/graphics/pixmap.h>
#include <x2d/graphics/texture.h>
//#include "../source/gfx/color.h"

class GLtexture : public Texture
{
	friend class OpenGL;
	friend class GLshader;
	friend class GLframebufferobject;
public:
	GLtexture(const Pixmap &pixmap);
	~GLtexture();

	// Mipmapping
	void enableMipmaps();
	void disableMipmaps();

	// Texture filtering
	void setFiltering(const xdTextureFilter filter);

	// Size
	int getWidth() const;
	int getHeight() const;

	// Pixmap (texture data)
	Pixmap getPixmap() const;
	void updatePixmap(const Pixmap &pixmap);
	void updatePixmap(const int x, const int y, const Pixmap &pixmap);
	void clear();

private:
	void updateFiltering();

	GLuint m_id;
	GLint m_filter;
	bool m_mipmaps;
	bool m_mipmapsGenerated;
	GLsizei m_width;
	GLsizei m_height;
};

#endif // GFX_GLTEXTURE_H