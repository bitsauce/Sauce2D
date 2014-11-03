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

AS_REG_POD(XPixmap, "Pixmap")

int XPixmap::Register(asIScriptEngine *scriptEngine)
{
	int r = 0;
	
	r = scriptEngine->RegisterObjectBehaviour("Pixmap", asBEHAVE_CONSTRUCT, "void f(Pixmap &in)", asFUNCTIONPR(Factory, (XPixmap&, XPixmap*), void), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Pixmap", asBEHAVE_CONSTRUCT, "void f(const int, const int)", asFUNCTIONPR(Factory, (const int, const int, XPixmap*), void), asCALL_CDECL_OBJLAST); AS_ASSERT
	r = scriptEngine->RegisterObjectBehaviour("Pixmap", asBEHAVE_CONSTRUCT, "void f(const int, const int, const array<Vector4> &in)", asFUNCTIONPR(Factory, (const int, const int, XScriptArray&, XPixmap*), void), asCALL_CDECL_OBJLAST); AS_ASSERT

	r = scriptEngine->RegisterObjectMethod("Pixmap", "void exportToFile(const string &in) const", asMETHOD(XPixmap, exportToFile), asCALL_THISCALL);

	return r;
}

XPixmap::XPixmap() :
	m_width(0),
	m_height(0),
	m_data(0)
{
}

XPixmap::XPixmap(const int width, const int height, const Vector4 *pixels) :
	m_width(width),
	m_height(height)
{
	// Copy pixels
	if(width >= 0 && height >= 0) {
		m_data = new Vector4[width*height];
		memcpy(m_data, pixels, width*height*sizeof(Vector4));
	}else{
		m_data = 0;
	}
}

XPixmap::XPixmap(const int width, const int height, const XScriptArray &pixels) :
	m_width(width),
	m_height(height)
{
	// Copy pixels
	if(width >= 0 && height >= 0) {
		m_data = new Vector4[width*height];
		int i = 0;
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				m_data[i] = *(Vector4*)pixels.At(i);
				i++;
			}
		}
	}else{
		m_data = 0;
	}
}

XPixmap::XPixmap(const int width, const int height) :
	m_width(width),
	m_height(height)
{
	// Create empty XPixmap
	if(width >= 0 && height >= 0) {
		m_data = new Vector4[width*height];
	}else{
		m_data = 0;
	}
}

XPixmap::XPixmap(const XPixmap &other)
{
	m_width = other.m_width;
	m_height = other.m_height;
	if(other.m_data)
	{
		m_data = new Vector4[m_width*m_height];
		memcpy(m_data, other.m_data, m_width*m_height*sizeof(Vector4));
	}else{
		m_data = 0;
	}
}

/*XPixmap &XPixmap::operator=(XPixmap &other)
{
	swap(m_data, other.m_data);
	swap(m_width, other.m_width);
	swap(m_height, other.m_height);
	return *this;
}*/

XPixmap::~XPixmap()
{
	delete[] m_data;
}

const float *XPixmap::getData() const
{
	return (const float*)m_data;
}

int XPixmap::getWidth() const
{
	return m_width;
}

int XPixmap::getHeight() const
{
	return m_height;
}

Vector4 XPixmap::getColor(const int x, const int y) const
{
	if(x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		return m_data[x+y*m_width];
	}else{
		//warn("");
	}
	return Vector4();
}

void XPixmap::setColor(const int x, const int y, const Vector4 &color)
{
	if(x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		m_data[x+y*m_width] = color;
	}else{
		//warn("");
	}
}

void XPixmap::exportToFile(const string &path) const
{
	// TODO: REFACTORING
	//XAssetManager::SavePixmap(path, (uchar*)m_data, m_width, m_height);
}