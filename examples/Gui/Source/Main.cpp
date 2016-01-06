#include "Config.h"
#include "Canvas.h"
#include "Box.h"

class GuiGame : public Game
{
	SpriteBatch *spriteBatch;
	Canvas *canvas;
	Box *box;
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

		box = new Box();
		box->setWidth(0.125f, 0.3125f);
		box->setPosition(Vector2(0.1f, 0.1f));
		//box->setPositionPx();
		//box->setAspectRatio();

		canvas->addChildLast(box);
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