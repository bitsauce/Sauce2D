/* Main CrossGame header. Includes all the framework
   classes and various STL classes. */
#include <x2d/x2d.h>

/* This just saves us some time as we don't
   have to type cg::* in front of every CG
   class/enum/etc */
using namespace cg;

/* This is our MinimalGame class.
   It inherits from gc::Game, which is the game managing class.
   gc::Game contains a bunch of virtual event methods. These
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
	   but before the game has started */
	void onStart(GraphicsContext &graphicsContext)
	{
		LOG("Game started");
	}

	/* onEnd event. This event is called when the game is closing. */
	void onEnd()
	{
		LOG("Game ended");
	}

	/* onUpdate event. This event is called when the framework wants to update
	   the game state. */
	void onUpdate(const float dt)
	{
		LOG("Update");
	}

	/* onDraw event. This event is called then the framework wants to draw
	   the current game state to the screen */
	void onDraw(GraphicsContext &context, const float alpha)
	{
		LOG("Draw");
	}
};

/* Main entry point. This is where our program first starts executing. */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	/* In this example we simply create our game object
	   and call run() to start the game */
	MinimalGame game;
	return game.run();
}