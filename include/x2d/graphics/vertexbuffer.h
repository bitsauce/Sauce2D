#ifndef X2D_VERTEX_BUFFER_H
#define X2D_VERTEX_BUFFER_H

#include "../engine.h"

namespace xd {

/*********************************************************************
**	Vertex buffer													**
**********************************************************************/
class VertexBufferObject;

class XDAPI VertexBuffer
{
	friend class Batch;
public:
	VertexBuffer();
	VertexBuffer(const VertexFormat &fmt);
	VertexBuffer(const VertexBuffer &other);
	~VertexBuffer();

	// Add vertices and indices to the batch
	void addVertices(Vertex *vertices, int vcount, uint *indices, int icount);
	//void addVerticesAS(XScriptArray *vertices, XScriptArray *indices);
	void modifyVertices(const int idx, Vertex *vertex, const int count);
	//void modifyVerticesAS(const int idx, XScriptArray *vertices);

	// Get vertex/vertex count
	VertexFormat getVertexFormat() const;
	Vertex *getVertices(const int idx, const int count) const;
	//XScriptArray *getVerticesAS(const int idx, const int count) const;
	char *getVertexData() const;
	int getVertexCount() const;

	uint *getIndexData() const;
	int getIndexCount() const;

	VertexBufferObject *getVBO() const { return m_vbo; }

	// Clear vertex buffer
	void clear();

	enum BufferType
	{
		RAW_BUFFER,
		DYNAMIC_BUFFER,
		STATIC_BUFFER
	};

	void setBufferType(const BufferType drawMode);
	BufferType getBufferType() const;
	
	VertexBuffer &operator=(const VertexBuffer &other);

private:

	// Buffer vertex format
	VertexFormat m_format;
	int m_vertexSize;

	// Vertices
	char *m_vertexData;
	int m_vertexCount;

	// Indices
	uint *m_indexData;
	int m_indexCount;

	// Buffer type
	BufferType m_bufferType;

	// VBO object
	VertexBufferObject *m_vbo;
	
	static void Construct(VertexBuffer *self) { new (self) VertexBuffer(); }
	static void FmtConstruct(const VertexFormat &fmt, VertexBuffer *self) { new (self) VertexBuffer(fmt); }
	static void CopyConstruct(const VertexBuffer &other, VertexBuffer *self) { new (self) VertexBuffer(other); }
	static void Destruct(VertexBuffer *self) { self->~VertexBuffer(); }
};

}

#endif // X2D_VERTEX_BUFFER_H