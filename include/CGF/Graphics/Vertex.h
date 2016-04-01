#ifndef CGF_VERTEX_H
#define CGF_VERTEX_H

#include <CGF/Common.h>

BEGIN_CGF_NAMESPACE

/*********************************************************************
**	Data types														**
**********************************************************************/
enum DataType
{
	CGF_FLOAT = GL_FLOAT,
	CGF_UINT = GL_UNSIGNED_INT,
	CGF_INT = GL_INT,
	CGF_USHORT = GL_UNSIGNED_SHORT,
	CGF_SHORT = GL_SHORT,
	CGF_UBYTE = GL_UNSIGNED_BYTE,
	CGF_BYTE = GL_BYTE
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
class CGF_API VertexFormat
{
	friend class Game;
	friend class Vertex;
	friend class VertexBuffer;
public:
	VertexFormat();
	VertexFormat(const VertexFormat &other);

	void set(const VertexAttribute attrib, const int size, const DataType = CGF_FLOAT);
	int getElementCount(const VertexAttribute attrib) const;
	DataType getDataType(const VertexAttribute attrib) const;
	bool isAttributeEnabled(const VertexAttribute attrib) const;

	uint getVertexSizeInBytes() const;
	uint getAttributeOffset(const VertexAttribute attrib) const;
	
	Vertex *createVertices(const int count) const;
	
	VertexFormat &operator=(const VertexFormat &other);
	bool operator==(const VertexFormat &other);

protected:
	static VertexFormat s_vct; // Position, color, texture coord

private:
	struct Attribute
	{
		Attribute() :
			elementCount(0),
			dataType(CGF_FLOAT),
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
};

/*********************************************************************
**	Vertex															**
**********************************************************************/
class CGF_API Vertex
{
public:
	Vertex();
	Vertex(const Vertex &other);
	Vertex(const VertexFormat &fmt);
	~Vertex();

	void setFormat(const VertexFormat &fmt);
	VertexFormat getFormat() const;

	void set1f(const VertexAttribute attrib, const float v0);
	void set1ui(const VertexAttribute attrib, const uint v0);
	void set1i(const VertexAttribute attrib, const int v0);
	void set1us(const VertexAttribute attrib, const ushort v0);
	void set1s(const VertexAttribute attrib, const short v0);
	void set1ub(const VertexAttribute attrib, const uchar v0);
	void set1b(const VertexAttribute attrib, const char v0);

	void set2f(const VertexAttribute attrib, const float v0, const float v1);
	void set2ui(const VertexAttribute attrib, const uint v0, const uint v1);
	void set2i(const VertexAttribute attrib, const int v0, const int v1);
	void set2us(const VertexAttribute attrib, const ushort v0, const ushort v1);
	void set2s(const VertexAttribute attrib, const short v0, const short v1);
	void set2ub(const VertexAttribute attrib, const uchar v0, const uchar v1);
	void set2b(const VertexAttribute attrib, const char v0, const char v1);

	void set3f(const VertexAttribute attrib, const float v0, const float v1, const float v2);
	void set3ui(const VertexAttribute attrib, const uint v0, const uint v1, const uint v2);
	void set3i(const VertexAttribute attrib, const int v0, const int v1, const int v2);
	void set3us(const VertexAttribute attrib, const ushort v0, const ushort v1, const ushort v2);
	void set3s(const VertexAttribute attrib, const short v0, const short v1, const short v2);
	void set3ub(const VertexAttribute attrib, const uchar v0, const uchar v1, const uchar v2);
	void set3b(const VertexAttribute attrib, const char v0, const char v1, const char v2);

	void set4f(const VertexAttribute attrib, const float v0, const float v1, const float v2, const float v3);
	void set4ui(const VertexAttribute attrib, const uint v0, const uint v1, const uint v2, const uint v3);
	void set4i(const VertexAttribute attrib, const int v0, const int v1, const int v2, const int v3);
	void set4us(const VertexAttribute attrib, const ushort v0, const ushort v1, const ushort v2, const ushort v3);
	void set4s(const VertexAttribute attrib, const short v0, const short v1, const short v2, const short v3);
	void set4ub(const VertexAttribute attrib, const uchar v0, const uchar v1, const uchar v2, const uchar v3);
	void set4b(const VertexAttribute attrib, const char v0, const char v1, const char v2, const char v3);

	void setData(const char *data) const;
	void getData(char *data) const;
	
	Vertex &operator=(const Vertex &other);

	void print();

private:
	char *m_data;
	VertexFormat m_format;
};

END_CGF_NAMESPACE

#endif // CGF_VERTEX_H
