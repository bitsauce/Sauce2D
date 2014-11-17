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
	friend class XFrameBufferObject;
	friend class XGraphics;
	friend class XShader;
public:
	XTexture();
	XTexture(const string &path);
	XTexture(const XPixmap &pixmap);
	XTexture(const uint width, const uint height, const Vector4 &color = Vector4(0.0f));
	XTexture(const XTexture &other);
	~XTexture();

	void init(const XPixmap &pixmap);

	// Mipmapping
	void enableMipmaps();
	void disableMipmaps();
	bool isMipmapsEnabled() const;

	// Texture filtering
	void setFiltering(const XTextureFilter filter);
	XTextureFilter getFiltering() const;
	
	// Texture wrapping
	void setWrapping(const XTextureWrapping wrapping);
	XTextureWrapping getWrapping() const;

	// Size
	int getWidth() const;
	int getHeight() const;
	Vector2i getSize() const { return Vector2i(getWidth(), getHeight()); }

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

#endif // X2D_TEXTURE_H