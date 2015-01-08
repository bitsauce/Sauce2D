#ifndef X2D_VERTEX_BUFFER_H
#define X2D_VERTEX_BUFFER_H

#include "../engine.h"

BEGIN_XD_NAMESPACE

class Vertex;

/*********************************************************************
**	Vertex buffer													**
**********************************************************************/
class XDAPI VertexBuffer
{
	friend class GraphicsContext;
public:
	// Add vertices and indices to the batch
	void setData(const Vertex *vertices, const uint vertexCount);
	char *getData() const;

	// Get vertex/vertex format/vertex count
	VertexFormat getVertexFormat() const;
	uint getSize() const { return m_size; }

protected:

	enum BufferType
	{
		STATIC_BUFFER = GL_STATIC_DRAW,
		DYNAMIC_BUFFER = GL_DYNAMIC_DRAW
	};

	VertexBuffer(const BufferType type);
	~VertexBuffer();

	// Buffer ID
	GLuint m_id;

	// Vertex format
	VertexFormat m_format;

private:
	// Buffer type
	BufferType m_type;

	// Size
	uint m_size;
};

class XDAPI DynamicVertexBuffer : public VertexBuffer
{
public:
	DynamicVertexBuffer();
	DynamicVertexBuffer(const Vertex *vertices, const uint vertexCount);

	void modifyData(const uint startIdx, Vertex *vertex, const uint vertexCount);
};

class XDAPI StaticVertexBuffer : public VertexBuffer
{
public:
	StaticVertexBuffer();
	StaticVertexBuffer(const Vertex *vertices, const uint vertexCount);
};

/*********************************************************************
**	Index buffer													**
**********************************************************************/
class XDAPI IndexBuffer
{
	friend class GraphicsContext;
public:
	// Add vertices and indices to the batch
	void setData(const uint *indices, const uint indexCount);
	char *getData() const;

	// Get size
	uint getSize() const { return m_size; }

protected:

	enum BufferType
	{
		STATIC_BUFFER = GL_STATIC_DRAW,
		DYNAMIC_BUFFER = GL_DYNAMIC_DRAW
	};

	IndexBuffer(const BufferType type);
	~IndexBuffer();

	// Buffer ID
	GLuint m_id;

private:
	// Buffer type
	BufferType m_type;

	// Size
	uint m_size;
};

class XDAPI DynamicIndexBuffer : public IndexBuffer
{
public:
	DynamicIndexBuffer();
	DynamicIndexBuffer(const uint *vertices, const uint indexCount);

	void modifyData(const uint startIdx, uint *indices, const uint indexCount);
};

class XDAPI StaticIndexBuffer : public IndexBuffer
{
public:
	StaticIndexBuffer();
	StaticIndexBuffer(const uint *vertices, const uint indexCount);
};

END_XD_NAMESPACE

#endif // X2D_VERTEX_BUFFER_H