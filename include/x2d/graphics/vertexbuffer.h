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
	AS_DECL_REF
public:
	XVertexBuffer(const XVertexFormat &fmt);
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

	XVertexBuffer *copy() const;

	// Makes the batch static for increased performance (by using VBOs)
	// Note: While its called static, modifyVertex will still modify the buffer
	void makeStatic();
	bool isStatic() const;
	
	XVertexBuffer &operator=(const XVertexBuffer &other);

private:
	XVertexBuffer(const XVertexBuffer&);

	// Buffer vertex format
	XVertexFormat m_format;
	int m_vertexSize;

	// Vertices
	char *m_vertexData;
	int m_vertexCount;

	// Indices
	uint *m_indexData;
	int m_indexCount;

	// Static vbo
	XVertexBufferObject *m_vbo;

	static XVertexBuffer *Factory(const XVertexFormat &fmt) { return new XVertexBuffer(fmt); }
};

#endif // X2D_VERTEX_BUFFER_H