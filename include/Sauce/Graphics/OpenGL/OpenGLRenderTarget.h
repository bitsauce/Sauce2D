#pragma once

#include <Sauce/Common.h>
#include <Sauce/Graphics/Texture.h>

BEGIN_SAUCE_NAMESPACE

class SAUCE_API OpenGLRenderTarget2D : public RenderTarget2D
{
public:
	OpenGLRenderTarget2D(GraphicsContext *graphicsContext, const uint width, const uint height, const uint targetCount = 1, const PixelFormat &fmt = PixelFormat());
	OpenGLRenderTarget2D(GraphicsContext *graphicsContext, shared_ptr<Texture2D> target);
	~OpenGLRenderTarget2D();

	shared_ptr<Texture2D> getTexture(const uint target = 0) { if(target < m_textureCount) return m_textures[target]; else return nullptr; }

	uint getWidth() const { return m_width; }
	uint getHeight() const { return m_height; }

private:
	void bind();
	void unbind();

	GLuint m_id;
	GLenum *m_buffers;
};

END_SAUCE_NAMESPACE
