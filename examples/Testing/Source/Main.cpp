#include <CGF/cgf.h>

using namespace cgf;

// This tests:
// Loading texture from image file
// Saving image to file
// Rendering said image to the screen
class DrawTexturedQuad : public Game
{
	Texture2DPtr texture, textureSaved;
public:
	DrawTexturedQuad() :
		Game("DrawTexturedQuad")
	{
	}

	void onStart(GraphicsContext &context)
	{
		texture = ResourceManager::get<Texture2D>(":/Content/Texture.jpg");
		texture->exportToFile(":/Content/Texture_Saved.png");
		textureSaved = ResourceManager::get<Texture2D>(":/Content/Texture_Saved.png");
	}

	void onEnd()
	{
	}

	void onUpdate(const float dt)
	{
	}

	void onDraw(GraphicsContext &context, const float alpha)
	{
		context.setTexture(texture);
		context.drawRectangle(0, 0, context.getWidth() / 2, context.getHeight());
		context.setTexture(textureSaved);
		context.drawRectangle(context.getWidth() / 2, 0, context.getWidth() / 2, context.getHeight());
		context.setTexture(0);
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// DrawTexturedQuad
	{
		DrawTexturedQuad game;
		if(game.run() != CGF_OK) return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}