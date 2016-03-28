//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <CGF/Common.h>
#include <CGF/graphics.h>

BEGIN_CGF_NAMESPACE

uint PixelFormat::getComponentCount() const
{
	switch (m_components) {
	case R: return 1;
	case RG: return 2;
	case RGB: return 3;
	case RGBA: return 4;
	}
	return 0;
}

uint PixelFormat::getDataTypeSizeInBytes() const
{
	switch (m_dataType)
	{
	case UNSIGNED_INT: case INT: return sizeof(GLint);
	case UNSIGNED_BYTE: case BYTE: return sizeof(GLbyte);
	case FLOAT: return sizeof(GLfloat);
	}
	return 0;
}

uint PixelFormat::getPixelSizeInBytes() const
{
	return getComponentCount() * getDataTypeSizeInBytes();
}

Pixmap::Pixmap(const PixelFormat &format) :
	m_width(0),
	m_height(0),
	m_data(0),
	m_format(format)
{
}

Pixmap::Pixmap(const uint width, const uint height, const void *data, const PixelFormat &format) :
	m_width(width),
	m_height(height),
	m_format(format)
{
	// Copy pixels
	if(width >= 0 && height >= 0)
	{
		m_data = new uchar[width * height * m_format.getPixelSizeInBytes()];
		memcpy(m_data, data, width * height * m_format.getPixelSizeInBytes());
	}
	else
	{
		m_data = 0;
	}
}

Pixmap::Pixmap(const uint width, const uint height, const PixelFormat &format) :
	m_width(width),
	m_height(height),
	m_format(format)
{
	// Create empty pixmap
	if(width >= 0 && height >= 0)
	{
		m_data = new uchar[width * height * m_format.getPixelSizeInBytes()];
		memset(m_data, 0, width * height * m_format.getPixelSizeInBytes());
	}
	else
	{
		m_data = 0;
	}
}

Pixmap::Pixmap(const Pixmap &other)
{
	m_width = other.m_width;
	m_height = other.m_height;
	m_format = other.m_format;
	if(other.m_data)
	{
		m_data = new uchar[m_width * m_height * m_format.getPixelSizeInBytes()];
		memcpy(m_data, other.m_data, m_width * m_height * m_format.getPixelSizeInBytes());
	}
	else
	{
		m_data = 0;
	}
}

Pixmap::Pixmap(const string &imageFile, const bool premultiplyAlpha) :
	m_format()
{
	// Load pixmap from image file
	SDL_Surface *surface = IMG_Load(imageFile.c_str());
	if(surface)
	{
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		if(surface->format->format != SDL_PIXELFORMAT_ABGR8888)
		{
			SDL_Surface *oldSurface = surface;
			surface = SDL_ConvertSurfaceFormat(oldSurface, SDL_PIXELFORMAT_ABGR8888, 0);
			SDL_FreeSurface(oldSurface);
		}
#else
		if(surface->format->format != SDL_PIXELFORMAT_RGBA8888)
		{
			SDL_Surface *oldSurface = surface;
			surface = SDL_ConvertSurfaceFormat(oldSurface, SDL_PIXELFORMAT_RGBA8888, 0);
			SDL_FreeSurface(oldSurface);
		}
#endif
		// Create pixmap data
		if(surface->w >= 0 && surface->h >= 0)
		{
			m_data = new uchar[surface->w * surface->h * m_format.getPixelSizeInBytes()];
		}
		else
		{
			m_data = 0;
		}

		// Fill pixmap data
		if(premultiplyAlpha)
		{
			for(uint i = 0; i < surface->w * surface->h; i++)
			{
				uchar alpha = m_data[i * 4 + 3] = ((uchar*) surface->pixels)[i * 4 + 3];
				m_data[i * 4 + 0] = ((uchar*) surface->pixels)[i * 4 + 0] * alpha / 255.0f;
				m_data[i * 4 + 1] = ((uchar*) surface->pixels)[i * 4 + 1] * alpha / 255.0f;
				m_data[i * 4 + 2] = ((uchar*) surface->pixels)[i * 4 + 2] * alpha / 255.0f;
			}
		}
		else
		{
			memcpy(m_data, surface->pixels, surface->w * surface->h * m_format.getPixelSizeInBytes());
		}

		// Set width and height
		m_width = surface->w;
		m_height = surface->h;

		// Free surface
		SDL_FreeSurface(surface);
	}
	else
	{
		// Set data, width and height to 0
		m_data = 0;
		m_width = m_height = 0;

		// Unable to read file
		LOG("Unable to load image '%s'\n%s", imageFile.c_str(), IMG_GetError());
	}
}

