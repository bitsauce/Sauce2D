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

VertexBuffer::VertexBuffer(const BufferType type) :
	m_format(),
	m_type(type)
{
	glGenBuffers(1, &m_id);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void VertexBuffer::setData(const Vertex *vertices, const uint vertexCount)
{
	// Get vertex data
	m_format = vertices->getFormat();
	char *vertexData = new char[vertexCount * m_format.getVertexSizeInBytes()];
	for(uint i = 0; i < vertexCount; ++i)
	{
		vertices[i].getData(vertexData + i * m_format.getVertexSizeInBytes());
	}

	// Upload vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * m_format.getVertexSizeInBytes(), vertexData, m_type);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertexData;

	m_size = vertexCount;
}

VertexFormat VertexBuffer::getVertexFormat() const
{
	return m_format;
}

DynamicVertexBuffer::DynamicVertexBuffer() :
	VertexBuffer(DYNAMIC_BUFFER)
{
}

DynamicVertexBuffer::DynamicVertexBuffer(const Vertex *vertices, const uint vertexCount) :
	VertexBuffer(DYNAMIC_BUFFER)
{
	setData(vertices, vertexCount);
}

void DynamicVertexBuffer::modifyData(const uint startIdx, Vertex *vertices, const uint vertexCount)
{
	if(!(m_format == vertices->getFormat())) return;

	// Get vertex data
	char *vertexData = new char[vertexCount * m_format.getVertexSizeInBytes()];
	for(uint i = 0; i < vertexCount; ++i)
	{
		vertices[i].getData(vertexData + i * m_format.getVertexSizeInBytes());
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferSubData(GL_ARRAY_BUFFER, startIdx * getVertexFormat().getVertexSizeInBytes(), vertexCount * getVertexFormat().getVertexSizeInBytes(), vertexData);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertexData;
}

StaticVertexBuffer::StaticVertexBuffer() :
	VertexBuffer(STATIC_BUFFER)
{
}

StaticVertexBuffer::StaticVertexBuffer(const Vertex *vertices, const uint vertexCount) :
	VertexBuffer(STATIC_BUFFER)
{
	setData(vertices, vertexCount);
}


// -------------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(const BufferType type) :
	m_type(type)
{
	glGenBuffers(1, &m_id);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

void IndexBuffer::setData(const uint *indices, const uint indexCount)
{
	// Upload index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint), indices, m_type);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	m_size = indexCount;
}

DynamicIndexBuffer::DynamicIndexBuffer() :
	IndexBuffer(DYNAMIC_BUFFER)
{
}

DynamicIndexBuffer::DynamicIndexBuffer(const uint *indices, const uint indexCount) :
	IndexBuffer(DYNAMIC_BUFFER)
{
	setData(indices, indexCount);
}

void DynamicIndexBuffer::modifyData(const uint startIdx, uint *indices, const uint indexCount)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, startIdx * sizeof(uint), indexCount * sizeof(uint), indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

StaticIndexBuffer::StaticIndexBuffer() :
	IndexBuffer(STATIC_BUFFER)
{
}

StaticIndexBuffer::StaticIndexBuffer(const uint *indices, const uint indexCount) :
	IndexBuffer(STATIC_BUFFER)
{
	setData(indices, indexCount);
}

}