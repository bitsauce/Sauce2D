#ifndef X2D_RENDER_TARGET_H
#define X2D_RENDER_TARGET_H

#include <x2d/engine.h>
#include <x2d/graphics/graphicsContext.h>
#include <x2d/graphics/texture.h>

BEGIN_XD_NAMESPACE

class XDAPI RenderTarget2D
{
	friend class GraphicsContext;
public:
	RenderTarget2D(const uint width, const uint height, const uint targetCount = 1, const PixelFormat &fmt = PixelFormat());
	RenderTarget2D(Texture2DPtr target);
	~RenderTarget2D();

	Texture2DPtr getTexture(const uint target = 0) { if(target < m_textureCount) return m_textures[target]; else return nullptr; }

	uint getWidth() const { return m_width; }
	uint getHeight() const { return m_height; }

private:
	void bind();
	void unbind();

	GLuint m_id;
	GLenum *m_buffers;

	uint m_width, m_height;
	uint m_textureCount;
	Texture2DPtr *m_textures;
};

END_XD_NAMESPACE

#endif // X2D_RENDER_TARGET_H