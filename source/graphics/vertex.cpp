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

/*********************************************************************
**	Vertex format													**
**********************************************************************/

XVertexFormat::XVertexFormat() :
	m_vertexByteSize(0)
{
}

XVertexFormat::XVertexFormat(const XVertexFormat &other)
{
	for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
	{
		m_attributes[i] = other.m_attributes[i];
	}
	m_vertexByteSize = other.m_vertexByteSize;
}

void XVertexFormat::set(const XVertexAttribute attrib, const int size, const XDataType dataType)
{
	if(size >= 0 && size <= 4)
	{
		m_attributes[attrib].elementCount = size;
		m_attributes[attrib].dataType = dataType;

		m_vertexByteSize = 0;
		for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
		{
			XVertexAttribute at = XVertexAttribute(i);
			if(isAttributeEnabled(at))
			{
				m_attributes[at].offset = m_vertexByteSize;
				switch(getDataType(at))
				{
				case XD_FLOAT:
					m_vertexByteSize += sizeof(float)*getElementCount(at); break;
				case XD_UINT:
				case XD_INT:
					m_vertexByteSize += sizeof(int)*getElementCount(at); break;
				case XD_USHORT:
				case XD_SHORT:
					m_vertexByteSize += sizeof(short)*getElementCount(at); break;
				case XD_UBYTE:
				case XD_BYTE:
					m_vertexByteSize += sizeof(char)*getElementCount(at); break;
				}
			}
		}
	}
	else
	{
		LOG("XVertexFormat::set(): Size must be in the range [0, 4].");
	}
}

int XVertexFormat::getElementCount(const XVertexAttribute attrib) const
{
	return m_attributes[attrib].elementCount;
}

XDataType XVertexFormat::getDataType(const XVertexAttribute attrib) const
{
	return m_attributes[attrib].dataType;
}

uint XVertexFormat::getVertexSizeInBytes() const
{
	return m_vertexByteSize;
}

uint XVertexFormat::getAttributeOffset(const XVertexAttribute attrib) const
{
	return m_attributes[attrib].offset;
}

bool XVertexFormat::isAttributeEnabled(const XVertexAttribute attrib) const
{
	return m_attributes[attrib].elementCount != 0;
}

XVertex *XVertexFormat::createVertices(const int count) const
{
	XVertex *vertices = new XVertex[count];
	for(int i = 0; i < count; i++)
	{
		vertices[i].setFormat(*this);
	}
	return vertices;
}

XVertexFormat &XVertexFormat::operator=(const XVertexFormat &other)
{
	for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
	{
		m_attributes[i] = other.m_attributes[i];
	}
	m_vertexByteSize = other.m_vertexByteSize;
	return *this;
}

bool XVertexFormat::operator==(const XVertexFormat &other)
{
	for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
	{
		if(m_attributes[i] != other.m_attributes[i])
			return false;
	}
	return true;
}

/*********************************************************************
**	Vertex															**
**********************************************************************/

XVertex::XVertex() :
	m_data(0),
	m_format(XVertexFormat::s_vct)
{
	setFormat(m_format);
}

XVertex::XVertex(const XVertex &other) :
	m_data(0),
	m_format(other.m_format)
{
	m_data = new char[m_format.getVertexSizeInBytes()];
	memcpy(m_data, other.m_data, m_format.getVertexSizeInBytes());
}

XVertex::XVertex(const XVertexFormat &fmt) :
	m_data(0),
	m_format(fmt)
{
	setFormat(fmt);
}

XVertex::~XVertex()
{
	delete[] m_data;
}

void XVertex::setFormat(const XVertexFormat &fmt)
{
	delete[] m_data;
	m_data = new char[m_format.getVertexSizeInBytes()];
	memset(m_data, 0, m_format.getVertexSizeInBytes());
	m_format = fmt;
}

XVertexFormat XVertex::getFormat() const
{
	return m_format;
}

