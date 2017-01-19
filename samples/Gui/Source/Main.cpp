#include "Config.h"
#include "Canvas.h"
#include "Button.h"
#include "LineEdit.h"
#include "Gradient.h"

class GuiGame : public Game
{
	SpriteBatch *spriteBatch;
	Canvas *canvas;
	Button *button;

public:
	GuiGame() :
		Game("Gui", SAUCE_WINDOW_RESIZABLE)
	{
	}

	void onKeyDown(KeyEvent *e)
	{
		if((e->getModifiers() & KeyEvent::CTRL) != 0)
		{
			switch(e->getKeycode())
			{
				case SAUCE_KEY_1: getWindow()->setSize(800, 600); break;
				case SAUCE_KEY_2: getWindow()->setSize(600, 800); break;
				case SAUCE_KEY_3: getWindow()->setSize(1024, 1024); break;
				case SAUCE_KEY_4: getWindow()->setSize(1280, 720); break;
				case SAUCE_KEY_5: getWindow()->setSize(1920, 1080); break;
			}
		}
		Game::onKeyDown(e);
	}

	void onStart(GameEvent*)
	{
		canvas = new Canvas(getWindow(), 1280, 720);
		addChildLast(canvas);

		GraphicsContext *gfx = getWindow()->getGraphicsContext();
		spriteBatch = new SpriteBatch(gfx);

		Gradient *back = new Gradient(canvas);
		back->setAnchor(0.5f, 0.5f);
		back->setOrigin(0.5f, 0.5f);
		back->setSize(1.0f, 1.0f);

		button = new Button(back);
		button->setSize(150.0f / 1280.0f, 40.0f / 720.0f);
		button->setAnchor(0.5f, 0.85f);
		button->setOrigin(0.5f, 0.5f);
		button->setPosition(0.0f, 0.0f);

		LineEdit *lineEdit = new LineEdit(gfx, back);
		lineEdit->setSize(200.0f / 1280.0f, 40.0f / 720.0f);
		lineEdit->setPosition(0.0f, -0.5f);
		lineEdit->setAnchor(0.5f, 0.85f);
		lineEdit->setOrigin(0.5f, 0.5f);
		lineEdit->setDefaultText("World name");
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