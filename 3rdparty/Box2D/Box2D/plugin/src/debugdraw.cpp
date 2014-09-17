#include "debugdraw.h"
#include "box2d.h"

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	XBatch *batch = b2d->getDrawBatch();
	batch->setPrimitive(XBatch::PRIMITIVE_LINES);
	batch->setTexture(0);

	XVertex *data = new XVertex[vertexCount];
	vector<uint> indices;
	for(int i = 0; i < vertexCount; i++)
	{
		data[i].color.set(color.r, color.g, color.b, 1.0f);
		data[i].position = toXDVec(vertices[i]);

		if(i > 0)
		{
			indices.push_back(i-1);
			indices.push_back(i);
		}
	}

	indices.push_back(vertexCount-1);
	indices.push_back(0);

	batch->addVertices(data, vertexCount, indices.data(), indices.size());

	delete[] data;
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	XBatch *batch = b2d->getDrawBatch();
	batch->setPrimitive(XBatch::PRIMITIVE_TRIANGLES);
	batch->setTexture(0);
	
	XVertex *data = new XVertex[vertexCount];
	vector<uint> indices;

	for(int i = 0; i < vertexCount; i++)
	{
		data[i].color = Vector4(color.r, color.g, color.b, 1.0f)*0.5f;
		data[i].position = toXDVec(vertices[i]);
		
		if(i >= 2)
		{
			indices.push_back(0);
			indices.push_back(i < vertexCount ? i : 1);
			indices.push_back(i-1);
		}
	}
	
	batch->addVertices(data, vertexCount, indices.data(), indices.size());

	delete[] data;

	DrawPolygon(vertices, vertexCount, color);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const int segments = 16;
	const float increment = 2.0f * b2_pi / segments;
	float theta = 0.0f;

	b2Vec2 *vertices = new b2Vec2[segments+2];

	vertices[0] = center;
	for(int i = 1; i < segments+2; i++)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		vertices[i] = v;
		theta += increment;
	}

	DrawPolygon(vertices, segments+2, color);

	delete[] vertices;
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const int segments = 16;
	const float increment = 2.0f * b2_pi / segments;
	float theta = atan2(axis.y, axis.x);

	b2Vec2 *vertices = new b2Vec2[segments+2];

	vertices[0] = center;
	for(int i = 1; i < segments+2; i++)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		vertices[i] = v;
		theta += increment;
	}

	DrawSolidPolygon(vertices, segments+2, color);
	DrawPolygon(vertices, segments+2, color);

	delete[] vertices;
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	b2Vec2 *vertices = new b2Vec2[2];
	vertices[0] = p1;
	vertices[1] = p2;
	DrawPolygon(vertices, 2, color);
	delete[] vertices;
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	XBatch *batch = b2d->getDrawBatch();
	batch->setPrimitive(XBatch::PRIMITIVE_LINES);
	batch->setTexture(0);

	b2Vec2 p1 = xf.p, p2;
	const float32 axisScale = 0.4f;

	XVertex *data = new XVertex[4];
	
	data[0].position = toXDVec(p1);
	data[0].color.set(1.0f, 0.0f, 0.0f, 1.0f);

	p2 = p1 + axisScale * xf.q.GetXAxis();
	
	data[1].position = toXDVec(p2);
	data[1].color.set(1.0f, 0.0f, 0.0f, 1.0f);
	
	data[2].position = toXDVec(p1);
	data[2].color.set(0.0f, 1.0f, 0.0f, 1.0f);

	p2 = p1 + axisScale * xf.q.GetYAxis();
	
	data[3].position = toXDVec(p2);
	data[3].color.set(0.0f, 1.0f, 0.0f, 1.0f);

	uint indices[4] = { 0, 1, 2, 3 };

	batch->addVertices(data, 4, indices, 4);
}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	XBatch *batch = b2d->getDrawBatch();
	batch->setPrimitive(XBatch::PRIMITIVE_POINTS);
	batch->setTexture(0);

	XVertex vertex;
	vertex.color.set(color.r, color.g, color.b, 1.0f);
	vertex.position = toXDVec(p);
	
	uint indices = 0;

	batch->addVertices(&vertex, 1, &indices, 1);
}

void DebugDraw::DrawString(int x, int y, const char* string, ...)
{
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color)
{
	XBatch *batch = b2d->getDrawBatch();
	batch->setPrimitive(XBatch::PRIMITIVE_TRIANGLES);

	XVertex *data = new XVertex[4];
	Vector2 upper = toXDVec(aabb->upperBound);
	Vector2 lower = toXDVec(aabb->lowerBound);
	
	data[0].position.set(lower.x, lower.y);
	data[0].color.set(color.r, color.g, color.b, 1.0f);
	
	data[1].position.set(upper.x, lower.y);
	data[1].color.set(color.r, color.g, color.b, 1.0f);
	
	data[2].position.set(upper.x, upper.y);
	data[2].color.set(color.r, color.g, color.b, 1.0f);

	data[3].position.set(lower.x, upper.y);
	data[3].color.set(color.r, color.g, color.b, 1.0f);

	batch->addVertices(data, 4, QUAD_INDICES, 6);

	delete[] data;
}