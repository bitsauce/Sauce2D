#include "Gui.h"

Gui::Gui() :
	m_fadeTime(0.5f),
	m_transitionTime(0.0f),
	m_transitionDirection(0)
{
}

void Gui::onEvent(Event *e)
{
	// Pass the event if it's not a draw event
	if(e->getType() != EVENT_DRAW)
	{
		if(e->getType() == EVENT_WINDOW_SIZE_CHANGED)
		{
			// This accounts for the fringe case where we popped a non-transparent canvas and we resize
			// the window during the trasition. In this case we should ignore the first non-transparent canvas
			int transparentCounter = (m_transitionDirection != 0 && !m_canvas.front()->isTransparent()) ? 2 : 1;
			for(Canvas *canvas : m_canvas)
			{
				canvas->onEvent(e);
				if(!canvas->isTransparent())
				{
					if(--transparentCounter == 0)
					{
						break;
					}
				}
			}
		}
		else
		{
			if(!m_canvas.empty())
			{
				m_canvas.front()->onEvent(e);
			}
		}
	}
	SceneObject::onEvent(e);
}

void Gui::onDraw(DrawEvent *e)
{
	list<Canvas*>::const_iterator itr = m_canvas.begin();
	while(itr != m_canvas.end() && ((*itr)->isTransparent() || m_transitionTime > 0.0f)) itr++;
	if(itr == m_canvas.end()) itr--;
	while(true)
	{
		Canvas *canvas = *itr;
		canvas->onEvent(e);

		GraphicsContext *graphicsContext = e->getGraphicsContext();
		float alpha = itr == m_canvas.begin() ? (m_transitionDirection == -1 ? max(m_transitionTime / m_fadeTime, 0.0f) : min(1.f - (m_transitionTime / m_fadeTime), 1.0f)) : 1.0f;

		// Draw canvas
		Vector2I canvasSize = canvas->getSize();
		graphicsContext->setTexture(canvas->getRenderTarget()->getTexture());
		graphicsContext->drawRectangle(0, 0, canvasSize.x, canvasSize.y, Color(255, 255, 255, 255 * alpha));
		graphicsContext->setTexture(0);

		if(itr == m_canvas.begin())
		{
			break;
		}
		itr--;
	}
}

void Gui::onTick(TickEvent *e)
{
	Canvas *currentCanvas = m_canvas.front();
	m_transitionTime -= e->getDelta();
	if(m_transitionDirection != 0 && m_transitionTime <= 0.0f)
	{
		if(m_transitionDirection == -1)
		{
			m_canvas.pop_front();

			// Send a mouse move event to make sure the previous state is reset
			Vector2F position = Game::Get()->getInputManager()->getPosition();
			MouseEvent e(MouseEvent::MOVE, Game::Get()->getInputManager(), position.x, position.y, SAUCE_MOUSE_BUTTON_NONE, 0, 0);
			m_canvas.front()->onEvent(&e);
		}
		m_transitionDirection = 0;
	}
	SceneObject::onTick(e);
}

void Gui::pushCanvas(Canvas *canvas)
{
	m_canvas.push_front(canvas);
	m_transitionTime = m_fadeTime;
	m_transitionDirection = 1;

	// Send a window resize event to make sure all the elements of the canvas are resized correctly
	WindowEvent e(WindowEvent::SIZE_CHANGED, Game::Get()->getWindow(), Game::Get()->getWindow()->getWidth(), Game::Get()->getWindow()->getHeight());
	canvas->onEvent(&e);
}

bool Gui::popCanvas()
{
	// If there are more than 2 canvases
	if(m_canvas.size() >= 2)
	{
		// Setup transition
		Canvas *currentCanvas = m_canvas.front();
		m_transitionTime = m_fadeTime;
		m_transitionDirection = -1;

		// Send a window resize event to make sure all the elements of the canvas are resized correctly
		WindowEvent e(WindowEvent::SIZE_CHANGED, Game::Get()->getWindow(), Game::Get()->getWindow()->getWidth(), Game::Get()->getWindow()->getHeight());
		(*++m_canvas.begin())->onEvent(&e);
		return false;
	}
	m_canvas.pop_front();
	return true;
}