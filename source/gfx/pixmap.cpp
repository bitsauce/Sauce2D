//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "pixmap.h"
#include "color.h"

AS_REGISTER_VALUE(Pixmap)

int Pixmap::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;

	AS_REGISTER_CONSTRUCTOR(Pixmap, "const int, const int", (const int, const int, Pixmap*))
	AS_REGISTER_CONSTRUCTOR(Pixmap, "const int, const int, const array<Color> &in", (const int, const int, const Color*, Pixmap*))
	AS_REGISTER_CONSTRUCTOR(Pixmap, "Pixmap &in", (const Pixmap&, Pixmap*))

	return r;
}

Pixmap::Pixmap(const int width, const int height, const Color *pixels) :
	m_width(width),
	m_height(height)
{
	if (width >= 0 && height >= 0) {
		// Copy pixels
		m_data = new Color[width*height];
		memcpy(m_data, pixels, width*height*4);
	}else{
		//error("Pixmaps cannot have width or height less than 0!");
		return;
	}
}

Pixmap::Pixmap(const int width, const int height)
{
	if (width >= 0 && height >= 0) {
		// Create empty pixmap
		m_data = new Color[width*height];
	}else{
		//error("Pixmaps cannot have width or height less than 0!");
		return;
	}
}

Pixmap::~Pixmap()
{
	delete[] m_data;
}

const char *Pixmap::getData() const
{
	return (const char*)m_data;
}

int Pixmap::getWidth() const
{
	return m_width;
}

int Pixmap::getHeight() const
{
	return m_height;
}

Color Pixmap::getColor(const int x, const int y) const
{
	if(x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		return m_data[x+y*m_width];
	}else{
		//warn("");
	}
	return Color();
}

void Pixmap::setColor(const int x, const int y, const Color &color)
{
	if(x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		m_data[x+y*m_width] = color;
	}else{
		//warn("");
	}
}