/* Main CrossGame header. Includes all the framework
   classes and various STL classes. */
#include <Sauce/Sauce.h>

/* This just saves us some time as we don't
   have to type cg::* in front of every CG
   class/enum/etc */
using namespace sauce;

/* This is our MinimalGame class.
   It inherits from cgf::Game, which is the game managing class.
   cgf::Game contains a bunch of virtual event methods. These
   methods start with on* and are invoked at special times. 
   For instance, onUpdate is called whenever the game needs to
   update its game state, and onKeyDown is called whenever a
   key on the keyboard is pressed.*/
class MinimalGame : public Game
{
public:
	/* Consturctor for our game class */
	MinimalGame() :
		/* cg::Game class takes a string which is the game name. */
		Game("Minimal")
	{
	}

	/* onStart event. This event called once the framework has initialized,
	   but before the game loop has started */
	void onStart(GameEvent *e)
	{
		LOG("onStart");
		Game::onStart(e);
	}

	/* onEnd event. This event is called when the game is closing. */
	void onEnd(GameEvent *e)
	{
		LOG("onEnd");
		Game::onEnd(e);
	}

	/* onTick event. This event is called when the framework wants to update
	   the game state. */
	void onTick(TickEvent *e)
	{
		LOG("onTick");
		Game::onTick(e);
	}

	/* onDraw event. This event is called then the framework wants to draw
	   the current game state to the screen */
	void onDraw(DrawEvent *e)
	{
		LOG("onDraw");
		Game::onDraw(e);
	}
};

/* Main entry point. This is where our program first starts executing. */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	/* To start our game we simply create our game object
	   and call run() */
	MinimalGame game;
	return game.run();
}