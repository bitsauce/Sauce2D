#ifndef X2D_FRAME_BUFFER_H
#define X2D_FRAME_BUFFER_H

#include "../engine.h"
#include "texture.h"

namespace xd
{
class GraphicsDevice;
struct PixelFormat;

class XDAPI RenderTarget2D
{
public:
	RenderTarget2D(GraphicsDevice *gd, const uint width, const uint height, const uint targetCount, const PixelFormat &fmt);
	~RenderTarget2D();

private:
	void bind();
	void unbind();

	GLuint m_id;
	float m_ortho[6];
	int m_viewport[4];
	GLenum *m_buffers;

	uint m_width, m_height;
	GraphicsDevice *m_graphicsDevice;

	uint m_textureCount;
	Texture2DPtr *m_textures;
};

}

#endif // X2D_FRAME_BUFFER_H