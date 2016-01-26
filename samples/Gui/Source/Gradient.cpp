#include "Gradient.h"

Gradient::Gradient(UiObject *parent) :
	UiObject(parent),
	m_topColor(200, 200, 200),
	m_bottomColor(250, 250, 250),
	m_font(Game::GetInstance()->getResourceManager()->get<Font>("Font"))
{
}

void Gradient::onDraw(DrawEvent *e)
{
	RectI rect = getDrawRect();
	GraphicsContext *g = e->getGraphicsContext();

	// Draw gradient
	m_vertices[0].set4f(VERTEX_POSITION, rect.getLeft(), rect.getTop());
	m_vertices[1].set4f(VERTEX_POSITION, rect.getLeft(), rect.getBottom());
	m_vertices[2].set4f(VERTEX_POSITION, rect.getRight(), rect.getTop());
	m_vertices[3].set4f(VERTEX_POSITION, rect.getRight(), rect.getBottom());
	m_vertices[0].set4ub(VERTEX_COLOR, m_topColor.r, m_topColor.g, m_topColor.b, m_topColor.a);
	m_vertices[1].set4ub(VERTEX_COLOR, m_bottomColor.r, m_bottomColor.g, m_bottomColor.b, m_bottomColor.a);
	m_vertices[2].set4ub(VERTEX_COLOR, m_topColor.r, m_topColor.g, m_topColor.b, m_topColor.a);
	m_vertices[3].set4ub(VERTEX_COLOR, m_bottomColor.r, m_bottomColor.g, m_bottomColor.b, m_bottomColor.a);
	g->drawPrimitives(GraphicsContext::PRIMITIVE_TRIANGLE_STRIP, m_vertices, 4);

	SpriteBatch *spriteBatch = (SpriteBatch*)e->getUserData();

	Game *game = Game::GetInstance();
	Vector2I pos = game->getInputManager()->getPosition();
	string str = "FPS: " + util::floatToStr(game->getFPS()) + "\nCursor Pos: " + util::intToStr(pos.x) + ", " + util::intToStr(pos.y) + "\nCanvas aspect ratio: " + util::floatToStr((float)rect.size.y / (float)rect.size.x);

	m_font.get()->draw(spriteBatch, Vector2F(10.0f), str);

	SceneObject::onDraw(e);
}