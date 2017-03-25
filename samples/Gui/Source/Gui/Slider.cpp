#include "Slider.h"

Slider::Slider(UiObject *parent) :
	UiObject(parent),
	m_percentage(0.0f)
{
}

void Slider::onTick(TickEvent *e)
{
	m_percentage += e->getDelta();
	if(m_percentage > 1.0f)
	{
		m_percentage = 0.0f;
	}
}

void Slider::onDraw(DrawEvent *e)
{
	RectI rect = getDrawRect();

	GraphicsContext *graphicsContext = e->getGraphicsContext();
	graphicsContext->drawCircle(rect.position.x, rect.position.y, 16.0f, 32);
	graphicsContext->drawRectangle(rect.position.x);
}
