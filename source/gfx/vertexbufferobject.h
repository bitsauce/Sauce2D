#ifndef GFX_VERTEX_BUFFER_OBJECT_H
#define GFX_VERTEX_BUFFER_OBJECT_H

#include "batch.h"

class XDAPI VertexBufferObject
{
public:
	virtual ~VertexBufferObject() {}

	virtual void upload(const VertexBuffer *buffer) = 0;
	virtual void uploadSub(int offset, Vertex *vertices, int count) = 0;
};

#endif // GFX_VERTEX_BUFFER_OBJECT_H