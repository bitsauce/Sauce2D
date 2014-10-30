#ifndef X2D_VERTEX_BUFFER_H
#define X2D_VERTEX_BUFFER_H

#include "../engine.h"

/*********************************************************************
**	Vertex buffer													**
**********************************************************************/
class XVertexBufferObject;

class XDAPI XVertexBuffer
{
	friend class XBatch;
	AS_DECL_VALUE
public:
	XVertexBuffer();
	XVertexBuffer(const XVertexFormat &fmt);
	XVertexBuffer(const XVertexBuffer &other);
	~XVertexBuffer();

	// Add vertices and indices to the batch
	void addVertices(XVertex *vertices, int vcount, uint *indices, int icount);
	void addVerticesAS(XScriptArray *vertices, XScriptArray *indices);
	void modifyVertices(const int idx, XVertex *vertex, const int count);
	void modifyVerticesAS(const int idx, XScriptArray *vertices);

	// Get vertex/vertex count
	XVertexFormat getVertexFormat() const;
	XVertex *getVertices(const int idx, const int count) const;
	XScriptArray *getVerticesAS(const int idx, const int count) const;
	char *getVertexData() const;
	int getVertexCount() const;

	uint *getIndexData() const;
	int getIndexCount() const;

	XVertexBufferObject *getVBO() const { return m_vbo; }

	// Draw vertex buffer
	void draw(XBatch *batch, XTexture *texture);
	void clear();

	enum BufferType
	{
		RAW_BUFFER,
		DYNAMIC_BUFFER,
		STATIC_BUFFER
	};

	void setBufferType(const BufferType drawMode);
	BufferType getBufferType() const;
	
	XVertexBuffer &operator=(const XVertexBuffer &other);

private:

	// Buffer vertex format
	XVertexFormat m_format;
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
	XVertexBufferObject *m_vbo;
	
	static void Construct(XVertexBuffer *self) { new (self) XVertexBuffer(); }
	static void FmtConstruct(const XVertexFormat &fmt, XVertexBuffer *self) { new (self) XVertexBuffer(fmt); }
	static void CopyConstruct(const XVertexBuffer &other, XVertexBuffer *self) { new (self) XVertexBuffer(other); }
	static void Destruct(XVertexBuffer *self) { self->~XVertexBuffer(); }
};

#endif // X2D_VERTEX_BUFFER_H