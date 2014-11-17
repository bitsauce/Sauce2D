#ifndef X2D_VERTEX_BUFFER_OBJECT_H
#define X2D_VERTEX_BUFFER_OBJECT_H

#include "../engine.h"
#include "vertex.h"

class XVertexBuffer;

class XDAPI XVertexBufferObject
{
	friend class XGraphics;
public:
	XVertexBufferObject(const XVertexBuffer &buffer);
	~XVertexBufferObject();

	void update(const int offset, const char *data, const int count);

private:
	GLuint m_vboId;
	GLuint m_iboId;
	XVertexFormat m_vertexFormat;
};

#endif // X2D_VERTEX_BUFFER_OBJECT_H