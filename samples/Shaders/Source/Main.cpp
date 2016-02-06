
#include <CGF/CGF.h>

using namespace cgf;

class ShadersGame : public Game
{
	Resource<Shader> shader;
public:
	ShadersGame() :
		Game("Shaders")
	{
	}

	void onStart(GameEvent *e)
	{
		shader = getResourceManager()->get<Shader>("Shader");
		Game::onStart(e);
	}

	void onEnd(GameEvent *e)
	{
		Game::onEnd(e);
	}

	void onTick(TickEvent *e)
	{
		Game::onTick(e);
	}

	void onDraw(DrawEvent *e)
	{
		GraphicsContext *context = getWindow()->getGraphicsContext();
		context->setShader(shader);
		context->drawRectangle(0, 0, getWindow()->getWidth(), getWindow()->getHeight());
		context->setShader(0);
		Game::onDraw(e);
	}
};

/* Main entry point. This is where our program first starts executing. */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	/* To start our game we simply create our game object
	   and call run() */
	ShadersGame game;
	return game.run();
}