#include <CGF/CGF.h>

using namespace cgf;

// This tests:
// Loading texture from image file
// Saving image to file
// Rendering said image to the screen
class DrawTexturedQuad : public Game
{
	Resource<Texture2D> texture, textureSaved;
public:
	DrawTexturedQuad() :
		Game("DrawTexturedQuad")
	{
	}

	void onStart(GameEvent *e)
	{
		texture = getResourceManager()->get<Texture2D>("Koala");
		texture->exportToFile(":/Content/Texture_Saved.png");
		textureSaved = Resource<Texture2D>(new Texture2D(Pixmap("Texture_Saved.png")));
	}

	void onEnd(GameEvent *e)
	{
	}

	void onTick(TickEvent *e)
	{
	}

	void onDraw(DrawEvent *e)
	{
		GraphicsContext *context = e->getGraphicsContext();
		context->setTexture(texture);
		context->drawRectangle(0, 0, context->getWidth() / 2, context->getHeight());
		context->setTexture(textureSaved);
		context->drawRectangle(context->getWidth() / 2, 0, context->getWidth() / 2, context->getHeight());
		context->setTexture(0);
	}
};

class Testing : public Game
{
	float x, prevX;
	bool move;

public:
	Testing() :
		Game("Testing"),
		x(0.0f),
		prevX(0.0f),
		move(false)
	{
		setFlags(CGF_RUN_IN_BACKGROUND);
	}

	void onStart(GameEvent *)
	{
		getWindow()->setSize(800, 600);
	}

	void onKeyDown(KeyEvent *e)
	{
		move = !move;
	}

	void onTick(TickEvent *e)
	{
		if(move)
		{
			prevX = x;
			x += 10.0f;
			if(x > getWindow()->getWidth())
			{
				prevX = x = x - getWindow()->getWidth();
			}
		}
	}

	void onDraw(DrawEvent *e)
	{
		GraphicsContext *context = e->getGraphicsContext();
		context->drawRectangle(math::lerp(prevX, x, e->getAlpha()) - 50, context->getHeight() * 0.5f - 50, 100, 100, Color(255));
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// Testing
	{
		Testing game;
		if(game.run() != CGF_OK) return EXIT_FAILURE;
	}

	// DrawTexturedQuad
	/*{
		DrawTexturedQuad game;
		if(game.run() != CGF_OK) return EXIT_FAILURE;
	}*/

	return EXIT_SUCCESS;
}