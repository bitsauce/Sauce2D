#include <x2d/x2d.h>
using namespace xd;

class NoiseGame : public Game
{
public:
	void main(GraphicsContext &graphicsContext)
	{
		m_shader = ResourceManager::get<Shader>(":/Shader/Simplex3D.frag");
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
	NoiseGame game;
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