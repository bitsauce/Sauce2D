#include "Config.h"
#include "AspectRatioContainer.h"
#include "Button.h"
#include "LineEdit.h"
#include "Canvas.h"

class GuiGame : public Game
{
	SpriteBatch *spriteBatch;
	Canvas *canvas;
	Button *button;
	Resource<Font> font;

public:
	GuiGame() :
		Game("Gui", SAUCE_WINDOW_RESIZABLE)
	{
	}

	void onKeyDown(KeyEvent *e)
	{
		Game::onKeyDown(e);
	}

	void onStart(GameEvent*)
	{
		font = Resource<Font>("Font");

		GraphicsContext *gfx = getWindow()->getGraphicsContext();
		spriteBatch = new SpriteBatch(gfx);

		canvas = new Canvas(canvas, getWindow());
		canvas->setSize(1.0f, 1.0f);
		addChildLast(canvas);

		AspectRatioContainer *aspectRatioContainer = new AspectRatioContainer(getWindow(), 1280, 720);
		aspectRatioContainer->setAnchor(0.5f, 0.5f);
		aspectRatioContainer->setOrigin(0.5f, 0.5f);
		aspectRatioContainer->setSize(1.0f, 1.0f);

		button = new Button(aspectRatioContainer);
		button->setSize(150.0f / 1280.0f, 40.0f / 720.0f);
		button->setAnchor(0.5f, 0.85f);
		button->setOrigin(0.5f, 0.5f);
		button->setPosition(0.0f, 0.0f);

		LineEdit *lineEdit = new LineEdit(gfx, aspectRatioContainer);
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
		// Setup sprite batch
		e->setUserData(spriteBatch);
		spriteBatch->begin();

		// Draw UI objects
		Game::onDraw(e);

		// Draw debug info
		Game *game = Game::Get();

		// Make debugstring
		stringstream ss;
		ss << "FPS: " << game->getFPS() << "\n";
		ss << "Cursor position: " << game->getInputManager()->getPosition() << "\n";
		ss << "Canvas aspect ratio: " << canvas->getAspectRatio() << "\n";

		font->draw(spriteBatch, Vector2F(10.0f), ss.str());

		spriteBatch->end();
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	GuiGame game;
	return game.run();
}