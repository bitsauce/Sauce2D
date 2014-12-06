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

XFrameBufferObject::XFrameBufferObject()
{
	glGenFramebuffers(1, &m_id);
}

XFrameBufferObject::~XFrameBufferObject()
{
	glDeleteFramebuffers(1, &m_id);
}

void XFrameBufferObject::bind(shared_ptr<XTexture> texture)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->m_id, 0);

	XGraphics::getOrthoProjection(m_ortho[0], m_ortho[1], m_ortho[2], m_ortho[3], m_ortho[4], m_ortho[5]);
	XGraphics::getViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
		
	int w = texture->getWidth(), h = texture->getHeight();
	XGraphics::setOrthoProjection(0.0f, (float)w, (float)h, 0.0f, m_ortho[4], m_ortho[5]);
	XGraphics::setViewport(Recti(0, 0, w, h));
}

void XFrameBufferObject::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	XGraphics::setOrthoProjection(m_ortho[0], m_ortho[1], m_ortho[2], m_ortho[3], m_ortho[4], m_ortho[5]);
	XGraphics::setViewport(Recti(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]));
}