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

/*********************************************************************
**	Vertex format													**
**********************************************************************/

VertexFormat::VertexFormat() :
	m_vertexByteSize(0)
{
}

VertexFormat::VertexFormat(const VertexFormat &other)
{
	for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
	{
		m_attributes[i] = other.m_attributes[i];
	}
	m_vertexByteSize = other.m_vertexByteSize;
}

void VertexFormat::set(const VertexAttribute attrib, const int size, const DataType dataType)
{
	if(size >= 0 && size <= 4)
	{
		m_attributes[attrib].elementCount = size;
		m_attributes[attrib].dataType = dataType;

		m_vertexByteSize = 0;
		for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
		{
			VertexAttribute at = VertexAttribute(i);
			if(isAttributeEnabled(at))
			{
				m_attributes[at].offset = m_vertexByteSize;
				switch(getDataType(at))
				{
				case CGF_FLOAT:
					m_vertexByteSize += sizeof(float)*getElementCount(at); break;
				case CGF_UINT:
				case CGF_INT:
					m_vertexByteSize += sizeof(int)*getElementCount(at); break;
				case CGF_USHORT:
				case CGF_SHORT:
					m_vertexByteSize += sizeof(short)*getElementCount(at); break;
				case CGF_UBYTE:
				case CGF_BYTE:
					m_vertexByteSize += sizeof(char)*getElementCount(at); break;
				}
			}
		}
	}
	else
	{
		LOG("VertexFormat::set(): Size must be in the range [0, 4].");
	}
}

int VertexFormat::getElementCount(const VertexAttribute attrib) const
{
	return m_attributes[attrib].elementCount;
}

DataType VertexFormat::getDataType(const VertexAttribute attrib) const
{
	return m_attributes[attrib].dataType;
}

uint VertexFormat::getVertexSizeInBytes() const
{
	return m_vertexByteSize;
}

uint VertexFormat::getAttributeOffset(const VertexAttribute attrib) const
{
	return m_attributes[attrib].offset;
}

bool VertexFormat::isAttributeEnabled(const VertexAttribute attrib) const
{
	return m_attributes[attrib].elementCount != 0;
}

Vertex *VertexFormat::createVertices(const int count) const
{
	Vertex *vertices = new Vertex[count];
	for(int i = 0; i < count; i++)
	{
		vertices[i].setFormat(*this);
	}
	return vertices;
}

VertexFormat &VertexFormat::operator=(const VertexFormat &other)
{
	for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
	{
		m_attributes[i] = other.m_attributes[i];
	}
	m_vertexByteSize = other.m_vertexByteSize;
	return *this;
}

bool VertexFormat::operator==(const VertexFormat &other)
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

Vertex::Vertex() :
	m_data(0),
	m_format(VertexFormat::s_vct)
{
	setFormat(m_format);
}

Vertex::Vertex(const Vertex &other) :
	m_data(0),
	m_format(other.m_format)
{
	m_data = new char[m_format.getVertexSizeInBytes()];
	memcpy(m_data, other.m_data, m_format.getVertexSizeInBytes());
}

Vertex::Vertex(const VertexFormat &fmt) :
	m_data(0),
	m_format(fmt)
{
	setFormat(fmt);
}

Vertex::~Vertex()
{
	delete[] m_data;
}

void Vertex::setFormat(const VertexFormat &fmt)
{
	delete[] m_data;
	m_data = new char[m_format.getVertexSizeInBytes()];
	memset(m_data, 0, m_format.getVertexSizeInBytes());
	m_format = fmt;
}

VertexFormat Vertex::getFormat() const
{
	return m_format;
}

void Vertex::set4f(const VertexAttribute attrib, const float v0, const float v1, const float v2, const float v3)
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
		LOG("void Vertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void Vertex::set4ui(const VertexAttribute attrib, const uint v0, const uint v1, const uint v2, const uint v3)
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
		LOG("void Vertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void Vertex::set4i(const VertexAttribute attrib, const int v0, const int v1, const int v2, const int v3)
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
		LOG("void Vertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void Vertex::set4us(const VertexAttribute attrib, const ushort v0, const ushort v1, const ushort v2, const ushort v3)
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
		LOG("void Vertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void Vertex::set4s(const VertexAttribute attrib, const short v0, const short v1, const short v2, const short v3)
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
		LOG("void Vertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void Vertex::set4ub(const VertexAttribute attrib, const uchar v0, const uchar v1, const uchar v2, const uchar v3)
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
		LOG("void Vertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void Vertex::set4b(const VertexAttribute attrib, const char v0, const char v1, const char v2, const char v3)
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
		LOG("void Vertex::set(): Attribute not enabled with the current vertex format.");
	}
}

void Vertex::setData(const char *data) const
{
	memcpy(m_data, data, m_format.getVertexSizeInBytes());
}

void Vertex::getData(char *data) const
{
	memcpy(data, m_data, m_format.getVertexSizeInBytes());
}

Vertex &Vertex::operator=(const Vertex &other)
{
	m_format = other.m_format;

	delete[] m_data;
	m_data = new char[m_format.getVertexSizeInBytes()];
	memcpy(m_data, other.m_data, m_format.getVertexSizeInBytes());

	return *this;
}

void Vertex::print()
{
	for(int i = 0; i < VERTEX_ATTRIB_MAX; i++)
	{
		VertexAttribute attrib = VertexAttribute(i);
		if(m_format.isAttributeEnabled(attrib))
		{
			LOG("Attrib: %i", attrib);
			stringstream ss;
			for(int j = 0; j < m_format.getElementCount(attrib); j++)
			{
				switch(m_format.getDataType(attrib))
				{
				case CGF_FLOAT:
					ss << ((float*)(m_data + m_format.getAttributeOffset(attrib)))[j];
					break;
				case CGF_UINT:
				case CGF_INT:
					ss << ((int*)(m_data + m_format.getAttributeOffset(attrib)))[j];
					break;
				case CGF_USHORT:
				case CGF_SHORT:
					ss << ((short*)(m_data + m_format.getAttributeOffset(attrib)))[j];
					break;
				case CGF_UBYTE:
				case CGF_BYTE:
					ss << (int)((char*)(m_data + m_format.getAttributeOffset(attrib)))[j];
					break;
				}
				ss << " ";
			}
			LOG("%s", ss.str().c_str());
		}
	}
}

END_CGF_NAMESPACE
