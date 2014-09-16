#ifndef X2D_VERTEX_BUFFER_OBJECT_H
#define X2D_VERTEX_BUFFER_OBJECT_H

#include "common/engine.h"

struct VertexBuffer;
struct Vertex;

class XDAPI XVertexBufferObject
{
public:
	virtual ~XVertexBufferObject() {}

	virtual void upload(const VertexBuffer *buffer) = 0;
	virtual void uploadSub(int offset, Vertex *vertices, int count) = 0;
};

#endif // X2D_VERTEX_BUFFER_OBJECT_H