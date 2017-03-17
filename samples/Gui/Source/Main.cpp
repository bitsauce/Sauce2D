#include "Config.h"
#include "Gui/AspectRatioContainer.h"
#include "Gui/Button.h"
#include "Gui/LineEdit.h"
#include "Gui/Canvas.h"
#include "Gui/Gui.h"
#include "Gui/Background.h"

// TODO: Expand this sample so it can:
// o Go from one menu to another (with crossfade)
// o Manage a menu stack.
//   Example: Main Menu -> Start Game -> Options -> Back -> Back -> Options
//   Should both show the same options menu
// o Pop-up messages
// o Sliders
// o Animated buttons
// o Button sounds

class GuiGame : public Game
{
	SpriteBatch *spriteBatch;
	Canvas *canvas;
	Button *button1;
	Button *button2;
	Button *button3;
	LineEdit *lineEdit;
	AspectRatioContainer *aspectRatioContainer;
	Resource<Font> font;
	Gui *gui;

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

		GraphicsContext *graphicsContext = getWindow()->getGraphicsContext();
		spriteBatch = new SpriteBatch(graphicsContext);

		gui = new Gui();

		canvas = new Canvas(getWindow());

		Background *bg = new Background(canvas);

		aspectRatioContainer = new AspectRatioContainer(bg, getWindow(), 1280, 1280.0f / 720.0f);
		aspectRatioContainer->setAnchor(0.5f, 0.5f);
		aspectRatioContainer->setOrigin(0.5f, 0.5f);

		button1 = new Button(aspectRatioContainer, 150, 40);
		button1->setSize(150.0f / 1280.0f, 40.0f / 720.0f);
		button1->setAnchor(0.5f, 0.85f);
		button1->setOrigin(0.5f, 0.5f);
		button1->setPosition(0.0f, 0.0f);

		button2 = new Button(aspectRatioContainer, 1280, 40);
		button2->setSize(1.0f, 40.0f / 720.0f);
		button2->setAnchor(0.5f, 1.0f - 20.0f / 720.0f);
		button2->setOrigin(0.5f, 0.5f);
		button2->setPosition(0.0f, 0.0f);

		button3 = new Button(bg, 40, 40);
		button3->setSize(40.0f / 1280.0f, 40.0f / 720.0f);
		button3->setAnchor(1.0f, 0.0f);
		button3->setOrigin(1.0f, 0.0f);
		button3->setPosition(0.0f, 0.0f);

		lineEdit = new LineEdit(aspectRatioContainer, graphicsContext, 200, 40);
		lineEdit->setSize(200.0f / 1280.0f, 40.0f / 720.0f);
		lineEdit->setPosition(0.0f, -0.5f);
		lineEdit->setAnchor(0.5f, 0.85f);
		lineEdit->setOrigin(0.5f, 0.5f);
		lineEdit->setDefaultText("World name");

		gui->pushCanvas(canvas);
		addChildLast(gui);
	}

	void onEnd(GameEvent*)
	{
		delete button1;
		delete button2;
		delete aspectRatioContainer;
		delete button3;
		delete canvas;
		delete lineEdit;
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
		SceneObject::onDraw(e);

		// Draw debug info
		stringstream ss;
		ss << "FPS: " << getFPS() << "\n";
		ss << "Cursor position: " << getInputManager()->getPosition() << "\n";
		ss << "Canvas size: " << canvas->getSize().x << "x" << canvas->getSize().y << " (" << canvas->getAspectRatio() << ")\n";
		ss << "Aspect ratio container size: " << aspectRatioContainer->getDrawSize().x << "x" << aspectRatioContainer->getDrawSize().y << " (" << aspectRatioContainer->getAspectRatio() << ")\n";

		font->draw(spriteBatch, Vector2F(10.0f), ss.str());

		spriteBatch->end();
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	GuiGame game;
	return game.run();
}