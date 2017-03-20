#include "Config.h"
#include "Gui/AspectRatioContainer.h"
#include "Gui/Button.h"
#include "Gui/LineEdit.h"
#include "Gui/Canvas.h"
#include "Gui/Gui.h"
#include "Gui/Background.h"
#include "Gui/DialogBoxCanvas.h"

// TODO: Expand this sample so it can:
// + Go from one menu to another (with crossfade)
// + Manage a menu stack.
//   Example: Main Menu -> Start Game -> Options -> Back -> Back -> Options
//   Should both show the same options menu
// + Test transparent canvases: Make it so that elements behind a transparent canvas are non-interactable
// + Pop-up messages
// o SpriteBatch should rather take a GraphicsContext in the constructor. Doing this lets us avoid passing graphics contexts everywhere.
// o Sliders (put them in the options menu. Maybe GUI scale and gamma?)
// o Animated buttons
// o Pop-up message background
// o Generalization of all the Background* classes (maybe just an BitmapBackground class or something)
// o Button sounds

class GuiGame : public Game
{
	SpriteBatch *spriteBatch;
	Canvas *canvasMain;
	Canvas *canvasOptions;
	Canvas *canvasDialog;
	Canvas *canvasDialog2;
	Button *buttonSingleplayer;
	Button *buttonMultiplayer;
	Button *buttonOptions;
	Button *buttonQuit;
	Button *buttonBack;
	LineEdit *lineEdit;
	AspectRatioContainer *aspectRatioContainer;
	Resource<Font> font;
	Gui *gui;
	bool showDebugInfo = false;

public:
	GuiGame() :
		Game("Gui", SAUCE_WINDOW_RESIZABLE)
	{
	}

	void onKeyDown(KeyEvent *e)
	{
		if(e->getKeycode() == SAUCE_KEY_F1)
		{
			showDebugInfo = !showDebugInfo;
		}
		Game::onKeyDown(e);
	}

	void onStart(GameEvent*)
	{
		font = Resource<Font>("Font");

		GraphicsContext *graphicsContext = getWindow()->getGraphicsContext();
		spriteBatch = new SpriteBatch(graphicsContext);

		gui = new Gui();

		canvasMain = new Canvas(getWindow());
		canvasOptions = new Canvas(getWindow());
		canvasDialog = new DialogBoxCanvas(gui, getWindow(), "Lorem ipsum dolor sit amet, consectetur adipiscing elit. In porttitor porttitor eros sed accumsan."
			"Cras eleifend purus non est gravida euismod. Nunc laoreet orci turpis, ac varius sapien aliquet non. Aliquam eu ex fringilla, finibus ligula sit amet, tempor enim.");
		canvasDialog2 = new DialogBoxCanvas(gui, getWindow(), "Singleplayer button clicked");

		Background *bg = new Background(canvasMain);
		Background *bg2 = new Background(canvasOptions);

		aspectRatioContainer = new AspectRatioContainer(bg, getWindow(), 1280, 1280.0f / 720.0f);
		aspectRatioContainer->setAnchor(0.5f, 0.5f);
		aspectRatioContainer->setOrigin(0.5f, 0.5f);

		// Singleplayer
		buttonSingleplayer = new Button(aspectRatioContainer, 230, 40);
		buttonSingleplayer->setText("Singleplayer");
		buttonSingleplayer->setSize(230.0f / 1280.0f, 40.0f / 720.0f);
		buttonSingleplayer->setAnchor(0.5f, 0.25f);
		buttonSingleplayer->setOrigin(0.5f, 0.5f);
		buttonSingleplayer->setPosition(0.0f, 0.0f);
		buttonSingleplayer->setOnClickCallback(bind(&Gui::pushCanvas, gui, canvasDialog2));

		// Multiplayer
		buttonMultiplayer = new Button(aspectRatioContainer, 230, 40);
		buttonMultiplayer->setText("Multiplayer");
		buttonMultiplayer->setSize(230.0f / 1280.0f, 40.0f / 720.0f);
		buttonMultiplayer->setAnchor(0.5f, 0.25f);
		buttonMultiplayer->setOrigin(0.5f, 0.5f);
		buttonMultiplayer->setPosition(0.0f, (40.0f + 10.0f) / 720.0f);
		buttonMultiplayer->setOnClickCallback(bind(&Gui::pushCanvas, gui, canvasDialog));

		// Options
		buttonOptions = new Button(aspectRatioContainer, 230, 40);
		buttonOptions->setText("Options");
		buttonOptions->setSize(230.0f / 1280.0f, 40.0f / 720.0f);
		buttonOptions->setAnchor(0.5f, 0.25f);
		buttonOptions->setOrigin(0.5f, 0.5f);
		buttonOptions->setPosition(0.0f, (40.0f + 10.0f) / 720.0f * 2);
		buttonOptions->setOnClickCallback(bind(&Gui::pushCanvas, gui, canvasOptions));

		// Quit
		buttonQuit = new Button(aspectRatioContainer, 230, 40);
		buttonQuit->setText("Quit");
		buttonQuit->setSize(230.0f / 1280.0f, 40.0f / 720.0f);
		buttonQuit->setAnchor(0.5f, 0.25f);
		buttonQuit->setOrigin(0.5f, 0.5f);
		buttonQuit->setPosition(0.0f, (40.0f + 10.0f) / 720.0f * 3);
		buttonQuit->setOnClickCallback(bind(&Game::end, this));

		buttonBack = new Button(bg2, 40, 40);
		buttonBack->setText("Back");
		buttonBack->setSize(40.0f / 1280.0f, 40.0f / 720.0f);
		buttonBack->setAnchor(1.0f, 0.0f);
		buttonBack->setOrigin(1.0f, 0.0f);
		buttonBack->setPosition(0.0f, 0.0f);
		buttonBack->setOnClickCallback(bind(&Gui::popCanvas, gui));
		/*
		lineEdit = new LineEdit(aspectRatioContainer, graphicsContext, 200, 40);
		lineEdit->setSize(200.0f / 1280.0f, 40.0f / 720.0f);
		lineEdit->setPosition(0.0f, -0.5f);
		lineEdit->setAnchor(0.5f, 0.85f);
		lineEdit->setOrigin(0.5f, 0.5f);
		lineEdit->setDefaultText("World name");
		*/
		gui->pushCanvas(canvasMain);
		addChildLast(gui);
	}

	void onEnd(GameEvent*)
	{
		delete buttonSingleplayer;
		delete buttonOptions;
		delete aspectRatioContainer;
		delete buttonBack;
		delete canvasMain;
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

		if(showDebugInfo)
		{
			// Draw debug info
			stringstream ss;
			ss << "FPS: " << getFPS() << "\n";
			ss << "Cursor position: " << getInputManager()->getPosition() << "\n";
			ss << "Canvas size: " << canvasMain->getSize().x << "x" << canvasMain->getSize().y << " (" << canvasMain->getAspectRatio() << ")\n";
			ss << "Aspect ratio container size: " << aspectRatioContainer->getDrawSize().x << "x" << aspectRatioContainer->getDrawSize().y << " (" << aspectRatioContainer->getAspectRatio() << ")\n";
			font->draw(spriteBatch, Vector2F(10.0f), ss.str());
		}
		spriteBatch->end();
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	GuiGame game;
	return game.run();
}