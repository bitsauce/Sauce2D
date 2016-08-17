#include <Sauce/Sauce.h>

using namespace sauce;

class DrawTextGame : public Game
{
	SpriteBatch *m_spriteBatch;
	Resource<Font> m_font;

public:
	DrawTextGame() :
		Game("DrawText")
	{
	}

	void onStart(GameEvent*)
	{
		m_spriteBatch = new SpriteBatch(getWindow()->getGraphicsContext());
		m_font = Resource<Font>("Arial");
	}

	void onEnd(GameEvent*)
	{
	}

	void onTick(TickEvent*)
	{
	}

	void onDraw(DrawEvent *e)
	{
		m_spriteBatch->begin();
		m_font->draw(m_spriteBatch, 10, 10, "This sample shows how to render text using BMFont", FONT_ALIGN_LEFT);
		m_font->setColor(Color(255, 20, 255, 255));
		m_font->draw(m_spriteBatch, e->getGraphicsContext()->getSize() / 2, "This piece of text is center aligned", FONT_ALIGN_CENTER);
		m_font->setColor(Color(255, 255, 255, 255));
		m_font->drawBox(m_spriteBatch, 10, 200, 500, "This shows how you can use drawBox to limit the length of the lines in your text. This is useful for multi-line text editors, in-game chats, etc", 0, FONT_ALIGN_LEFT);
		m_spriteBatch->end();
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	DrawTextGame game;
	return game.run();
}