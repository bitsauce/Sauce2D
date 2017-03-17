#include "Gui.h"
#include "Canvas.h"

Gui::Gui()
{
}

void Gui::onEvent(Event *e)
{
	for(Canvas *canvas : m_canvas)
	{
		canvas->onEvent(e);
		if(!canvas->isTransparent())
		{
			break;
		}
	}
	SceneObject::onEvent(e);
}

void Gui::pushCanvas(Canvas *canvas)
{
	m_canvas.push_front(canvas);
}

bool Gui::popCanvas()
{
	m_canvas.pop_front();
	return m_canvas.empty();
}