//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "gltexture.h"

/*
*/

GLint enumToGL(const XTextureFilter filter)
{
	switch(filter)
	{
	case xdNearest: return GL_NEAREST;
	case xdLinear: return GL_LINEAR;
	}
	return 0;
}

XTextureFilter enumFromGL(const GLint filter)
{
	switch(filter)
	{
	case GL_NEAREST: return xdNearest;
	case GL_LINEAR: return xdLinear;
	}
	return XTextureFilter(0);
}

GLint wrapToGL(const XTextureWrapping wrapping)
{
	switch(wrapping)
	{
	case CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
	case CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
	case REPEAT: return GL_REPEAT;
	case MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
	}
	return GL_CLAMP_TO_BORDER;
}

XTextureWrapping wrapFromGL(const GLint wrapping)
{
	switch(wrapping)
	{
	case GL_CLAMP_TO_BORDER: return CLAMP_TO_BORDER;
	case GL_CLAMP_TO_EDGE: return CLAMP_TO_EDGE;
	case GL_REPEAT: return REPEAT;
	case GL_MIRRORED_REPEAT: return MIRRORED_REPEAT;
	}
	return CLAMP_TO_BORDER;
}

GLtexture::GLtexture(const XPixmap &pixmap)
{
	// Create an empty texture
	glGenTextures(1, &m_id);

	// Set default values
	m_filter = GL_NEAREST; // Prefs::GetDefaultFilterMode();
	m_wrapping = GL_CLAMP_TO_BORDER;
	m_mipmaps = false; // Prefs::UseMipmaps()

	// Update pixmap
	updatePixmap(pixmap);
}

GLtexture::~GLtexture()
{
	glDeleteTextures(1, &m_id);
}

void GLtexture::enableMipmaps()
{
	if(!m_mipmaps) {
		m_mipmaps = true;
		updateFiltering();
	}else{
		//warn("Mipmapping already enabled");
	}
}

void GLtexture::disableMipmaps()
{
	if(m_mipmaps) {
		m_mipmaps = false;
		updateFiltering();
	}else{
		//warn("Mipmapping already disabled");
	}
}

void GLtexture::setFiltering(const XTextureFilter filter)
{
	GLint glfilter = enumToGL(filter);
	if(m_filter != glfilter) {
		m_filter = glfilter;
		updateFiltering();
	}else{
		//warn("Texture filtering was not changed.");
	}
}

XTextureFilter GLtexture::getFiltering() const
{
	return enumFromGL(m_filter);
}

void GLtexture::setWrapping(const XTextureWrapping wrapping)
{
	GLint glwrapping = wrapToGL(wrapping);
	if(m_wrapping != glwrapping)
	{
		m_wrapping = glwrapping;
		updateFiltering();
	}
}

XTextureWrapping GLtexture::getWrapping() const
{
	return wrapFromGL(m_wrapping);
}

XPixmap GLtexture::getPixmap() const
{
	// Get texture data
	Vector4 *data = new Vector4[m_width*m_height];
	glBindTexture(GL_TEXTURE_2D, m_id);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (GLvoid*)data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Copy data to pixmap
	XPixmap pixmap(m_width, m_height, data);
	delete[] data;
	return pixmap;
}

void GLtexture::updatePixmap(const XPixmap &pixmap)
{
	// Store dimentions
	m_width = pixmap.getWidth();
	m_height = pixmap.getHeight();

	// Set default filtering
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)m_width, (GLsizei)m_height, 0, GL_RGBA, GL_FLOAT, (const GLvoid*)pixmap.getData());
	glBindTexture(GL_TEXTURE_2D, 0);

	// Regenerate mipmaps
	m_mipmapsGenerated = false;
	
	// NOTE: There is a litte redundancy with the glBindTexture() calls.
	// Use default filtering options
	updateFiltering();
}

void GLtexture::updatePixmap(const int x, const int y, const XPixmap &pixmap)
{
	// Set default filtering
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)x, (GLint)y, (GLsizei)pixmap.getWidth(), (GLsizei)pixmap.getHeight(), GL_RGBA, GL_FLOAT, (const GLvoid*)pixmap.getData());
	glBindTexture(GL_TEXTURE_2D, 0);

	// Regenerate mipmaps
	m_mipmapsGenerated = false;
	
	// NOTE: There is a litte redunacny with the glBindTexture() calls.
	// Use default filtering options
	updateFiltering();
}

void GLtexture::clear()
{
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, vector<GLubyte>(m_width*m_height*4, 0).data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLtexture::updateFiltering()
{
	if(m_mipmaps && !m_mipmapsGenerated) {
		glGenerateMipmap(GL_TEXTURE_2D);
		m_mipmapsGenerated = true;
	}
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_mipmaps ? (m_filter == GL_NEAREST ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_LINEAR) : m_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapping);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int GLtexture::getWidth() const
{
	return (int)m_width;
}

int GLtexture::getHeight() const
{
	return (int)m_height;
}