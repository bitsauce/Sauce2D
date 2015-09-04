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

class GameManager
{
public:
	static FontPtr font;
	static SpriteBatch *spriteBatch;

	static void main(GraphicsContext &graphicsDevice)
	{
		font = ResourceManager::get<Font>(":/arial.fnt");
		spriteBatch = new SpriteBatch(graphicsDevice);
	}

	static void update(const float dt)
	{
	}

	static void draw(GraphicsContext &graphicsDevice, const float alpha)
	{
		spriteBatch->begin();
		font->draw(spriteBatch, graphicsDevice.getWidth() * 0.5f, (graphicsDevice.getHeight() - font->getHeight()) * 0.5f, "Hello World", FONT_ALIGN_CENTER);
		spriteBatch->end();
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
			case 'w': workDir = string(__argv[i]+3); break;
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
#ifdef X2D_DEBUG
	config.workDir = "..\\game\\";
#endif
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