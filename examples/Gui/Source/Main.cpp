#include "Config.h"
#include "Canvas.h"
#include "Button.h"
#include "Gradient.h"

class GuiGame : public Game
{
	SpriteBatch *spriteBatch;
	Canvas *canvas;
	Button *button;
	FontPtr font;
public:
	GuiGame() :
		Game("Gui")
	{
	}

	void onKeyDown(KeyEvent *e)
	{
		switch(e->getKeycode())
		{
			case CGF_KEY_1: getWindow()->setSize(800, 600); break;
			case CGF_KEY_2: getWindow()->setSize(1024, 1024); break;
			case CGF_KEY_3: getWindow()->setSize(1280, 720); break;
			case CGF_KEY_4: getWindow()->setSize(1920, 1080); break;
			case CGF_KEY_5: getWindow()->setSize(600, 800); break;
		}
		Game::onKeyDown(e);
	}

	void onStart(GameEvent*)
	{
		font = ResourceManager::get<Font>("Font.fnt");

		canvas = new Canvas(getWindow(), 1280, 720);
		addChildLast(canvas);

		spriteBatch = new SpriteBatch(getWindow()->getGraphicsContext());

		Gradient *back = new Gradient();
		back->setAnchor(0.5f, 0.5f);
		back->setWidth(1.0f, 720.0f / 1280.0f);
		canvas->addChildLast(back);

		button = new Button();
		button->setWidth(0.125f, 0.3125f);
		button->setAnchor(Vector2F(0.5f, 0.85f));
		button->setPosition(Vector2F(0.0f, 0.0f));

		back->addChildLast(button);
	}

	void onEnd(GameEvent*)
	{
	}

	void onTick(TickEvent *e)
	{
		Game::onTick(e);
	}

	void onDraw(DrawEvent *e)
	{
		e->setUserData(spriteBatch);
		spriteBatch->begin();
		Game::onDraw(e);

		// Show cursor position
		Vector2I pos = getInputManager()->getPosition();
		font->draw(spriteBatch, Vector2F(10), util::intToStr(pos.x) + ", " + util::intToStr(pos.y));

		spriteBatch->end();
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	GuiGame game;
	return game.run();
}