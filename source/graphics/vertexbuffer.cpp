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

VertexBuffer::VertexBuffer() :
	m_format(VertexFormat::s_vct),
	m_vertexData(0),
	m_vertexCount(0),
	m_indexData(0),
	m_indexCount(0),
	m_bufferType(RAW_BUFFER),
	m_vbo(0)
{
	m_vertexSize = m_format.getVertexSizeInBytes();
}

VertexBuffer::VertexBuffer(const VertexFormat &fmt) :
	m_format(fmt),
	m_vertexData(0),
	m_vertexCount(0),
	m_indexData(0),
	m_indexCount(0),
	m_bufferType(RAW_BUFFER),
	m_vbo(0)
{
	m_vertexSize = m_format.getVertexSizeInBytes();
}

VertexBuffer::VertexBuffer(const VertexBuffer &other) :
	m_format(VertexFormat::s_vct),
	m_bufferType(RAW_BUFFER),
	m_vbo(0)
{
	// Set variables
	m_format = other.m_format;

	// Copy index data
	m_indexCount = other.m_indexCount;
	m_indexData = new uint[m_indexCount];
	memcpy(m_indexData, other.m_indexData, m_indexCount * sizeof(uint));

	// Copy vertex data
	m_vertexCount = other.m_vertexCount;
	m_vertexSize = other.m_vertexSize;
	m_vertexData = new char[m_vertexCount * m_vertexSize];
	memcpy(m_vertexData, other.m_vertexData, m_vertexCount * m_vertexSize);

	// This will create our vbo
	setBufferType(other.m_bufferType);
}

VertexBuffer::~VertexBuffer()
{
	clear();
}

void VertexBuffer::addVertices(Vertex *vertices, int vcount, uint *indices, int icount)
{
	if(m_vbo)
	{
		LOG("Batch::addVertices(): Cannot add vertices to a static batch.");
		return;
	}
	
	if(vertices->getFormat() == m_format)
	{
		// Store index offset
		int ioffset = m_vertexCount;

		// Append vertex data
		char *newVertexData = new char[(m_vertexCount + vcount) * m_vertexSize];
		memcpy(newVertexData, m_vertexData, m_vertexCount * m_vertexSize);
		for(int i = 0; i < vcount; i++)
		{
			vertices[i].getData(newVertexData + (m_vertexCount + i) * m_vertexSize);
		}
		m_vertexCount += vcount;
		delete[] m_vertexData;
		m_vertexData = newVertexData;
	
		// Append index data
		uint *newIndexData = new uint[m_indexCount + icount];
		memcpy(newIndexData, m_indexData, m_indexCount * sizeof(uint));
		for(int i = 0; i < icount; i++)
		{
			newIndexData[m_indexCount + i] = indices[i] + ioffset;
		}
		m_indexCount += icount;
		delete[] m_indexData;
		m_indexData = newIndexData;
	}
	else
	{
		LOG("void VertexBuffer::addVertices(): Vertex format mismatch.");
	}
}

void VertexBuffer::modifyVertices(const int idx, Vertex *vertex, const int count)
{
	if(idx >= 0 && idx < m_vertexCount)
	{
		if(vertex->getFormat() == m_format)
		{
			// Modify vertex data
			for(int i = 0; i < count; i++)
			{
				vertex[i].getData(m_vertexData + (idx + i) * m_vertexSize);
			}

			// Upload to VBO
			if(m_vbo)
			{
				m_vbo->update(idx, m_vertexData + idx * m_vertexSize, count);
			}
		}
		else
		{
			LOG("void VertexBuffer::modifyVertices(): Vertex format mismatch.");
		}
	}
	else
	{
		LOG("void VertexBuffer::modifyVertices(): Index out-of-bounds.");
	}
}

VertexFormat VertexBuffer::getVertexFormat() const
{
	return m_format;
}

Vertex *VertexBuffer::getVertices(const int idx, const int count) const
{
	Vertex *vertices = 0;
	if(idx >= 0 && idx + count <= m_vertexCount)
	{
		vertices = new Vertex[count];
		for(int i = 0; i < count; i++)
		{
			vertices[i].setFormat(m_format);
			vertices[i].setData(m_vertexData + (idx + i) * m_vertexSize);
		}
	}
	else
	{
		LOG("Vertex VertexBuffer::getVertices(): Index out-of-bounds.");
	}
	return vertices;
}

char *VertexBuffer::getVertexData() const
{
	return m_vertexData;
}

int VertexBuffer::getVertexCount() const
{
	return m_vertexCount;
}

uint *VertexBuffer::getIndexData() const
{
	return m_indexData;
}

int VertexBuffer::getIndexCount() const
{
	return m_indexCount;
}

void VertexBuffer::clear()
{
	// Clear vertex data
	delete[] m_vertexData;
	m_vertexData = 0;
	m_vertexCount = 0;

	// Clear index data
	delete[] m_indexData;
	m_indexData = 0;
	m_indexCount = 0;

	// Delete vbo
	if(m_vbo)
	{
		delete m_vbo;
		m_vbo = 0;
	}
	m_bufferType = RAW_BUFFER;
}

void VertexBuffer::setBufferType(const BufferType type)
{
	if(type != m_bufferType)
	{
		delete m_vbo;
		m_vbo = 0;
	}
	m_bufferType = type;
	if(m_bufferType != RAW_BUFFER && !m_vbo)
	{
		m_vbo = new VertexBufferObject(*this);
	}
}

VertexBuffer::BufferType VertexBuffer::getBufferType() const
{
	return m_bufferType;
}

VertexBuffer &VertexBuffer::operator=(const VertexBuffer &other)
{
	clear();

	// Set variables
	m_format = other.m_format;

	// Copy index data
	m_indexCount = other.m_indexCount;
	m_indexData = new uint[m_indexCount];
	memcpy(m_indexData, other.m_indexData, m_indexCount * sizeof(uint));

	// Copy vertex data
	m_vertexCount = other.m_vertexCount;
	m_vertexSize = other.m_vertexSize;
	m_vertexData = new char[m_vertexCount * m_vertexSize];
	memcpy(m_vertexData, other.m_vertexData, m_vertexCount * m_vertexSize);
	
	// This will create our VBO
	setBufferType(other.m_bufferType);

	return *this;
}

}