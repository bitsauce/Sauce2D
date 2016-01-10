#pragma once

#include "UiObject.h"

class Gradient : public UiObject
{
public:
	Gradient();

	void onDraw(DrawEvent *e);

private:
	Color m_topColor, m_bottomColor;
	Vertex m_vertices[4];
};