#ifndef X2D_TEXTURE_H
#define X2D_TEXTURE_H

#include "../engine.h"
#include "pixmap.h"

namespace xd {
	
class Texture2D;
typedef shared_ptr<Texture2D> Texture2DPtr;

class XDAPI Texture2D
{
	friend class RenderTarget2D;
	friend class GraphicsContext;
	friend class Shader;
public:
	Texture2D(const PixelFormat format = RGBA);
	Texture2D(const uint width, const uint height, const Color &color = Color(0), const PixelFormat format = RGBA);
	Texture2D(const Pixmap &pixmap);
	Texture2D(const Texture2D &other);
	~Texture2D();

	// Mipmapping
	void enableMipmaps();
	void disableMipmaps();
	bool isMipmapsEnabled() const;

	// Texture filtering
	enum TextureFilter
	{
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR
	};

	void setFiltering(const TextureFilter filter);
	TextureFilter getFiltering() const;
	
	// Texture wrapping
	enum TextureWrapping
	{
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		REPEAT = GL_REPEAT,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT
	};

	void setWrapping(const TextureWrapping wrapping);
	TextureWrapping getWrapping() const;

	// Size
	uint getWidth() const;
	uint getHeight() const;
	Vector2i getSize() const { return Vector2i(getWidth(), getHeight()); }

	// Pixmap (texture data)
	Pixmap getPixmap(const PixelFormat format = RGBA) const;
	void updatePixmap(const Pixmap &pixmap);
	void updatePixmap(const int x, const int y, const Pixmap &pixmap);
	void clear();

	void exportToFile(string path);

	static Texture2DPtr loadResource(const string &name);

private:
	void init(const Pixmap &pixmap);
	void updateFiltering();

	GLuint m_id;

	GLint m_filter;
	GLint m_wrapping;

	bool m_mipmaps;
	bool m_mipmapsGenerated;

	uint m_width;
	uint m_height;
};

template XDAPI class shared_ptr<Texture2D>;

}

#endif // X2D_TEXTURE_H