#ifndef X2D_SHAPE_H
#define X2D_SHAPE_H

#include <x2d/engine.h>

BEGIN_CG_NAMESPACE

class Texture2D;

class XDAPI Shape
{
	friend class Sprite;
public:
	Shape();
	Shape(const Rect &rect);
	Shape(const Vector2 &center, const float radius, const int vertCount = -1);
	Shape(const Vertex *vertices, const int vertCount);
	~Shape();

	//void addVertex(const Vertex &vertex);
	//void addVertices(const vector<Vertex> &vertices);

	void setFillColor(const Color &color);
	void setFillTexture(const Texture2DPtr &texture);
	void setPenColor(const Color &color);
	void setPenSize(const float size);

	//void draw(Batch *batch);

	Shape merge(const Shape &shape); // union
	Shape subtract(const Shape &shape);
	Shape intersect(const Shape &shape);

private:
	bool validate();

	Color m_fillColor;
	Texture2DPtr m_fillTexture;
	Color m_penColor;
	float m_penSize;
	Vertex *m_vertices;
	int m_vertCount;
	vector<uint> m_indices;
	int m_index;
};

END_CG_NAMESPACE

#endif // X2D_SHAPE_H