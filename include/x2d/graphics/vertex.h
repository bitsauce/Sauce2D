#ifndef X2D_VERTEX_H
#define X2D_VERTEX_H

#include <x2d/engine.h>

BEGIN_XD_NAMESPACE

/*********************************************************************
**	Data types														**
**********************************************************************/
enum DataType
{
	XD_FLOAT = GL_FLOAT,
	XD_UINT = GL_UNSIGNED_INT,
	XD_INT = GL_INT,
	XD_USHORT = GL_UNSIGNED_SHORT,
	XD_SHORT = GL_SHORT,
	XD_UBYTE = GL_UNSIGNED_BYTE,
	XD_BYTE = GL_BYTE
};

/*********************************************************************
**	Vertex attributes												**
**********************************************************************/
enum VertexAttribute
{
	VERTEX_POSITION,
	VERTEX_COLOR,
	VERTEX_TEX_COORD,
	VERTEX_NORMAL,
	VERTEX_ATTRIB_MAX
};

/*********************************************************************
**	Vertex format													**
**********************************************************************/
class Vertex;
class XDAPI VertexFormat
{
	friend class Graphics;
	friend class Vertex;
	friend class VertexBuffer;
public:
	VertexFormat();
	VertexFormat(const VertexFormat &other);

	void set(const VertexAttribute attrib, const int size, const DataType = XD_FLOAT);
	int getElementCount(const VertexAttribute attrib) const;
	DataType getDataType(const VertexAttribute attrib) const;
	bool isAttributeEnabled(const VertexAttribute attrib) const;

	uint getVertexSizeInBytes() const;
	uint getAttributeOffset(const VertexAttribute attrib) const;
	
	Vertex *createVertices(const int count) const;
	//XScriptArray *createVerticesAS(const int count) const;
	
	VertexFormat &operator=(const VertexFormat &other);
	bool operator==(const VertexFormat &other);

protected:
	static VertexFormat s_vct; // Position, color, texture coord

private:
	struct Attribute
	{
		Attribute() :
			elementCount(0),
			dataType(XD_FLOAT),
			offset(0)
		{
		}

		int elementCount;
		DataType dataType;
		uint offset;
		
		bool operator!=(const Attribute &other)
		{
			return elementCount != other.elementCount || dataType != other.dataType;
		}
	};

	Attribute m_attributes[VERTEX_ATTRIB_MAX];
	uint m_vertexByteSize;

	static void Construct(VertexFormat *self) { new (self) VertexFormat(); }
};

/*********************************************************************
**	Vertex															**
**********************************************************************/
class XDAPI Vertex
{
public:
	Vertex();
	Vertex(const Vertex &other);
	Vertex(const VertexFormat &fmt);
	~Vertex();
	
	void clear();

	void setFormat(const VertexFormat &fmt);
	VertexFormat getFormat() const;
	// TODO: Add set3* set2* set*
	void set4f(const VertexAttribute attrib, const float v0, const float v1 = 0.0f, const float v2 = 0.0f, const float v3 = 0.0f);
	void set4ui(const VertexAttribute attrib, const uint v0, const uint v1 = 0, const uint v2 = 0, const uint v3 = 0);
	void set4i(const VertexAttribute attrib, const int v0, const int v1 = 0, const int v2 = 0, const int v3 = 0);
	void set4us(const VertexAttribute attrib, const ushort v0, const ushort v1 = 0, const ushort v2 = 0, const ushort v3 = 0);
	void set4s(const VertexAttribute attrib, const short v0, const short v1 = 0, const short v2 = 0, const short v3 = 0);
	void set4ub(const VertexAttribute attrib, const uchar v0, const uchar v1 = 0, const uchar v2 = 0, const uchar v3 = 0);
	void set4b(const VertexAttribute attrib, const char v0, const char v1 = 0, const char v2 = 0, const char v3 = 0);
	void setData(const char *data) const;
	void getData(char *data) const;
	
	Vertex &operator=(const Vertex &other);

	void print();

private:
	char *m_data;
	VertexFormat m_format;

	static void Construct(Vertex *self) { new (self) Vertex(); }
	static void CopyConstruct(const Vertex &other, Vertex *self) { new (self) Vertex(other); }
	static void Destruct(Vertex *self) { self->~Vertex(); }
};

END_XD_NAMESPACE

#endif // X2D_VERTEX_H