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
	static Texture2DPtr texture;
	static ShaderPtr shader;

	static void main(GraphicsContext &graphicsContext)
	{
		font = ResourceManager::get<Font>(":/arial.fnt");
		spriteBatch = new SpriteBatch(graphicsContext);
		//texture = ResourceManager::get<Texture2D>(":/background.png");

		shader = ResourceManager::get<Shader>(":/TileMap");

		Pixmap pixmap(32, 32, PixelFormat(PixelFormat::RGBA, PixelFormat::UNSIGNED_INT));
		for (int i = 0; i < 32; ++i)
		{
			for (int j = 0; j < 32; ++j)
			{
				uint pixel[4];
				pixel[0] = pixel[1] = pixel[2] = (i + j * 32) % 256;
				pixel[3] = 255;
				pixmap.setPixel(i, j, pixel);
			}
		}

		texture = Texture2DPtr(new Texture2D(pixmap));
	}

	static void update(const float dt)
	{
	}

	static void draw(GraphicsContext &context, const float alpha)
	{
		shader->setSampler2D("u_tileMap", texture);
		context.setShader(shader);
		context.drawRectangle(Rect(0, 0, context.getWidth(), context.getHeight()));
		context.setShader(0);
	}

	static void exit()
	{
		delete spriteBatch;
	}
};

FontPtr GameManager::font;
SpriteBatch *GameManager::spriteBatch = 0;
Texture2DPtr GameManager::texture = 0;
ShaderPtr GameManager::shader = 0;

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