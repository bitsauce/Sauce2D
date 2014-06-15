#include "debugdraw.h"
#include "box2d.h"

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x*b2d->getScale(), vertices[i].y*b2d->getScale());
	}
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	for(int i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x*b2d->getScale(), vertices[i].y*b2d->getScale());
	}
	glEnd();

	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x*b2d->getScale(), vertices[i].y*b2d->getScale());
	}
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x*b2d->getScale(), v.y*b2d->getScale());
		theta += k_increment;
	}
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	for(int i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x*b2d->getScale(), v.y*b2d->getScale());
		theta += k_increment;
	}
	glEnd();

	theta = 0.0f;
	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x*b2d->getScale(), v.y*b2d->getScale());
		theta += k_increment;
	}
	glEnd();

	b2Vec2 p = center + radius * axis;
	glBegin(GL_LINES);
	glVertex2f(center.x*b2d->getScale(), center.y*b2d->getScale());
	glVertex2f(p.x*b2d->getScale(), p.y*b2d->getScale());
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2f(p1.x*b2d->getScale(), p1.y*b2d->getScale());
	glVertex2f(p2.x*b2d->getScale(), p2.y*b2d->getScale());
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(p1.x*b2d->getScale(), p1.y*b2d->getScale());
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	glVertex2f(p2.x*b2d->getScale(), p2.y*b2d->getScale());

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(p1.x*b2d->getScale(), p1.y*b2d->getScale());
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	glVertex2f(p2.x*b2d->getScale(), p2.y*b2d->getScale());

	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(p.x*b2d->getScale(), p.y*b2d->getScale());
	glEnd();
	glPointSize(1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void DebugDraw::DrawString(int x, int y, const char* string, ...)
{
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(aabb->lowerBound.x*b2d->getScale(), aabb->lowerBound.y*b2d->getScale());
	glVertex2f(aabb->upperBound.x*b2d->getScale(), aabb->lowerBound.y*b2d->getScale());
	glVertex2f(aabb->upperBound.x*b2d->getScale(), aabb->upperBound.y*b2d->getScale());
	glVertex2f(aabb->lowerBound.x*b2d->getScale(), aabb->upperBound.y*b2d->getScale());
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}