//     _____                        ______             _            
//    / ____|                      |  ____|           (_)           
//   | (___   __ _ _   _  ___ ___  | |__   _ __   __ _ _ _ __   ___ 
//    \___ \ / _` | | | |/ __/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \
//    ____) | (_| | |_| | (_|  __/ | |____| | | | (_| | | | | |  __/
//   |_____/ \__,_|\__,_|\___\___| |______|_| |_|\__, |_|_| |_|\___|
//                                                __/ |             
//                                               |___/              
// Made by Marcus "Bitsauce" Loo Vergara
// 2011-2018 (C)

#include <Sauce/Common.h>
#include <Sauce/Graphics.h>
#include <Sauce/Graphics/OpenGL/OpenGLRenderTarget.h>
#include <Sauce/Graphics/OpenGL/OpenGLTexture.h>

BEGIN_SAUCE_NAMESPACE

OpenGLRenderTarget2D::OpenGLRenderTarget2D(GraphicsContext *graphicsContext, const uint width, const uint height, const uint count, const PixelFormat &fmt) :
	RenderTarget2D(graphicsContext, width, height, count, fmt)
{
	// Generate framebuffer
	glGenFramebuffers(1, &m_id);

	// Allocate textures
	m_buffers = new GLenum[m_textureCount];
	for(uint i = 0; i < m_textureCount; ++i)
	{
		m_buffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
}

OpenGLRenderTarget2D::OpenGLRenderTarget2D(GraphicsContext *graphicsContext, shared_ptr<Texture2D> target) :
	RenderTarget2D(graphicsContext, target)
{
	// Generate framebuffer
	glGenFramebuffers(1, &m_id);

	// Set texture variables
	(m_buffers = new GLenum[1])[0] = GL_COLOR_ATTACHMENT0;
}

OpenGLRenderTarget2D::~OpenGLRenderTarget2D()
{
	glDeleteFramebuffers(1, &m_id);
	delete[] m_textures;
	delete[] m_buffers;
}

void OpenGLRenderTarget2D::bind()
{
	// Bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	for(uint i = 0; i < m_textureCount; ++i)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, dynamic_cast<OpenGLTexture2D*>(m_textures[i].get())->getID(), 0);
	}
	glDrawBuffers(m_textureCount, m_buffers);
}

void OpenGLRenderTarget2D::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

END_SAUCE_NAMESPACE
