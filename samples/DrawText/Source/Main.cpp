#include <Sauce/Sauce.h>

using namespace sauce;

class DrawTextGame : public Game
{
	SpriteBatch *m_spriteBatch;
	Resource<Font> m_font;
	shared_ptr<Texture2D> m_texture;

public:
	DrawTextGame() :
		Game("DrawText", SAUCE_WINDOW_RESIZABLE)
	{
	}

	void onStart(GameEvent*)
	{
		m_spriteBatch = new SpriteBatch;
		m_font = Resource<Font>("Arial");
		m_texture = shared_ptr<Texture2D>(getWindow()->getGraphicsContext()->createTexture(Pixmap("Image.png")));
	}

	void onEnd(GameEvent*)
	{
	}

	void onTick(TickEvent*)
	{
	}

	void onDraw(DrawEvent *e)
	{
		m_spriteBatch->begin(e->getGraphicsContext());
		m_font->draw(m_spriteBatch, 10, 10, "This sample shows how to render text using BMFont", FONT_ALIGN_LEFT);
		m_font->setColor(Color(255, 20, 255, 255));
		m_font->draw(m_spriteBatch, e->getGraphicsContext()->getSize() / 2, "This piece of text is center aligned", FONT_ALIGN_CENTER);
		m_font->setColor(Color(255, 255, 255, 255));
		m_font->drawBox(m_spriteBatch, 10, 200, 500, "This shows how you can use drawBox to limit the length of the lines in your text. This is useful for multi-line text editors, in-game chats, etc", 0, FONT_ALIGN_LEFT);
		m_spriteBatch->drawSprite(Sprite(m_texture, RectF(0, 0, 200, 200)));
		m_spriteBatch->end();

		GraphicsContext *graphicsContext = e->getGraphicsContext();
		graphicsContext->setTexture(m_texture);
		graphicsContext->drawRectangle(200, 0, 200, 200);
		graphicsContext->setTexture(0);
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	DrawTextGame game;
	return game.run();
}