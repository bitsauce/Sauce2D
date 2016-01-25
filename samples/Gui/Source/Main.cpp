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
	FontPtr font;
public:
	GuiGame() :
		Game("Gui")
	{
	}

	void onKeyDown(KeyEvent *e)
	{
		if((e->getModifiers() & KeyEvent::CTRL) != 0)
		{
			switch(e->getKeycode())
			{
				case CGF_KEY_1: getWindow()->setSize(800, 600); break;
				case CGF_KEY_2: getWindow()->setSize(600, 800); break;
				case CGF_KEY_3: getWindow()->setSize(1024, 1024); break;
				case CGF_KEY_4: getWindow()->setSize(1280, 720); break;
				case CGF_KEY_5: getWindow()->setSize(1920, 1080); break;
			}
		}
		Game::onKeyDown(e);
	}

	void onStart(GameEvent*)
	{
		// Font holds a shared_ptr<FontResource>, which is loaded by the ResourceManager
		/*Font*/ font = ResourceManager::get<Font>("Font.fnt");

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