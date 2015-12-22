#ifndef CGF_TEXTURE_H
#define CGF_TEXTURE_H

#include <CGF/Common.h>
#include <CGF/Graphics/Pixmap.h>

BEGIN_CGF_NAMESPACE
	
class Texture2D;
typedef shared_ptr<Texture2D> Texture2DPtr;

class CGF_API Texture2D
{
	friend class RenderTarget2D;
	friend class GraphicsContext;
	friend class Shader;
public:
	Texture2D(const PixelFormat &format = PixelFormat());
	Texture2D(const uint width, const uint height, const void *data = 0, const PixelFormat &format = PixelFormat());
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
	Pixmap getPixmap() const;
	void updatePixmap(const Pixmap &pixmap);
	void updatePixmap(const uint x, const uint y, const Pixmap &pixmap);
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
	PixelFormat m_pixelFormat;
};

template CGF_API class shared_ptr<Texture2D>;

END_CGF_NAMESPACE

#endif // CGF_TEXTURE_H
