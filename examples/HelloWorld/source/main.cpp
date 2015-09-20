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

class GameManager
{
public:
	static FontPtr font;
	static SpriteBatch *spriteBatch;

	static void main(GraphicsContext &graphicsContext)
	{
		font = ResourceManager::get<Font>(":/arial.fnt");
		spriteBatch = new SpriteBatch(graphicsContext);
		Input::bind(XD_KEY_1, toggleVSync);
		Input::bind(XD_KEY_2, toggleFullscreen);
	}

	static void toggleVSync()
	{
		Graphics::setVsync(!Graphics::getVsync());
	}

	static void toggleFullscreen()
	{
		Window::setFullScreen(!Window::getFullScreen());
	}

	static void update(const double dt)
	{
		xPrev = x;
		x += 5.0f;
		if(x > Window::getWidth())
		{
			x -= Window::getWidth();
		}
	}

	static void draw(GraphicsContext &context, const double alpha)
	{
		spriteBatch->begin();
		font->draw(spriteBatch, 0, 0, "FPS: " + util::floatToStr(Graphics::getFPS()) + "\nVSync: " + (Graphics::getVsync() == 1 ? "ON" : "OFF") + " (press 1 to toggle)\nFullscreen: " + (Window::getFullScreen() ? "ON" : "OFF") + " (press 2 to toggle)");
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
	// Process the command-line
	int flags = 0;
	string workDir;
	for(int i = 0; i < __argc; i++)
	{
		if(__argv[i][0] == '-')
		{
			switch(__argv[i][1])
			{
				case 'v': flags |= XD_EXPORT_LOG; break;
				case 'w': workDir = string(__argv[i] + 3); break;
			}
		}
	}
	flags |= XD_EXPORT_LOG; // For now we force this flag

	Engine *engine = CreateEngine();

	Config config;
	config.mainFunc = &GameManager::main;
	config.updateFunc = &GameManager::update;
	config.drawFunc = &GameManager::draw;
	config.endFunc = &GameManager::exit;
	//#ifdef X2D_DEBUG
	config.workDir = "..\\game\\";
	//#endif
	config.flags = flags;

	if(engine->init(config) != X2D_OK)
	{
		delete engine;
		return -1;
	}

	int r = engine->run();
	delete engine;
	return r;
}