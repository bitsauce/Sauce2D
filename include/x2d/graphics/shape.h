#ifndef X2D_SHAPE_H
#define X2D_SHAPE_H

#include "../engine.h"
#include "batch.h"

class XTexture;

class XDAPI XShape
{
	friend class XSprite;
public:
	AS_DECL_REF

	XShape();
	XShape(const Rect &rect);
	XShape(const Vector2 &center, const float radius, const int vertCount = -1);
	XShape(const XVertex *vertices, const int vertCount);
	~XShape();

	//void addVertex(const Vertex &vertex);
	//void addVertices(const vector<Vertex> &vertices);

	void setFillColor(const Vector4 &color);
	void setFillTexture(XTexture* texture);
	void setPenColor(const Vector4 &color);
	void setPenSize(const float size);

	void draw(XBatch *batch);

	XShape merge(const XShape &shape); // union
	XShape subtract(const XShape &shape);
	XShape intersect(const XShape &shape);

private:
	bool validate();

	Vector4 m_fillColor;
	XTexture *m_fillTexture;
	Vector4 m_penColor;
	float m_penSize;
	XVertex *m_vertices;
	int m_vertCount;
	vector<uint> m_indices;
	int m_index;
	
	static XShape *Factory() { return new XShape(); }
	static XShape *Factory(const Rect& rect) { return new XShape(rect); }
	static XShape *Factory(const Vector2& center, const float radius, const int vertCount) { return new XShape(center, radius, vertCount); }
	static XShape *Factory(const XScriptArray& arr);
};

#endif // X2D_SHAPE_H