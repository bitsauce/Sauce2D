#ifndef X2D_SHAPE_H
#define X2D_SHAPE_H

#include "../engine.h"
#include "batch.h"

class XTexture;

class XDAPI XShape
{
	friend class XSprite;
public:
	XShape();
	XShape(const Rect &rect);
	XShape(const Vector2 &center, const float radius, const int vertCount = -1);
	XShape(const XVertex *vertices, const int vertCount);
	~XShape();

	//void addVertex(const Vertex &vertex);
	//void addVertices(const vector<Vertex> &vertices);

	void setFillColor(const XColor &color);
	void setFillTexture(XTexture* texture);
	void setPenColor(const XColor &color);
	void setPenSize(const float size);

	void draw(XBatch *batch);

	XShape merge(const XShape &shape); // union
	XShape subtract(const XShape &shape);
	XShape intersect(const XShape &shape);

private:
	bool validate();

	XColor m_fillColor;
	XTexture *m_fillTexture;
	XColor m_penColor;
	float m_penSize;
	XVertex *m_vertices;
	int m_vertCount;
	vector<uint> m_indices;
	int m_index;
};

#endif // X2D_SHAPE_H