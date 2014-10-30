#ifndef X2D_VERTEX_H
#define X2D_VERTEX_H

#include "../engine.h"

/*********************************************************************
**	Data types														**
**********************************************************************/
enum XDataType
{
	XD_FLOAT,
	XD_UINT,
	XD_INT,
	XD_USHORT,
	XD_SHORT,
	XD_UBYTE,
	XD_BYTE
};

/*********************************************************************
**	Vertex attributes												**
**********************************************************************/
enum XVertexAttribute
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
class XVertex;
class XDAPI XVertexFormat
{
	friend class XGraphics;
	friend class XVertex;
	friend class XBatch;
	friend class XVertexBuffer;
	AS_DECL_POD
public:
	XVertexFormat();
	XVertexFormat(const XVertexFormat &other);

	void set(const XVertexAttribute attrib, const int size, const XDataType = XD_FLOAT);
	int getElementCount(const XVertexAttribute attrib) const;
	XDataType getDataType(const XVertexAttribute attrib) const;
	bool isAttributeEnabled(const XVertexAttribute attrib) const;

	uint getVertexSizeInBytes() const;
	uint getAttributeOffset(const XVertexAttribute attrib) const;
	
	XVertex *createVertices(const int count) const;
	XScriptArray *createVerticesAS(const int count) const;
	
	XVertexFormat &operator=(const XVertexFormat &other);
	bool operator==(const XVertexFormat &other);

protected:
	static XVertexFormat s_vct; // Position, color, texture coord

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
		XDataType dataType;
		uint offset;
		
		bool operator!=(const Attribute &other)
		{
			return elementCount != other.elementCount || dataType != other.dataType;
		}
	};

	Attribute m_attributes[VERTEX_ATTRIB_MAX];
	uint m_vertexByteSize;

	static void Construct(XVertexFormat *self) { new (self) XVertexFormat(); }
};

/*********************************************************************
**	Vertex															**
**********************************************************************/
class XDAPI XVertex
{
	AS_DECL_VALUE
public:
	XVertex();
	XVertex(const XVertex &other);
	XVertex(const XVertexFormat &fmt);
	~XVertex();
	
	void clear();

	void setFormat(const XVertexFormat &fmt);
	XVertexFormat getFormat() const;
	// TODO: Add set3* set2* set*
	void set4f(const XVertexAttribute attrib, const float v0, const float v1 = 0.0f, const float v2 = 0.0f, const float v3 = 0.0f);
	void set4ui(const XVertexAttribute attrib, const uint v0, const uint v1 = 0, const uint v2 = 0, const uint v3 = 0);
	void set4i(const XVertexAttribute attrib, const int v0, const int v1 = 0, const int v2 = 0, const int v3 = 0);
	void set4us(const XVertexAttribute attrib, const ushort v0, const ushort v1 = 0, const ushort v2 = 0, const ushort v3 = 0);
	void set4s(const XVertexAttribute attrib, const short v0, const short v1 = 0, const short v2 = 0, const short v3 = 0);
	void set4ub(const XVertexAttribute attrib, const uchar v0, const uchar v1 = 0, const uchar v2 = 0, const uchar v3 = 0);
	void set4b(const XVertexAttribute attrib, const char v0, const char v1 = 0, const char v2 = 0, const char v3 = 0);
	void setData(const char *data) const;
	void getData(char *data) const;
	
	XVertex &operator=(const XVertex &other);

	void print();

private:
	char *m_data;
	XVertexFormat m_format;

	static void Construct(XVertex *self) { new (self) XVertex(); }
	static void CopyConstruct(const XVertex &other, XVertex *self) { new (self) XVertex(other); }
	static void Destruct(XVertex *self) { self->~XVertex(); }
};

#endif // X2D_VERTEX_H