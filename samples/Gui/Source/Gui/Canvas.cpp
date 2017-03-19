#include "Canvas.h"

Canvas::Canvas(Window *window, const bool transparent) :
	UiObject(0, false),
	m_transparent(transparent)
{
	m_renderTarget = new RenderTarget2D(window->getWidth(), window->getHeight());
	setSize(window->getWidth(), window->getHeight());
}

void Canvas::onDraw(DrawEvent *e)
{
	GraphicsContext *graphicsContext = e->getGraphicsContext();
	graphicsContext->pushState();
	graphicsContext->setRenderTarget(m_renderTarget);
	graphicsContext->setBlendState(BlendState(BlendState::BLEND_ZERO, BlendState::BLEND_ZERO));
	graphicsContext->drawRectangle(0, 0, m_renderTarget->getWidth(), m_renderTarget->getHeight());
	graphicsContext->setBlendState(BlendState(BlendState::PRESET_PREMULTIPLIED_ALPHA));
	UiObject::onDraw(e);
	graphicsContext->popState();
}

void Canvas::onWindowSizeChanged(WindowEvent *e)
{
	if(m_renderTarget->getWidth() != e->getWidth() || m_renderTarget->getHeight() != e->getHeight())
	{
		delete m_renderTarget;
		m_renderTarget = new RenderTarget2D(e->getWidth(), e->getHeight());
		setSize(e->getWidth(), e->getHeight());
	}
}

bool Canvas::isTransparent() const
{
	return m_transparent;
}
