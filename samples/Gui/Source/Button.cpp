#include "Button.h"

Button::Button(UiObject *parent) :
	UiObject(parent),
	m_texture(Game::GetInstance()->getResourceManager()->get<Texture2D>("ButtonInactive")),
	m_textureHover(Game::GetInstance()->getResourceManager()->get<Texture2D>("ButtonHover")),
	m_textureActive(Game::GetInstance()->getResourceManager()->get<Texture2D>("ButtonActive")),
	m_font(Game::GetInstance()->getResourceManager()->get<Font>("Font")),
	m_text("Button")
{
	m_font.get()->setColor(Color(0, 0, 0, 255));
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
	RectF rect = RectF(getDrawRect());
	GraphicsContext *g = e->getGraphicsContext();
	if(isPressed() && isHovered())
	{
		g->setTexture(m_textureActive.get());
		m_text = "Pressed";
	}
	else if(isHovered())
	{
		g->setTexture(m_textureHover.get());
		m_text = "Hover";
	}
	else
	{
		g->setTexture(m_texture.get());
		m_text = "Normal";
	}
	
	g->drawRectangle(rect.position, Vector2F(16.0f), Color(255), TextureRegion(0.0f, 0.0f, 1.0f / 3.0f, 1.0f / 3.0f));
	g->drawRectangle(rect.position.x + rect.size.x - 16.0f, rect.position.y, 16.0f, 16.0f, Color(255), TextureRegion(2.0f / 3.0f, 0.0f, 1.0f, 1.0f / 3.0f));
	g->drawRectangle(rect.position.x, rect.position.y + rect.size.y - 16.0f, 16.0f, 16.0f, Color(255), TextureRegion(0.0f, 2.0f / 3.0f, 1.0f / 3.0f, 1.0f));
	g->drawRectangle(rect.position + rect.size - Vector2F(16.0f), Vector2F(16.0f), Color(255), TextureRegion(2.0f / 3.0f, 2.0f / 3.0f, 1.0f, 1.0f));

	g->drawRectangle(rect.position.x + 16.0f,               rect.position.y,                       rect.size.x - 32.0f, 16.0f,               Color(255), TextureRegion(1.0f / 3.0f, 0.0f / 3.0f, 2.0f / 3.0f, 1.0f / 3.0f));
	g->drawRectangle(rect.position.x + 16.0f,               rect.position.y + rect.size.y - 16.0f, rect.size.x - 32.0f, 16.0f,               Color(255), TextureRegion(1.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f, 3.0f / 3.0f));
	g->drawRectangle(rect.position.x,                       rect.position.y + 16.0f,               16.0f,               rect.size.y - 32.0f, Color(255), TextureRegion(0.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f, 2.0f / 3.0f));
	g->drawRectangle(rect.position.x + rect.size.x - 16.0f, rect.position.y + 16.0f,               16.0f,               rect.size.y - 32.0f, Color(255), TextureRegion(2.0f / 3.0f, 1.0f / 3.0f, 3.0f / 3.0f, 2.0f / 3.0f));

	g->drawRectangle(rect.position + Vector2F(16.0f), rect.size - Vector2F(32.0f), Color(255), TextureRegion(1.0f / 3.0f, 1.0f / 3.0f, 2.0f / 3.0f, 2.0f / 3.0f));

	g->setTexture(0);

	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
	m_font.get()->setHeight(max(rect.size.y - 34.0f, 16.0f));
	m_font.get()->draw(spriteBatch, rect.getCenter() - Vector2F(0.0f, m_font.get()->getHeight() * 0.5f), m_text, FONT_ALIGN_CENTER);

	UiObject::onDraw(e);
}