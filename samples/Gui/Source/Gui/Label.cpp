#include "Label.h"

Label::Label(UiObject *parent) :
	UiObject(parent),
	m_font(Resource<Font>("Font.fnt")),
	m_text("")
{
	m_font.get()->setColor(Color(0, 0, 0, 255));
}

void Label::onDraw(DrawEvent *e)
{
	RectF rect = getDrawRect();
	GraphicsContext *g = e->getGraphicsContext();

	SpriteBatch *spriteBatch = (SpriteBatch*) e->getUserData();
	m_font->setHeight(max(rect.size.y - 34.0f, 16.0f));
	m_font->draw(spriteBatch, rect.getCenter() - Vector2F(0.0f, m_font->getHeight() * 0.5f), m_text);

	UiObject::onDraw(e);
}