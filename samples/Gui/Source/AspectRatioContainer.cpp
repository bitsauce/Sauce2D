#include "AspectRatioContainer.h"

AspectRatioContainer::AspectRatioContainer(UiObject *parent, Window *window, const int maxWidth, const float aspectRatio) :
	UiObject(parent),
	m_window(window),
	m_maxWidth(maxWidth),
	m_aspectRatio(aspectRatio)
{
	setAnchor(0.5f, 0.5f);
	setOrigin(0.5f, 0.5f);

	WindowEvent e(WindowEvent::SIZE_CHANGED, window, window->getWidth(), window->getHeight());
	onWindowSizeChanged(&e);
}

void AspectRatioContainer::onWindowSizeChanged(WindowEvent *e)
{
	// NOTE: There might be some simplification potential here
	Vector2F size;
	size.x = min(m_aspectRatio / ((float) e->getWidth() / (float) e->getHeight()), min((float) m_maxWidth / (float) e->getWidth(), 1.0f));
	size.y = min(((float) e->getWidth() / (float) e->getHeight()) / m_aspectRatio, min((float) (m_maxWidth / m_aspectRatio) / (float) e->getHeight(), 1.0f));
	setSize(size);
	UiObject::onWindowSizeChanged(e);
}

void AspectRatioContainer::onDraw(DrawEvent *e)
{
	UiObject::onDraw(e);

	// TODO: Draw outline?
	//if(debug) {
	e->getGraphicsContext()->drawRectangle(getDrawRect(), Color(127, 127, 127, 127));
	//}
}