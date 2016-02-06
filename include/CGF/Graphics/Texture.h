#ifndef CGF_TEXTURE_H
#define CGF_TEXTURE_H

#include <CGF/Common.h>
#include <CGF/Graphics/Pixmap.h>

BEGIN_CGF_NAMESPACE

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
	Vector2I getSize() const { return Vector2I(getWidth(), getHeight()); }

	// Pixmap (texture data)
	Pixmap getPixmap() const;
	void updatePixmap(const Pixmap &pixmap);
	void updatePixmap(const uint x, const uint y, const Pixmap &pixmap);
	void clear();

	void exportToFile(string path);

	Texture2D(ResourceDesc *desc);

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

template class CGF_API shared_ptr<Texture2D>;

class TextureResourceDesc : public ResourceDesc
{
public:
	TextureResourceDesc(const string &name, const string &path, const bool premultiplyAlpha) :
		ResourceDesc(RESOURCE_TYPE_TEXTURE, name),
		m_premultiplyAlpha(premultiplyAlpha),
		m_path(path)
	{
	}

	string getPath() const
	{
		return m_path;
	}

	bool getPremultiplyAlpha() const
	{
		return m_premultiplyAlpha;
	}

private:
	const bool m_premultiplyAlpha;
	const string m_path;
};

class FontResourceDesc : public ResourceDesc
{
public:
	FontResourceDesc(const string &name, const string &path, const bool premultiplyAlpha) :
		ResourceDesc(RESOURCE_TYPE_FONT, name),
		m_premultiplyAlpha(premultiplyAlpha),
		m_path(path)
	{
	}

	string getPath() const
	{
		return m_path;
	}

	bool getPremultiplyAlpha() const
	{
		return m_premultiplyAlpha;
	}

private:
	const bool m_premultiplyAlpha;
	const string m_path;
};

END_CGF_NAMESPACE

#endif // CGF_TEXTURE_H
