#include <x2d/x2d.h>
using namespace xd;

class MinimalGame : public Game
{
public:
	void main(GraphicsContext &graphicsContext)
	{
	}

	void update(const float dt)
	{
	}

	void draw(GraphicsContext &context, const float alpha)
	{
	}

	void end()
	{
	}
};

// Main entry point
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// Setup game
	MinimalGame game;
	game.setWorkDir("../../content/");

	// Create engine
	Engine *engine = CreateEngine();
	if(engine->init(&game) != X2D_OK)
	{
		delete engine;
		return -1;
	}

	int r = engine->run();
	delete engine;
	return r;
}