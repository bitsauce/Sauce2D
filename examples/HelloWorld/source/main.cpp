//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/x2d.h>

using namespace xd;

float x = 0, xPrev = 0;
bool vsync = true;

class GameManager
{
public:
	static FontPtr font;
	static SpriteBatch *spriteBatch;

	static void main(GraphicsContext &graphicsContext)
	{
		font = ResourceManager::get<Font>(":/font/arial.fnt");
		spriteBatch = new SpriteBatch(graphicsContext);
		Input::bind(XD_KEY_1, toggleVSync);
		Input::bind(XD_KEY_2, toggleFullscreen);
		Graphics::enableVsync();
	}

	static void toggleVSync()
	{
		if(vsync)
			Graphics::disableVsync();
		else
			Graphics::enableVsync();
		vsync = !vsync;
	}

	static void toggleFullscreen()
	{
		if(Window::isFullscreen())
			Window::disableFullscreen();
		else
			Window::enableFullscreen();
	}

	static void update(const float dt)
	{
		xPrev = x;
		x += 5.0f;
		if(x > Window::getWidth())
		{
			xPrev = x = -32.0f;
			x += 5.0f;
		}
	}

	static void draw(GraphicsContext &context, const float alpha)
	{
		spriteBatch->begin();
		font->draw(spriteBatch, 0, 0, "FPS: " + util::floatToStr(Graphics::getFPS()) + "\nVSync: " + (vsync ? "ON" : "OFF") + " (press 1 to toggle)\nFullscreen: " + (Window::isFullscreen() ? "ON" : "OFF") + " (press 2 to toggle)");
		spriteBatch->end();
		context.drawRectangle(Rect(math::lerp(xPrev, x, alpha), context.getHeight() * 0.5f, 32.0f, 32.0f));
	}

	static void exit()
	{
		delete spriteBatch;
	}
};

FontPtr GameManager::font;
SpriteBatch *GameManager::spriteBatch = 0;

// Win32 entry point
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// Create engine
	Engine *engine = CreateEngine();

	Config config;
	config.mainFunc = &GameManager::main;
	config.updateFunc = &GameManager::update;
	config.drawFunc = &GameManager::draw;
	config.endFunc = &GameManager::exit;
	config.flags = XD_EXPORT_LOG;

	if(engine->init(config) != X2D_OK)
	{
		delete engine;
		return -1;
	}

	int r = engine->run();
	delete engine;
	return r;
}