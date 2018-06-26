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
#include <Sauce/graphics.h>

BEGIN_SAUCE_NAMESPACE

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
				case SAUCE_FLOAT:
					m_vertexByteSize += sizeof(float)*getElementCount(at); break;
				case SAUCE_UINT:
				case SAUCE_INT:
					m_vertexByteSize += sizeof(int)*getElementCount(at); break;
				case SAUCE_USHORT:
				case SAUCE_SHORT:
					m_vertexByteSize += sizeof(short)*getElementCount(at); break;
				case SAUCE_UBYTE:
				case SAUCE_BYTE:
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

void Vertex::set1f(const VertexAttribute attrib, const float v0)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 1)
	{
		((float*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set1ui(const VertexAttribute attrib, const uint v0)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 1)
	{
		((uint*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set1i(const VertexAttribute attrib, const int v0)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 1)
	{
		((int*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set1us(const VertexAttribute attrib, const ushort v0)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 1)
	{
		((ushort*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set1s(const VertexAttribute attrib, const short v0)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 1)
	{
		((short*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set1ub(const VertexAttribute attrib, const uchar v0)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 1)
	{
		((uchar*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set1b(const VertexAttribute attrib, const char v0)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 1)
	{
		((char*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set2f(const VertexAttribute attrib, const float v0, const float v1)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 2)
	{
		((float*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((float*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set2ui(const VertexAttribute attrib, const uint v0, const uint v1)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 2)
	{
		((uint*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((uint*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set2i(const VertexAttribute attrib, const int v0, const int v1)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 2)
	{
		((int*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((int*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set2us(const VertexAttribute attrib, const ushort v0, const ushort v1)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 2)
	{
		((ushort*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((ushort*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set2s(const VertexAttribute attrib, const short v0, const short v1)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 2)
	{
		((short*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((short*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set2ub(const VertexAttribute attrib, const uchar v0, const uchar v1)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 2)
	{
		((uchar*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((uchar*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set2b(const VertexAttribute attrib, const char v0, const char v1)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 2)
	{
		((char*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((char*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set3f(const VertexAttribute attrib, const float v0, const float v1, const float v2)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 3)
	{
		((float*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((float*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((float*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set3ui(const VertexAttribute attrib, const uint v0, const uint v1, const uint v2)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 3)
	{
		((uint*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((uint*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((uint*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set3i(const VertexAttribute attrib, const int v0, const int v1, const int v2)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 3)
	{
		((int*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((int*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((int*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set3us(const VertexAttribute attrib, const ushort v0, const ushort v1, const ushort v2)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 3)
	{
		((ushort*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((ushort*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((ushort*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set3s(const VertexAttribute attrib, const short v0, const short v1, const short v2)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 3)
	{
		((short*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((short*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((short*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set3ub(const VertexAttribute attrib, const uchar v0, const uchar v1, const uchar v2)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 3)
	{
		((uchar*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((uchar*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((uchar*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set3b(const VertexAttribute attrib, const char v0, const char v1, const char v2)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 3)
	{
		((char*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((char*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((char*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set4f(const VertexAttribute attrib, const float v0, const float v1, const float v2, const float v3)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 4)
	{
		((float*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((float*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((float*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		((float*) (m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set4ui(const VertexAttribute attrib, const uint v0, const uint v1, const uint v2, const uint v3)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 4)
	{
		((uint*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((uint*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((uint*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		((uint*) (m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set4i(const VertexAttribute attrib, const int v0, const int v1, const int v2, const int v3)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 4)
	{
		((int*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((int*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((int*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		((int*) (m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set4us(const VertexAttribute attrib, const ushort v0, const ushort v1, const ushort v2, const ushort v3)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 4)
	{
		((ushort*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((ushort*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((ushort*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		((ushort*) (m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set4s(const VertexAttribute attrib, const short v0, const short v1, const short v2, const short v3)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 4)
	{
		((short*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((short*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((short*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		((short*) (m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set4ub(const VertexAttribute attrib, const uchar v0, const uchar v1, const uchar v2, const uchar v3)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 4)
	{
		((uchar*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((uchar*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((uchar*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		((uchar*) (m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
	}
	else
	{
		LOG("Vertex format does not match");
	}
}

void Vertex::set4b(const VertexAttribute attrib, const char v0, const char v1, const char v2, const char v3)
{
	if(m_format.isAttributeEnabled(attrib) || m_format.getElementCount(attrib) != 4)
	{
		((char*) (m_data + m_format.getAttributeOffset(attrib)))[0] = v0;
		((char*) (m_data + m_format.getAttributeOffset(attrib)))[1] = v1;
		((char*) (m_data + m_format.getAttributeOffset(attrib)))[2] = v2;
		((char*) (m_data + m_format.getAttributeOffset(attrib)))[3] = v3;
	}
	else
	{
		LOG("Vertex format does not match");
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
			stringstream ss;
			ss << "Attrib: ";
			switch(attrib)
			{
				case VERTEX_POSITION: ss << "Position: "; break;
				case VERTEX_COLOR: ss << "Color: "; break;
				case VERTEX_TEX_COORD: ss << "Tex Coord: "; break;
				case VERTEX_NORMAL: ss << "Normal: "; break;
			}

			for(int j = 0; j < m_format.getElementCount(attrib); j++)
			{
				switch(m_format.getDataType(attrib))
				{
				case SAUCE_FLOAT:
					ss << ((float*)(m_data + m_format.getAttributeOffset(attrib)))[j];
					break;
				case SAUCE_UINT:
				case SAUCE_INT:
					ss << ((int*)(m_data + m_format.getAttributeOffset(attrib)))[j];
					break;
				case SAUCE_USHORT:
				case SAUCE_SHORT:
					ss << ((short*)(m_data + m_format.getAttributeOffset(attrib)))[j];
					break;
				case SAUCE_UBYTE:
				case SAUCE_BYTE:
					ss << (int)((char*)(m_data + m_format.getAttributeOffset(attrib)))[j];
					break;
				}
				ss << " ";
			}
			LOG("%s", ss.str().c_str());
		}
	}
}

END_SAUCE_NAMESPACE
