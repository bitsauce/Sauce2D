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

GLint enumToGL(const xdTextureFilter filter)
{
	switch(filter)
	{
	case xdNearest: return GL_NEAREST;
	case xdLinear: return GL_LINEAR;
	}
	return 0;
}

GLtexture::GLtexture(const Pixmap &pixmap)
{
	// Create an empty texture
	glGenTextures(1, &m_id);

	// Set default values
	m_filter = GL_NEAREST; // Prefs::GetDefaultFilterMode();
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

void GLtexture::setFiltering(const xdTextureFilter filter)
{
	GLint glfilter = enumToGL(filter);
	if(m_filter != glfilter) {
		m_filter = glfilter;
		updateFiltering();
	}else{
		//warn("Texture filtering was not changed.");
	}
}

Pixmap GLtexture::getPixmap() const
{
	// Get texture data
	Vector4 *data = new Vector4[m_width*m_height];
	glBindTexture(GL_TEXTURE_2D, m_id);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (GLvoid*)data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Copy data to pixmap
	Pixmap pixmap(m_width, m_height, data);
	delete[] data;
	return pixmap;
}

void GLtexture::updatePixmap(const Pixmap &pixmap)
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
	
	// NOTE: There is a litte redunacny with the glBindTexture() calls.
	// Use default filtering options
	updateFiltering();
}

void GLtexture::updatePixmap(const int x, const int y, const Pixmap &pixmap)
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