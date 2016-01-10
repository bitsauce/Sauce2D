#include "Canvas.h"

Canvas::Canvas(Window *window) :
	m_window(window),
	m_canvasWidth(0),
	m_canvasHeight(0),
	m_useWindowSize(true)
{
	WindowEvent e(WindowEvent::SIZE_CHANGED, window, window->getWidth(), window->getHeight());
	onWindowSizeChanged(&e);
}

Canvas::Canvas(Window *window, const int width, const int height) :
	m_window(window),
	m_canvasWidth(width),
	m_canvasHeight(height),
	m_useWindowSize(false)
{
	m_anchor.set(0.5f, 0.5f);

	WindowEvent e(WindowEvent::SIZE_CHANGED, window, window->getWidth(), window->getHeight());
	onWindowSizeChanged(&e);
}

void Canvas::onWindowSizeChanged(WindowEvent *e)
{
	m_rect.size = m_window->getSize();
}

Vector2I Canvas::getDrawPosition()
{
	return Vector2I(0, 0);
}

Vector2I Canvas::getDrawSize()
{
	return m_window->getSize();
}
