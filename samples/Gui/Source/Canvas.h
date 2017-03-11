#pragma once

#include "UiObject.h"

class Canvas : public UiObject
{
public:
	Canvas(Window *window);

	void onDraw(DrawEvent *e);

	Vector2I getDrawPosition() const;
	Vector2I getDrawSize() const;

private:
	Color m_topColor, m_bottomColor;
	Vertex m_vertices[4];
	Window *m_window;
};