#include <Sauce/Sauce.h>

using namespace sauce;

class TextureAtlasGame : public Game
{
	TextureAtlas *textureAtlas;
public:
	TextureAtlasGame() :
		Game("TextureAtlas", SAUCE_WINDOW_RESIZABLE)
	{
	}

	void onStart(GameEvent*)
	{
		// Setup texture atlas
		textureAtlas = new TextureAtlas();

		// Here we load Image0, Image1, ..., until Image[i] is not a file.
		int i = 0;
		while(util::fileExists("Image" + util::intToStr(i) + ".png"))
		{
			textureAtlas->add("Image" + util::intToStr(i), Pixmap("Image" + util::intToStr(i) + ".png"));
			i++;
		}

		// Create a texture atlas using these images
		textureAtlas->create();

		// Save the atlas to Atlas.png (just to show how the altas it looks like)
		textureAtlas->getTexture()->exportToFile("Atlas.png");
	}

	void onEnd(GameEvent*)
	{
	}

	void onTick(TickEvent*)
	{
	}

	void onDraw(DrawEvent *e)
	{
		// Here we draw Image0 using the texture atlas
		GraphicsContext *graphicsContext = e->getGraphicsContext();
		graphicsContext->setTexture(textureAtlas->getTexture());
		graphicsContext->drawRectangle(0, 0, 120, 99, Color::White, textureAtlas->get("Image0"));
		graphicsContext->drawRectangle(240, 0, 120, 99, Color::White, textureAtlas->get("Image1"));
		graphicsContext->drawRectangle(500, 500, 120, 99, Color::White, textureAtlas->get("Image2"));
		graphicsContext->drawRectangle(640, 120, 120, 99, Color::White, textureAtlas->get("Image3"));
		graphicsContext->setTexture(0);
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	TextureAtlasGame game;
	return game.run();
}