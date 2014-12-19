#ifndef X2D_RENDER_TARGET_H
#define X2D_RENDER_TARGET_H

#include "../engine.h"
#include "graphicscontext.h"
#include "texture.h"

namespace xd {

class XDAPI RenderTarget2D
{
	friend class GraphicsContext;
public:
	RenderTarget2D(const uint width, const uint height, const uint targetCount, const PixelFormat fmt = RGBA);
	~RenderTarget2D();

	Texture2DPtr getTexture(const uint target = 0) { if(target < m_textureCount) return m_textures[target]; else return nullptr; }

private:
	void bind();
	void unbind();

	GLuint m_id;
	GLenum *m_buffers;

	uint m_width, m_height;
	uint m_textureCount;
	Texture2DPtr *m_textures;
};

}

#endif // X2D_RENDER_TARGET_H