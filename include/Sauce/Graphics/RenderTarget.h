#ifndef SAUCE_RENDER_TARGET_H
#define SAUCE_RENDER_TARGET_H

#include <Sauce/Common.h>
#include <Sauce/graphics/graphicsContext.h>
#include <Sauce/graphics/texture.h>

BEGIN_SAUCE_NAMESPACE

class SAUCE_API RenderTarget2D
{
	friend class GraphicsContext;
public:
	RenderTarget2D(const uint width, const uint height, const uint targetCount = 1, const PixelFormat &fmt = PixelFormat());
	RenderTarget2D(Resource<Texture2D> target);
	~RenderTarget2D();

	Resource<Texture2D> getTexture(const uint target = 0) { if(target < m_textureCount) return m_textures[target]; else return nullptr; }

	uint getWidth() const { return m_width; }
	uint getHeight() const { return m_height; }

private:
	void bind();
	void unbind();

	GLuint m_id;
	GLenum *m_buffers;

	uint m_width, m_height;
	uint m_textureCount;
	Resource<Texture2D> *m_textures;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_RENDER_TARGET_H
