#include "Canvas.h"

Canvas::Canvas(Window *window) :
	m_window(window),
	m_canvasWidth(0),
	m_canvasHeight(0),
	m_useWindowSize(true)
{
}

Canvas::Canvas(Window *window, const int width, const int height) :
	m_window(window),
	m_canvasWidth(width),
	m_canvasHeight(height),
	m_useWindowSize(false)
{
	m_anchor.set(0.5f, 0.5f);
}

Vector2 Canvas::getSize() const
{
	Vector2i windowSize = m_window->getSize();
	if(m_useWindowSize)
	{
		return windowSize;
	}
	else
	{
		Vector2 size;
		if(windowSize.x < windowSize.y)
		{
			// Fit width and use inverse aspect ratio
			size.x = (float) min(m_canvasWidth, windowSize.x);
			size.y = size.x * (float) m_canvasHeight / (float) m_canvasWidth;
		}
		else
		{
			// Fit height and use aspect ratio
			size.y = (float) min(m_canvasHeight, windowSize.y);
			size.x = size.y * (float) m_canvasWidth / (float) m_canvasHeight;
		}
		return size;
	}
}