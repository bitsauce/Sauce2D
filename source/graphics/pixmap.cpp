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

Pixmap::Pixmap() :
	m_width(0),
	m_height(0),
	m_data(0)
{
}

Pixmap::Pixmap(const uint width, const uint height, const uchar *data) :
	m_width(width),
	m_height(height)
{
	// Copy pixels
	if(width >= 0 && height >= 0)
	{
		m_data = new uchar[width*height*4];
		memcpy(m_data, data, width*height*4);
	}
	else
	{
		m_data = 0;
	}
}

Pixmap::Pixmap(const uint width, const uint height) :
	m_width(width),
	m_height(height)
{
	// Create empty pixmap
	if(width >= 0 && height >= 0)
	{
		m_data = new uchar[width*height*4];
		memset(m_data, 0, width*height*4);
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
	if(other.m_data)
	{
		m_data = new uchar[m_width*m_height*4];
		memcpy(m_data, other.m_data, m_width*m_height*4);
	}
	else
	{
		m_data = 0;
	}
}

/*Pixmap &Pixmap::operator=(Pixmap &other)
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

Color Pixmap::getColor(const uint x, const uint y) const
{
	Color color;
	if(x < m_width && y < m_height)
	{
		memcpy(&color, m_data + (x + y*m_width) * 4, 4);
	}
	else
	{
		//warn("");
	}
	return color;
}

void Pixmap::setColor(const uint x, const uint y, const Color &color)
{
	if(x < m_width && y < m_height)
	{
		memcpy(m_data + (x + y*m_width) * 4, &color, 4);
	}
	else
	{
		//warn("");
	}
}

void Pixmap::fill(const Color &color)
{
	for(uint y = 0; y < m_height; ++y)
	{
		for(uint x = 0; x < m_width; ++x)
		{
			memcpy(m_data + (x + y*m_width) * 4, &color, 4);
		}
	}
}

void Pixmap::clear()
{
	fill(Color(0));
}

void Pixmap::exportToFile(const string &path) const
{
	// TODO: REFACTORING
	//XAssetManager::SavePixmap(path, (uchar*)m_data, m_width, m_height);
}

}