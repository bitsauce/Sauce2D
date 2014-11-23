#ifndef X2D_TEXTURE_H
#define X2D_TEXTURE_H

#include "../engine.h"

class XPixmap;
struct XColor;

class XDAPI XTexture
{
	friend class XFrameBufferObject;
	friend class XGraphics;
	friend class XShader;
public:
	XTexture();
	XTexture(const uint width, const uint height, const XColor &color = XColor(0));
	XTexture(const XPixmap &pixmap);
	XTexture(const XTexture &other);
	~XTexture();

	// Mipmapping
	void enableMipmaps();
	void disableMipmaps();
	bool isMipmapsEnabled() const;

	// Texture filtering
	enum TextureFilter
	{
		NEAREST,
		LINEAR
	};
	void setFiltering(const TextureFilter filter);
	TextureFilter getFiltering() const;
	
	// Texture wrapping
	enum TextureWrapping
	{
		CLAMP_TO_BORDER,
		CLAMP_TO_EDGE,
		REPEAT,
		MIRRORED_REPEAT
	};
	void setWrapping(const TextureWrapping wrapping);
	TextureWrapping getWrapping() const;

	// Size
	uint getWidth() const;
	uint getHeight() const;
	Vector2i getSize() const { return Vector2i(getWidth(), getHeight()); }

	// Pixmap (texture data)
	XPixmap getPixmap() const;
	void updatePixmap(const XPixmap &pixmap);
	void updatePixmap(const int x, const int y, const XPixmap &pixmap);
	void clear();

	static shared_ptr<XTexture> loadResource(const string &name);

private:
	void init(const XPixmap &pixmap);
	void updateFiltering();

	GLuint m_id;

	GLint m_filter;
	GLint m_wrapping;

	bool m_mipmaps;
	bool m_mipmapsGenerated;

	uint m_width;
	uint m_height;
};

template XDAPI class shared_ptr<XTexture>;

#endif // X2D_TEXTURE_H