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

GLint enumToGL(const XTexture::TextureFilter filter)
{
	switch(filter)
	{
	case XTexture::NEAREST: return GL_NEAREST;
	case XTexture::LINEAR: return GL_LINEAR;
	}
	return 0;
}

XTexture::TextureFilter enumFromGL(const GLint filter)
{
	switch(filter)
	{
	case GL_NEAREST: return XTexture::NEAREST;
	case GL_LINEAR: return XTexture::LINEAR;
	}
	return XTexture::TextureFilter(0);
}

GLint wrapToGL(const XTexture::TextureWrapping wrapping)
{
	switch(wrapping)
	{
	case XTexture::CLAMP_TO_BORDER: return GL_CLAMP_TO_BORDER;
	case XTexture::CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
	case XTexture::REPEAT: return GL_REPEAT;
	case XTexture::MIRRORED_REPEAT: return GL_MIRRORED_REPEAT;
	}
	return GL_CLAMP_TO_BORDER;
}

XTexture::TextureWrapping wrapFromGL(const GLint wrapping)
{
	switch(wrapping)
	{
	case GL_CLAMP_TO_BORDER: return XTexture::CLAMP_TO_BORDER;
	case GL_CLAMP_TO_EDGE: return XTexture::CLAMP_TO_EDGE;
	case GL_REPEAT: return XTexture::REPEAT;
	case GL_MIRRORED_REPEAT: return XTexture::MIRRORED_REPEAT;
	}
	return XTexture::CLAMP_TO_BORDER;
}

XTexture::XTexture(const XPixmap &pixmap)
{
	init(pixmap);
}
	
XTexture::XTexture(const uint width, const uint height, const XColor &color)
{
	XPixmap pixmap(width, height);
	pixmap.fill(color);
	init(pixmap);
}
	
XTexture::XTexture(const XTexture &texture)
{
	init(texture.getPixmap());
}

XTexture::~XTexture()
{
	glDeleteTextures(1, &m_id);
}

void XTexture::init(const XPixmap &pixmap)
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

void XTexture::enableMipmaps()
{
	if(!m_mipmaps)
	{
		m_mipmaps = true;
		updateFiltering();
	}
	else
	{
		//warn("Mipmapping already enabled");
	}
}

void XTexture::disableMipmaps()
{
	if(m_mipmaps)
	{
		m_mipmaps = false;
		updateFiltering();
	}
	else
	{
		//warn("Mipmapping already disabled");
	}
}

void XTexture::setFiltering(const TextureFilter filter)
{
	GLint glfilter = enumToGL(filter);
	if(m_filter != glfilter)
	{
		m_filter = glfilter;
		updateFiltering();
	}
	else
	{
		//warn("Texture filtering was not changed.");
	}
}

XTexture::TextureFilter XTexture::getFiltering() const
{
	return enumFromGL(m_filter);
}

void XTexture::setWrapping(const TextureWrapping wrapping)
{
	GLint glwrapping = wrapToGL(wrapping);
	if(m_wrapping != glwrapping)
	{
		m_wrapping = glwrapping;
		updateFiltering();
	}
}

XTexture::TextureWrapping XTexture::getWrapping() const
{
	return wrapFromGL(m_wrapping);
}

XPixmap XTexture::getPixmap() const
{
	// Get texture data
	uchar *data = new uchar[m_width*m_height*4];
	glBindTexture(GL_TEXTURE_2D, m_id);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Copy data to pixmap
	XPixmap pixmap(m_width, m_height, data);
	delete[] data;
	return pixmap;
}

void XTexture::updatePixmap(const XPixmap &pixmap)
{
	// Store dimentions
	m_width = pixmap.getWidth();
	m_height = pixmap.getHeight();

	// Set default filtering
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (GLsizei)m_width, (GLsizei)m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)pixmap.getData());
	glBindTexture(GL_TEXTURE_2D, 0);

	// Regenerate mipmaps
	m_mipmapsGenerated = false;
	
	// NOTE: There is a litte redundancy with the glBindTexture() calls.
	// Use default filtering options
	updateFiltering();
}

void XTexture::updatePixmap(const int x, const int y, const XPixmap &pixmap)
{
	// Set default filtering
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)x, (GLint)y, (GLsizei)pixmap.getWidth(), (GLsizei)pixmap.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)pixmap.getData());
	glBindTexture(GL_TEXTURE_2D, 0);

	// Regenerate mipmaps
	m_mipmapsGenerated = false;
	
	// NOTE: There is a litte redunacny with the glBindTexture() calls.
	// Use default filtering options
	updateFiltering();
}

void XTexture::clear()
{
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, vector<GLubyte>(m_width*m_height*4, 0).data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void XTexture::updateFiltering()
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

uint XTexture::getWidth() const
{
	return m_width;
}

uint XTexture::getHeight() const
{
	return m_height;
}

#include <freeimage.h>

void XTexture::exportToFile(string path)
{
	// Get texture data
	uchar *data = new uchar[m_width*m_height*4];
	glBindTexture(GL_TEXTURE_2D, m_id);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)data);
	glBindTexture(GL_TEXTURE_2D, 0);

	FIBITMAP *bitmap = FreeImage_ConvertFromRawBits(data, m_width, m_height, m_width * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN, FI_RGBA_BLUE, false);
	util::toAbsoluteFilePath(path);
	FreeImage_Save(FIF_PNG, bitmap, path.c_str(), PNG_DEFAULT); // For now, let's just save everything as png

	delete[] data;
}

shared_ptr<XTexture> XTexture::loadResource(const string &name)
{
	// Load texture from file
	XTexture *texture = 0;

	// Load asset as a image
	string content;
	if(XFileSystem::ReadFile(/*XResourceManager::getFileByName(name)*/name, content))
	{
		// Attach the binary data to a memory stream
		FIMEMORY *hmem = FreeImage_OpenMemory((uchar*)content.c_str(), content.size());
		
		// Get the file type
		FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem);
		
		// Load an image from the memory stream
		FIBITMAP *bitmap = FreeImage_LoadFromMemory(fif, hmem, 0);

		// Convert all non-32bpp bitmaps to 32bpp bitmaps
		// TODO: I should add support for loading different bpps into graphics memory
		if(FreeImage_GetBPP(bitmap) != 32)
		{
			FIBITMAP *newBitmap = FreeImage_ConvertTo32Bits(bitmap);
			FreeImage_Unload(bitmap);
			bitmap = newBitmap;
		}
			
		// Create pixmap
		uint width = FreeImage_GetWidth(bitmap), height = FreeImage_GetHeight(bitmap);
		BYTE *data = FreeImage_GetBits(bitmap);
		uchar *pixels = new uchar[width*height*4];
		for(uint i = 0; i < width*height; i++) // BGRA to RGBA
		{
			pixels[i*4+0] = data[i*4+2];
			pixels[i*4+1] = data[i*4+1];
			pixels[i*4+2] = data[i*4+0];
			pixels[i*4+3] = data[i*4+3];
		}
		texture = new XTexture(XPixmap(width, height, pixels));
		
		// Close the memory stream
		FreeImage_Unload(bitmap);
		FreeImage_CloseMemory(hmem);
	}
	else
	{
		// Unable to read file
		LOG("XTextureLoader::load() - Unable to read file '%s'", name);
	}
	return shared_ptr<XTexture>(texture);
}