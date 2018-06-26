/* Include the SauceEngine framework */
#include <Sauce/Sauce.h>
using namespace sauce;

class MinimalGame : public Game
{
public:
	/** 
	 * Game constructor.
	 * This method is generally empty since
	 * initialization of resources need to happen in onStart()
	 */
	MinimalGame() :
		/* Title of the game. Used as window title and name of folder containing save files */
		Game("Minimal")
	{
	}

	/** 
	 * onStart() event.
	 * This event called once the framework has initialized, but before the first tick.
	 * Load resources and setup the window here.
	 */
	void onStart(GameEvent *e)
	{
		Game::onStart(e);
	}

	/**
	 * onEnd() event.
	 * This event is called when the game is closing.
	 * Free all resources and save game state here.
	 */
	void onEnd(GameEvent *e)
	{
		Game::onEnd(e);
	}

	/**
	 * onTick() event.
	 * This event is called when the framework should update the game state.
	 * Things like physics and other simulations should go here.
	 */
	void onTick(TickEvent *e)
	{
		Game::onTick(e);
	}

	/**
	 * onDraw event.
	 * This event is called then the framework should draw the current game state to the screen.
	 * Using the graphics context provided by e->getGraphicsContext() is generally how we draw to the screen. 
	 */
	void onDraw(DrawEvent *e)
	{
		Game::onDraw(e);
	}
};

/* Main entry point. This is where our program first starts executing. */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	/** 
	 * To start the game we simply create a game object and call run().
	 * This function should typically not require modification.
	 */
	MinimalGame game;
	return game.run();
}