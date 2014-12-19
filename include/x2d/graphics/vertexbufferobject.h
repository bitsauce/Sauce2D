#ifndef X2D_VERTEX_BUFFER_OBJECT_H
#define X2D_VERTEX_BUFFER_OBJECT_H

#include "../engine.h"
#include "vertex.h"

namespace xd {

class VertexBuffer;

class XDAPI VertexBufferObject
{
	friend class GraphicsContext;
public:
	VertexBufferObject(const VertexBuffer &buffer);
	~VertexBufferObject();

	void update(const int offset, const char *data, const int count);

private:
	GLuint m_vboId;
	GLuint m_iboId;
	VertexFormat m_vertexFormat;
};

}

#endif // X2D_VERTEX_BUFFER_OBJECT_H