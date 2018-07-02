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
#include <Sauce/Graphics/OpenGL/OpenGLTexture.h>

#include "..\3rdparty\SDL_image\SDL_image.h"

BEGIN_SAUCE_NAMESPACE

GLint toInternalFormat(PixelFormat::Components fmt, PixelFormat::DataType dt)
{
	switch(fmt)
	{
		case PixelFormat::R:
		{
			switch(dt)
			{
				case PixelFormat::INT: return GL_R32I;
				case PixelFormat::UNSIGNED_INT: return GL_R32UI;
				case PixelFormat::BYTE: return GL_R8I;
				case PixelFormat::UNSIGNED_BYTE: return GL_R8;
				case PixelFormat::FLOAT: return GL_R32F;
			}
		}
		break;
		case PixelFormat::RG:
		{
			switch(dt)
			{
				case PixelFormat::INT: return GL_RG32I;
				case PixelFormat::UNSIGNED_INT: return GL_RG32UI;
				case PixelFormat::BYTE: return GL_RG8I;
				case PixelFormat::UNSIGNED_BYTE: return GL_RG8;
				case PixelFormat::FLOAT: return GL_RG32F;
			}
		}
		break;
		case PixelFormat::RGB:
		{
			switch(dt)
			{
				case PixelFormat::INT: return GL_RGB32I;
				case PixelFormat::UNSIGNED_INT: return GL_RGB32UI;
				case PixelFormat::BYTE: return GL_RGB8I;
				case PixelFormat::UNSIGNED_BYTE: return GL_RGB8;
				case PixelFormat::FLOAT: return GL_RGB32F;
			}
		}
		break;
		case PixelFormat::RGBA:
		{
			switch(dt)
			{
				case PixelFormat::INT: return GL_RGBA32I;
				case PixelFormat::UNSIGNED_INT: return GL_RGBA32UI;
				case PixelFormat::BYTE: return GL_RGBA8I;
				case PixelFormat::UNSIGNED_BYTE: return GL_RGBA8;
				case PixelFormat::FLOAT: return GL_RGBA32F;
			}
		}
		break;
	}
	return 0;
}

GLint toFormat(PixelFormat::Components fmt, PixelFormat::DataType dt)
{
	switch(fmt)
	{
		case PixelFormat::R:
		{
			switch(dt)
			{
				case PixelFormat::UNSIGNED_BYTE: case PixelFormat::BYTE: case PixelFormat::FLOAT: return GL_RED;
				case PixelFormat::UNSIGNED_INT: case PixelFormat::INT: return GL_RED_INTEGER;
			}
		}
		break;
		case PixelFormat::RG:
		{
			switch(dt)
			{
				case PixelFormat::UNSIGNED_BYTE: case PixelFormat::BYTE: case PixelFormat::FLOAT: return GL_RG;
				case PixelFormat::UNSIGNED_INT: case PixelFormat::INT: return GL_RG_INTEGER;
			}
		}
		break;
		case PixelFormat::RGB:
		{
			switch(dt)
			{
				case PixelFormat::UNSIGNED_BYTE: case PixelFormat::BYTE: case PixelFormat::FLOAT: return GL_RGB;
				case PixelFormat::UNSIGNED_INT: case PixelFormat::INT: return GL_RGB_INTEGER;
			}
		}
		break;
		case PixelFormat::RGBA:
		{
			switch(dt)
			{
				case PixelFormat::UNSIGNED_BYTE: case PixelFormat::BYTE: case PixelFormat::FLOAT: return GL_RGBA;
				case PixelFormat::UNSIGNED_INT: case PixelFormat::INT: return GL_RGBA_INTEGER;
			}
		}
		break;
	}
	return 0;
}

GLint toGLDataType(PixelFormat::DataType dt)
{
	switch(dt)
	{
		case PixelFormat::INT: return GL_INT;
		case PixelFormat::UNSIGNED_INT: return GL_UNSIGNED_INT;
		case PixelFormat::BYTE: return GL_BYTE;
		case PixelFormat::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
		case PixelFormat::FLOAT: return GL_FLOAT;
	}
	return 0;
}

OpenGLTexture2D::OpenGLTexture2D(const Pixmap &pixmap)
{
	initialize(pixmap);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_id);
}

void OpenGLTexture2D::initialize(const Pixmap &pixmap)
{
	// Create an empty texture
	glGenTextures(1, &m_id);

	// Set default values
	m_filter = NEAREST; // Prefs::GetDefaultFilterMode();
	m_wrapping = CLAMP_TO_BORDER;
	m_mipmaps = false; // Prefs::UseMipmaps()
	m_pixelFormat = pixmap.getFormat();

	// Update pixmap
	updatePixmap(pixmap);
}

Pixmap OpenGLTexture2D::getPixmap() const
{
	// Get texture data
	uchar *data = new uchar[m_width * m_height * m_pixelFormat.getPixelSizeInBytes()];
	glBindTexture(GL_TEXTURE_2D, m_id);
	glGetTexImage(GL_TEXTURE_2D, 0, toFormat(m_pixelFormat.getComponents(), m_pixelFormat.getDataType()), toGLDataType(m_pixelFormat.getDataType()), (GLvoid*) data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Copy data to pixmap
	Pixmap pixmap(m_width, m_height, data, m_pixelFormat);
	delete[] data;
	return pixmap;
}

void OpenGLTexture2D::updatePixmap(const Pixmap &pixmap)
{
	// Store dimensions
	m_width = pixmap.getWidth();
	m_height = pixmap.getHeight();

	// Set default filtering
	glBindTexture(GL_TEXTURE_2D, m_id);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixmap.getData());
	glTexImage2D(GL_TEXTURE_2D, 0, toInternalFormat(pixmap.getFormat().getComponents(), pixmap.getFormat().getDataType()), (GLsizei) m_width, (GLsizei) m_height, 0, toFormat(pixmap.getFormat().getComponents(), pixmap.getFormat().getDataType()), toGLDataType(pixmap.getFormat().getDataType()), (const GLvoid*) pixmap.getData());
	glBindTexture(GL_TEXTURE_2D, 0);

	// Regenerate mipmaps
	m_mipmapsGenerated = false;

	// NOTE: There is a litte redundancy with the glBindTexture() calls.
	// Use default filtering options
	updateFiltering();
}

void OpenGLTexture2D::updatePixmap(const uint x, const uint y, const Pixmap &pixmap)
{
	if(x >= m_width || y >= m_height)
	{
		LOG("OpenGLTexture2D::updatePixmap(): Position out of texture bounds.");
		return;
	}

	// Set default filtering
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint) x, (GLint) y, (GLsizei) pixmap.getWidth(), (GLsizei) pixmap.getHeight(), toFormat(pixmap.getFormat().getComponents(), pixmap.getFormat().getDataType()), toGLDataType(pixmap.getFormat().getDataType()), (const GLvoid*) pixmap.getData());
	glBindTexture(GL_TEXTURE_2D, 0);

	// Regenerate mipmaps
	m_mipmapsGenerated = false;

	// NOTE: There is a litte redunacny with the glBindTexture() calls.
	// Use default filtering options
	updateFiltering();
}

void OpenGLTexture2D::clear()
{
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, vector<GLubyte>(m_width*m_height * 4, 0).data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTexture2D::updateFiltering()
{
	if(m_mipmaps && !m_mipmapsGenerated)
	{
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

END_SAUCE_NAMESPACE
