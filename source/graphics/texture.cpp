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

Texture2D::Texture2D(const PixelFormat &format)
{
	init(Pixmap(format));
}
	
Texture2D::Texture2D(const uint width, const uint height, const void *data, const PixelFormat &format)
{
	Pixmap pixmap(width, height, format);
	if (data == 0)
	{
		uchar *tmp = new uchar[format.getPixelSizeInBytes()];
		memset(tmp, 0, format.getPixelSizeInBytes());
		pixmap.fill(tmp);
		delete[] tmp;
	}
	else
	{
		pixmap.fill(data);
	}
	init(pixmap);
}

Texture2D::Texture2D(const Pixmap &pixmap)
{
	init(pixmap);
}
	
Texture2D::Texture2D(const Texture2D &texture)
{
	init(texture.getPixmap());
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_id);
}

void Texture2D::init(const Pixmap &pixmap)
{
	// Create an empty texture
	glGenTextures(1, &m_id);

	// Set default values
	m_filter = GL_NEAREST; // Prefs::GetDefaultFilterMode();
	m_wrapping = GL_CLAMP_TO_BORDER;
	m_mipmaps = false; // Prefs::UseMipmaps()
	m_pixelFormat = pixmap.getFormat();

	// Update pixmap
	updatePixmap(pixmap);
}

void Texture2D::enableMipmaps()
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

void Texture2D::disableMipmaps()
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

void Texture2D::setFiltering(const TextureFilter filter)
{
	if(m_filter != filter)
	{
		m_filter = filter;
		updateFiltering();
	}
	else
	{
		//warn("Texture filtering was not changed.");
	}
}

Texture2D::TextureFilter Texture2D::getFiltering() const
{
	return TextureFilter(m_filter);
}

void Texture2D::setWrapping(const TextureWrapping wrapping)
{
	if(m_wrapping != wrapping)
	{
		m_wrapping = wrapping;
		updateFiltering();
	}
}

Texture2D::TextureWrapping Texture2D::getWrapping() const
{
	return TextureWrapping(m_wrapping);
}

Pixmap Texture2D::getPixmap() const
{
	// Get texture data
	uchar *data = new uchar[m_width * m_height * m_pixelFormat.getPixelSizeInBytes()];
	glBindTexture(GL_TEXTURE_2D, m_id);
	glGetTexImage(GL_TEXTURE_2D, 0, toFormat(m_pixelFormat.getComponents(), m_pixelFormat.getDataType()), toGLDataType(m_pixelFormat.getDataType()), (GLvoid*)data);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Copy data to pixmap
	Pixmap pixmap(m_width, m_height, data, m_pixelFormat);
	delete[] data;
	return pixmap;
}

void Texture2D::updatePixmap(const Pixmap &pixmap)
{
	// Store dimentions
	m_width = pixmap.getWidth();
	m_height = pixmap.getHeight();

	// Set default filtering
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexImage2D(GL_TEXTURE_2D, 0, toInternalFormat(pixmap.getFormat().getComponents(), pixmap.getFormat().getDataType()), (GLsizei)m_width, (GLsizei)m_height, 0, toFormat(pixmap.getFormat().getComponents(), pixmap.getFormat().getDataType()), toGLDataType(pixmap.getFormat().getDataType()), (const GLvoid*)pixmap.getData());
	glBindTexture(GL_TEXTURE_2D, 0);

	GLenum error;
	if((error = glGetError()) != GL_NO_ERROR)
	{
		LOG("GL error: %i", error);
	}

	// Regenerate mipmaps
	m_mipmapsGenerated = false;
	
	// NOTE: There is a litte redundancy with the glBindTexture() calls.
	// Use default filtering options
	updateFiltering();
}

void Texture2D::updatePixmap(const int x, const int y, const Pixmap &pixmap)
{
	// Set default filtering
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, (GLint)x, (GLint)y, (GLsizei)pixmap.getWidth(), (GLsizei)pixmap.getHeight(), toFormat(pixmap.getFormat().getComponents(), pixmap.getFormat().getDataType()), toGLDataType(pixmap.getFormat().getDataType()), (const GLvoid*)pixmap.getData());
	glBindTexture(GL_TEXTURE_2D, 0);

	// Regenerate mipmaps
	m_mipmapsGenerated = false;
	
	// NOTE: There is a litte redunacny with the glBindTexture() calls.
	// Use default filtering options
	updateFiltering();
}

void Texture2D::clear()
{
	glBindTexture(GL_TEXTURE_2D, m_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, vector<GLubyte>(m_width*m_height*4, 0).data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::updateFiltering()
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

uint Texture2D::getWidth() const
{
	return m_width;
}

uint Texture2D::getHeight() const
{
	return m_height;
}

#include <freeimage.h>

void Texture2D::exportToFile(string path)
{
	// NOTE TO SELF: If I ever decide to implement export for integer texture, glGetTexImage() expects GL_BGRA_INTEGER instead of GL_BGRA
	if (m_pixelFormat.getDataType() != PixelFormat::BYTE && m_pixelFormat.getDataType() != PixelFormat::UNSIGNED_BYTE)
	{
		LOG("Cannot export image with a pixel data type different from byte or unsigned byte");
		return;
	}

	// Get texture data
	uchar *data = new uchar[m_width * m_height * m_pixelFormat.getPixelSizeInBytes()];
	glBindTexture(GL_TEXTURE_2D, m_id);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)data);
	glBindTexture(GL_TEXTURE_2D, 0);

	FIBITMAP *bitmap = FreeImage_ConvertFromRawBits(data, m_width, m_height, m_width * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN, FI_RGBA_BLUE, false);
	util::toAbsoluteFilePath(path);
	FreeImage_Save(FIF_PNG, bitmap, path.c_str(), PNG_DEFAULT); // For now, let's just save everything as png

	delete[] data;
}

Texture2DPtr Texture2D::loadResource(const string &name)
{
	// Load texture from file
	Texture2D *texture = 0;

	// Split input
	vector<string> strings = util::splitString(name, "?");
	string filePath = strings[0];
	bool premultiply = false;
	for(uint i = 1; i < strings.size(); ++i)
	{
		if(strings[i] == "PremultiplyAlpha")
			premultiply = true;
	}

	// Load asset as a image
	string content;
	if(FileSystem::ReadFile(filePath, content))
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

		if(premultiply)
		{
			FreeImage_PreMultiplyWithAlpha(bitmap);
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
		texture = new Texture2D(Pixmap(width, height, pixels));
		
		// Close the memory stream
		FreeImage_Unload(bitmap);
		FreeImage_CloseMemory(hmem);
	}
	else
	{
		// Unable to read file
		LOG("Texture2DLoader::load() - Unable to read file '%s'", filePath.c_str());
	}
	return Texture2DPtr(texture);
}

}