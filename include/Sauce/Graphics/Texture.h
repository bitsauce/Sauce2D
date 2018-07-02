#ifndef SAUCE_TEXTURE_H
#define SAUCE_TEXTURE_H

#include <Sauce/Common.h>
#include <Sauce/Graphics/Pixmap.h>

BEGIN_SAUCE_NAMESPACE

class SAUCE_API Texture2D
{
	friend class RenderTarget2D;
	friend class GraphicsContext;
	friend class Shader;
public:
	Texture2D();
	virtual ~Texture2D();

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

	// Texture data functions
	virtual Pixmap getPixmap() const = 0;
	virtual void updatePixmap(const Pixmap &pixmap) = 0;
	virtual void updatePixmap(const uint x, const uint y, const Pixmap &pixmap) = 0;
	virtual void clear() = 0;

	void exportToFile(string path);

protected:
	virtual void updateFiltering() = 0;

	TextureFilter m_filter;
	TextureWrapping m_wrapping;

	bool m_mipmaps;
	bool m_mipmapsGenerated;

	uint m_width;
	uint m_height;
	PixelFormat m_pixelFormat;
};

template class SAUCE_API shared_ptr<Texture2D>;

class TextureResourceDesc : public ResourceDesc
{
public:
	TextureResourceDesc(const string &name, const string &path, const bool premultiplyAlpha) :
		ResourceDesc(RESOURCE_TYPE_TEXTURE, name),
		m_premultiplyAlpha(premultiplyAlpha),
		m_path(path)
	{
	}

	void *create() const;

private:
	const bool m_premultiplyAlpha;
	const string m_path;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_TEXTURE_H
