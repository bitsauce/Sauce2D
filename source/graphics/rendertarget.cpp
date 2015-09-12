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

BEGIN_XD_NAMESPACE

RenderTarget2D::RenderTarget2D(const uint width, const uint height, const uint count, const PixelFormat &fmt) :
	m_width(width),
	m_height(height),
	m_textureCount(count)
{
	// Generate framebuffer
	glGenFramebuffers(1, &m_id);

	// Allocate textures
	m_textures = new Texture2DPtr[m_textureCount];
	m_buffers = new GLenum[m_textureCount];
	for(uint i = 0; i < m_textureCount; ++i)
	{
		m_textures[i] = Texture2DPtr(new Texture2D(width, height, 0, fmt));
		m_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
}

RenderTarget2D::~RenderTarget2D()
{
	glDeleteFramebuffers(1, &m_id);
	delete[] m_textures;
	delete[] m_buffers;
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
}

void RenderTarget2D::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

END_XD_NAMESPACE