void XVertex::set4f(const XVertexAttribute attrib, const float v0, const float v1, const float v2, const float v3)
{
	if(m_format.isAttributeEnabled(attrib))
	{
		switch(m_format.getElementCount(attrib))
		{
		case 4: ((float*)(m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
		case 3: ((float*)(m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		case 2: ((float*)(m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		case 1: ((float*)(m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		}
	}
	else
	{
		LOG("void XVertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void XVertex::set4ui(const XVertexAttribute attrib, const uint v0, const uint v1, const uint v2, const uint v3)
{
	if(m_format.isAttributeEnabled(attrib))
	{
		switch(m_format.getElementCount(attrib))
		{
		case 4: ((uint*)(m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
		case 3: ((uint*)(m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		case 2: ((uint*)(m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		case 1: ((uint*)(m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		}
	}
	else
	{
		LOG("void XVertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void XVertex::set4i(const XVertexAttribute attrib, const int v0, const int v1, const int v2, const int v3)
{
	if(m_format.isAttributeEnabled(attrib))
	{
		switch(m_format.getElementCount(attrib))
		{
		case 4: ((int*)(m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
		case 3: ((int*)(m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		case 2: ((int*)(m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		case 1: ((int*)(m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		}
	}
	else
	{
		LOG("void XVertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void XVertex::set4us(const XVertexAttribute attrib, const ushort v0, const ushort v1, const ushort v2, const ushort v3)
{
	if(m_format.isAttributeEnabled(attrib))
	{
		switch(m_format.getElementCount(attrib))
		{
		case 4: ((ushort*)(m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
		case 3: ((ushort*)(m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		case 2: ((ushort*)(m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		case 1: ((ushort*)(m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		}
	}
	else
	{
		LOG("void XVertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void XVertex::set4s(const XVertexAttribute attrib, const short v0, const short v1, const short v2, const short v3)
{
	if(m_format.isAttributeEnabled(attrib))
	{
		switch(m_format.getElementCount(attrib))
		{
		case 4: ((short*)(m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
		case 3: ((short*)(m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		case 2: ((short*)(m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		case 1: ((short*)(m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		}
	}
	else
	{
		LOG("void XVertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void XVertex::set4ub(const XVertexAttribute attrib, const uchar v0, const uchar v1, const uchar v2, const uchar v3)
{
	if(m_format.isAttributeEnabled(attrib))
	{
		switch(m_format.getElementCount(attrib))
		{
		case 4: ((uchar*)(m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
		case 3: ((uchar*)(m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		case 2: ((uchar*)(m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		case 1: ((uchar*)(m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		}
	}
	else
	{
		LOG("void XVertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void XVertex::set4b(const XVertexAttribute attrib, const char v0, const char v1, const char v2, const char v3)
{
	if(m_format.isAttributeEnabled(attrib))
	{
		switch(m_format.getElementCount(attrib))
		{
		case 4: ((char*)(m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
		case 3: ((char*)(m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		case 2: ((char*)(m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		case 1: ((char*)(m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		}
	}
	else
	{
		LOG("void XVertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void XVertex::setData(const char *data) const
{
	memcpy(m_data, data, m_format.getVertexSizeInBytes());
}

void XVertex::getData(char *data) const
{
	memcpy(data, m_data, m_format.getVertexSizeInBytes());
}

XVertex &XVertex::operator=(const XVertex &other)
{
	m_format = other.m_format;

	delete[] m_data;
	m_data = new char[m_format.getVertexSizeInBytes()];
	memcpy(m_data, other.m_data, m_format.getVertexSizeInBytes());

	return *this;
}

void XVertex::print()
{
	for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
	{
		XVertexAttribute attrib = XVertexAttribute(i);
		if(m_format.isAttributeEnabled(attrib))
		{
			LOG("Attrib: %i", attrib);
			stringstream ss;
			for(int j = 0; j < m_format.getElementCount(attrib); j++)
			{
				switch(m_format.getDataType(attrib))
				{
				case XD_FLOAT:
					ss << ((float*)(m_data + m_format.getAttributeOffset(attrib)))[j];
					break;
				case XD_UINT:
				case XD_INT:
					ss << ((int*)(m_data + m_format.getAttributeOffset(attrib)))[j];
					break;
				case XD_USHORT:
				case XD_SHORT:
					ss << ((short*)(m_data + m_format.getAttributeOffset(attrib)))[j];
					break;
				case XD_UBYTE:
				case XD_BYTE:
					ss << (int)((char*)(m_data + m_format.getAttributeOffset(attrib)))[j];
					break;
				}
				ss << " ";
			}
			LOG("%s", ss.str().c_str());
		}
	}
}