/*Pixmap::Pixmap &operator=(Pixmap &other)
{
	swap(m_data, other.m_data);
	swap(m_width, other.m_width);
	swap(m_height, other.m_height);
	return *this;
}*/

Pixmap::~Pixmap()
{
	delete[] m_data;
}

Pixmap &Pixmap::operator=(Pixmap &other)
{
	m_width = other.m_width;
	m_height = other.m_height;
	m_format = other.m_format;
	if(other.m_data)
	{
		m_data = new uchar[m_width * m_height * m_format.getPixelSizeInBytes()];
		memcpy(m_data, other.m_data, m_width * m_height * m_format.getPixelSizeInBytes());
	}
	else
	{
		m_data = 0;
	}
	return *this;
}

const uchar *Pixmap::getData() const
{
	return m_data;
}

uint Pixmap::getWidth() const
{
	return m_width;
}

uint Pixmap::getHeight() const
{
	return m_height;
}

PixelFormat Pixmap::getFormat() const
{
	return m_format;
}

void Pixmap::getPixel(const uint x, const uint y, void *data) const
{
	if(x < m_width && y < m_height)
	{
		memcpy(data, m_data + (x + y * m_width) * m_format.getPixelSizeInBytes(), m_format.getPixelSizeInBytes());
	}
}

void Pixmap::setPixel(const uint x, const uint y, const void *data)
{
	if(x < m_width && y < m_height)
	{
		memcpy(m_data + (x + y*m_width) * m_format.getPixelSizeInBytes(), data, m_format.getPixelSizeInBytes());
	}
}

void Pixmap::flipY()
{
	uchar *pixel0 = new uchar[m_format.getPixelSizeInBytes()];
	uchar *pixel1 = new uchar[m_format.getPixelSizeInBytes()];
	for(int y0 = 0, y1 = m_height - 1; y0 < m_height / 2; y0++, y1--)
	{
		for(int x = 0; x < m_width; ++x)
		{
			getPixel(x, y0, pixel0);
			getPixel(x, y1, pixel1);
			setPixel(x, y1, pixel0);
			setPixel(x, y0, pixel1);
		}
	}
	delete[] pixel0;
	delete[] pixel1;
}

void Pixmap::fill(const void *data)
{
	for(uint y = 0; y < m_height; ++y)
	{
		for(uint x = 0; x < m_width; ++x)
		{
			memcpy(m_data + (x + y*m_width) * m_format.getPixelSizeInBytes(), data, m_format.getPixelSizeInBytes());
		}
	}
}

void Pixmap::clear()
{
	uchar *emptyPixel = new uchar[m_format.getPixelSizeInBytes()];
	memset(emptyPixel, 0, m_format.getPixelSizeInBytes());
	fill(emptyPixel);
	delete[] emptyPixel;
}

void Pixmap::exportToFile(string path) const
{
	if(m_format.getDataType() != PixelFormat::BYTE && m_format.getDataType() != PixelFormat::UNSIGNED_BYTE)
	{
		LOG("Cannot export image with a pixel data type different from byte or unsigned byte");
		return;
	}

	// Convert pixmap to surface and export as a PNG image
	SDL_Surface *surface = SDL_CreateRGBSurfaceFrom(m_data, m_width, m_height, 32, m_width * 4, R_MASK, G_MASK, B_MASK, A_MASK);
	util::toAbsoluteFilePath(path);
	IMG_SavePNG(surface, path.c_str());
	SDL_FreeSurface(surface);
}

END_CGF_NAMESPACE
