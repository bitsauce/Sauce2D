//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/graphics.h>

namespace xd {

RenderTarget2D::RenderTarget2D(GraphicsDevice *gd, const uint width, const uint height, const uint count, const PixelFormat &fmt) :
	m_width(width),
	m_height(height),
	m_graphicsDevice(gd),
	m_textureCount(count)
{
	// Generate framebuffer
	glGenFramebuffers(1, &m_id);

	// Allocate textures
	m_textures = new Texture2DPtr[m_textureCount];
	m_buffers = new GLenum[m_textureCount];
	for(uint i = 0; i < m_textureCount; ++i)
	{
		m_textures[i] = Texture2DPtr(new Texture2D(width, height));
		m_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
}

RenderTarget2D::~RenderTarget2D()
{
	glDeleteFramebuffers(1, &m_id);
}

void RenderTarget2D::bind()
{
	// Bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	for(uint i = 0; i < m_textureCount; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i]->m_id, 0);
	}
	glDrawBuffers(m_textureCount, m_buffers);

	// Setup viewport and projection
	m_graphicsDevice->getOrthoProjection(m_ortho[0], m_ortho[1], m_ortho[2], m_ortho[3], m_ortho[4], m_ortho[5]);
	m_graphicsDevice->getViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
		
	m_graphicsDevice->setOrthoProjection(0.0f, (float)m_width, (float)m_height, 0.0f, m_ortho[4], m_ortho[5]);
	m_graphicsDevice->setViewport(Recti(0, 0, m_width, m_height));
}

void RenderTarget2D::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_graphicsDevice->setOrthoProjection(m_ortho[0], m_ortho[1], m_ortho[2], m_ortho[3], m_ortho[4], m_ortho[5]);
	m_graphicsDevice->setViewport(Recti(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]));
}

}