#ifndef X2D_VERTEX_BUFFER_OBJECT_H
#define X2D_VERTEX_BUFFER_OBJECT_H

#include "../engine.h"

struct XVertexBuffer;
struct XVertex;

class XDAPI XVertexBufferObject
{
public:
	virtual ~XVertexBufferObject() {}

	virtual void upload(const XVertexBuffer *buffer) = 0;
	virtual void uploadSub(int offset, XVertex *vertices, int count) = 0;
};

#endif // X2D_VERTEX_BUFFER_OBJECT_H