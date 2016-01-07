#ifndef CGF_SHAPE_H
#define CGF_SHAPE_H

#include <CGF/Common.h>

BEGIN_CGF_NAMESPACE

class Texture2D;

class CGF_API Shape
{
	friend class Sprite;
public:
	Shape();
	Shape(const Rect<float> &rect);
	Shape(const Vector2F &center, const float radius, const int vertCount = -1);
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

END_CGF_NAMESPACE

#endif // CGF_SHAPE_H
