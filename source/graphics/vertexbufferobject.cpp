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

GLenum toGLBufferType(const VertexBuffer::BufferType value)
{
	switch(value) {
	case VertexBuffer::DYNAMIC_BUFFER:		return GL_DYNAMIC_DRAW;
	case VertexBuffer::STATIC_BUFFER:		return GL_STATIC_DRAW;
	}
	return GL_DYNAMIC_DRAW;
}

VertexBufferObject::VertexBufferObject(const VertexBuffer &buffer) :
	m_vertexFormat(buffer.getVertexFormat())
{
	// Generate buffers
	glGenBuffers(1, &m_vboId);
	glGenBuffers(1, &m_iboId);

	// Upload vertex data
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, buffer.getVertexCount() * m_vertexFormat.getVertexSizeInBytes(), buffer.getVertexData(), toGLBufferType(buffer.getBufferType()));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer.getIndexCount() * sizeof(uint), buffer.getIndexData(), toGLBufferType(buffer.getBufferType()));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexBufferObject::~VertexBufferObject()
{
	glDeleteBuffers(1, &m_vboId);
	glDeleteBuffers(1, &m_iboId);
}

void VertexBufferObject::update(const int offset, const char *data, const int count)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferSubData(GL_ARRAY_BUFFER, offset * m_vertexFormat.getVertexSizeInBytes(), count * m_vertexFormat.getVertexSizeInBytes(), data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}