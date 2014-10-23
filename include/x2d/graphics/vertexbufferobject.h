#ifndef X2D_VERTEX_BUFFER_OBJECT_H
#define X2D_VERTEX_BUFFER_OBJECT_H

#include "../engine.h"

class XDAPI XVertexBufferObject
{
public:
	virtual ~XVertexBufferObject() {}
	virtual void update(const int offset, const char *data, const int count) = 0;
};

#endif // X2D_VERTEX_BUFFER_OBJECT_H