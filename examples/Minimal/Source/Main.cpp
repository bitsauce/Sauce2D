#include <x2d/x2d.h>
using namespace xd;

class MinimalGame : public Game
{
public:
	void main(GraphicsContext &graphicsContext)
	{
		m_font = ResourceManager::get<Font>(":/font/arial.fnt");
		m_spriteBatch = new SpriteBatch(graphicsContext);
	}

	void update(const float dt)
	{
	}

	void draw(GraphicsContext &context, const float alpha)
	{
	}

	void end()
	{
		delete m_spriteBatch;
	}

private:
	FontPtr m_font;
	SpriteBatch *m_spriteBatch;
};

// Win32 entry point
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