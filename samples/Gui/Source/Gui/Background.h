#pragma once

#include "UiObject.h"

class Background : public UiObject
{
public:
	Background(UiObject *parent);

	void onDraw(DrawEvent *e);

private:
	Color m_topColor, m_bottomColor;
	Vertex m_vertices[4];
};