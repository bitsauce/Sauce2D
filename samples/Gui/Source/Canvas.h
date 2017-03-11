#pragma once

#include "UiObject.h"

class Canvas : public UiObject
{
public:
	Canvas(UiObject *parent, Window *window);

	void onDraw(DrawEvent *e);

	Vector2I getDrawPosition();
	Vector2I getDrawSize();

private:
	Color m_topColor, m_bottomColor;
	Vertex m_vertices[4];
	Window *m_window;
};