#include "Canvas.h"

Canvas::Canvas(Window *window, const bool transparent) :
	UiObject(0, false),
	m_transparent(transparent)
{
	setSize(window->getWidth(), window->getHeight());
}

void Canvas::onDraw(DrawEvent *e)
{
	UiObject::onDraw(e);
}

void Canvas::onWindowSizeChanged(WindowEvent *e)
{
	setSize(e->getWidth(), e->getHeight());
}

bool Canvas::isTransparent() const
{
	return m_transparent;
}
