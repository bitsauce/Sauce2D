#include "Button.h"

Button::Button(UiObject *parent) :
	UiObject(parent),
	m_texture(Resource<Texture2D>("ButtonInactive")),
	m_textureHover(Resource<Texture2D>("ButtonHover")),
	m_textureActive(Resource<Texture2D>("ButtonActive")),
	m_font(Resource<Font>("Font")),
	m_text("Button")
{
	m_renderTarget = new RenderTarget2D(150, 40);
	m_spriteBatch = 0;
}

void Button::onClick(ClickEvent *e)
{
	switch(e->getType())
	{
		case ClickEvent::DONE:
			break;

		case ClickEvent::BEGIN:
			break;

		case ClickEvent::CANCELED:
			break;
	}

	UiObject::onClick(e);
}

void Button::onDraw(DrawEvent *e)
{
	GraphicsContext *graphicsContext = e->getGraphicsContext();
	if(!m_spriteBatch)
	{
		m_spriteBatch =  new SpriteBatch(graphicsContext);
	}

	// Draw button to render target
	graphicsContext->setRenderTarget(m_renderTarget);

	if(isPressed() && isHovered())
	{
		graphicsContext->setTexture(m_textureActive);
		m_text = "Pressed";
	}
	else if(isHovered())
	{
		graphicsContext->setTexture(m_textureHover);
		m_text = "Hover";
	}
	else
	{
		graphicsContext->setTexture(m_texture);
		m_text = "Normal";
	}

	const Vector2I size(150, 40);
	
	graphicsContext->drawRectangle(0.0f,           0.0f,           16.0f, 16.0f, Color(255), TextureRegion(0.0f, 0.0f, 1.0f / 3.0f, 1.0f / 3.0f));
	graphicsContext->drawRectangle(size.x - 16.0f, 0.0f,           16.0f, 16.0f, Color(255), TextureRegion(2.0f / 3.0f, 0.0f, 1.0f, 1.0f / 3.0f));
	graphicsContext->drawRectangle(0.0f,           size.y - 16.0f, 16.0f, 16.0f, Color(255), TextureRegion(0.0f, 2.0f / 3.0f, 1.0f / 3.0f, 1.0f));
	graphicsContext->drawRectangle(size.x - 16.0f, size.y - 16.0f, 16.0f, 16.0f, Color(255), TextureRegion(2.0f / 3.0f, 2.0f / 3.0f, 1.0f, 1.0f));

	graphicsContext->drawRectangle(16.0f,          0.0f,           size.x - 32.0f, 16.0f,          Color(255), TextureRegion(1.0f / 3.0f, 0.0f / 3.0f, 2.0f / 3.0f, 1.0f / 3.0f));
	graphicsContext->drawRectangle(16.0f,          size.y - 16.0f, size.x - 32.0f, 16.0f,          Color(255), TextureRegion(1.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f));
	graphicsContext->drawRectangle(0.0f,           16.0f,          16.0f,          size.y - 32.0f, Color(255), TextureRegion(0.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f, 2.0f / 3.0f));
	graphicsContext->drawRectangle(size.x - 16.0f, 16.0f,          16.0f,          size.y - 32.0f, Color(255), TextureRegion(2.0f / 3.0f, 1.0f / 3.0f, 3.0f / 3.0f, 2.0f / 3.0f));

	graphicsContext->drawRectangle(Vector2F(16.0f), size - Vector2F(32.0f), Color(255), TextureRegion(1.0f / 3.0f, 1.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f));

	graphicsContext->setTexture(0);

	graphicsContext->setRenderTarget(0);

	// Draw button from render target
	RectI rect = getDrawRect();
	graphicsContext->setTexture(m_renderTarget->getTexture());
	graphicsContext->drawRectangle(rect);
	graphicsContext->setTexture(0);

	m_spriteBatch->begin();
	m_font->setHeight(min(rect.size.y, 16.0f));
	m_font->setColor(Color(0, 0, 0, 255));
	m_font->draw(m_spriteBatch, Vector2I(rect.position + rect.size / 2 - Vector2F(0.0f, m_font->getHeight() * 0.5f)), m_text, FONT_ALIGN_CENTER);
	m_font->setHeight(16.0f);
	m_spriteBatch->end();

	UiObject::onDraw(e);
}