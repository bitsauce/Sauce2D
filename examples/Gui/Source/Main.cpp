#include "Config.h"
#include "Canvas.h"
#include "Button.h"

class GuiGame : public Game
{
	SpriteBatch *spriteBatch;
	Canvas *canvas;
	Button *button;
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
		}
		Game::onKeyDown(e);
	}

	void onStart(GameEvent*)
	{
		canvas = new Canvas(getWindow());
		addChildLast(canvas);

		spriteBatch = new SpriteBatch(getWindow()->getGraphicsContext());

		button = new Button();
		button->setWidth(0.125f, 0.3125f);
		button->setAnchor(Vector2F(0.5f, 0.9f));
		button->setPosition(Vector2F(0.0f, 0.0f));

		canvas->addChildLast(button);
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
		spriteBatch->end();
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	GuiGame game;
	return game.run();
}