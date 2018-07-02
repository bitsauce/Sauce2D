#ifndef SAUCE_VERTEX_BUFFER_H
#define SAUCE_VERTEX_BUFFER_H

#include <Sauce/Common.h>

BEGIN_SAUCE_NAMESPACE

class Vertex;

/*********************************************************************
**	Vertex buffer													**
**********************************************************************/
class SAUCE_API VertexBuffer
{
	friend class OpenGLContext;
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

class SAUCE_API DynamicVertexBuffer : public VertexBuffer
{
public:
	DynamicVertexBuffer();
	DynamicVertexBuffer(const Vertex *vertices, const uint vertexCount);

	void modifyData(const uint startIdx, Vertex *vertex, const uint vertexCount);
};

class SAUCE_API StaticVertexBuffer : public VertexBuffer
{
public:
	StaticVertexBuffer();
	StaticVertexBuffer(const Vertex *vertices, const uint vertexCount);
};

/*********************************************************************
**	Index buffer													**
**********************************************************************/
class SAUCE_API IndexBuffer
{
	friend class OpenGLContext;
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

class SAUCE_API DynamicIndexBuffer : public IndexBuffer
{
public:
	DynamicIndexBuffer();
	DynamicIndexBuffer(const uint *vertices, const uint indexCount);

	void modifyData(const uint startIdx, uint *indices, const uint indexCount);
};

class SAUCE_API StaticIndexBuffer : public IndexBuffer
{
public:
	StaticIndexBuffer();
	StaticIndexBuffer(const uint *vertices, const uint indexCount);
};

END_SAUCE_NAMESPACE

#endif // SAUCE_VERTEX_BUFFER_H
