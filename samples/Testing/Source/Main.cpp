#include <Sauce/Sauce.h>

using namespace sauce;

// This tests:
// Loading texture from image file
// Saving image to file
// Rendering said image to the screen
class DrawTexturedQuad : public Game
{
	Resource<Texture2D> texture;
	shared_ptr<Texture2D> textureSaved;
	StaticVertexBuffer *vbo;
public:
	DrawTexturedQuad() :
		Game("DrawTexturedQuad")
	{
	}

	void onStart(GameEvent *e)
	{
		texture = Resource<Texture2D>("Koala");
		texture->exportToFile(":/Content/Texture_Saved.png");
		textureSaved = shared_ptr<Texture2D>(new Texture2D(Pixmap("Texture_Saved.png")));
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

#define WORLD_HEIGHT 10
#define WORLD_WIDTH 10
#define TILE_WIDTH 120
#define TILE_HEIGHT 60

class Testing : public Game
{
	Resource<Texture2D> tileTexture, buildingTexture, unitATexture, unitBTexture;
	StaticVertexBuffer *tileVBO;
	StaticIndexBuffer *tileIBO;
	
	RenderTarget2D *buildingsRT;
	RenderTarget2D *unitsRT;

	Resource<Shader> buildingsShader, unitShader;

	bool showBuildings;
	bool showUnits;
	bool showTiles;

public:
	Testing() :
		Game("Testing"),
		showBuildings(true),
		showUnits(true),
		showTiles(true)
	{
		setFlags(SAUCE_RUN_IN_BACKGROUND);
	}

	void onStart(GameEvent *)
	{
	}

	void onWindowSizeChanged(WindowEvent *e)
	{
	}

	void onTick(TickEvent *e)
	{

	}

	void onKeyEvent(KeyEvent *e)
	{
	}
	
	void onDraw(DrawEvent *e)
	{
		GraphicsContext *context = e->getGraphicsContext();

		const uint segments = 4;

		Vertex *vertices = context->getVertices(segments + 2);

		float x = 100, y = 500;

		vertices[0].set2f(VERTEX_POSITION, x, y);
		vertices[0].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
		vertices[0].set2f(VERTEX_TEX_COORD, 0.5f, 0.5f);

		float angle = 0.0f;

		float f = angle - PI / 4.0f;

		for(uint i = 1; i < segments + 2; ++i)
		{
			vertices[i].set2f(VERTEX_POSITION, x + cos(f) * 500, y + sin(f) * 500);
			vertices[i].set4ub(VERTEX_COLOR, 0, 0, 0, 0);
			vertices[i].set2f(VERTEX_TEX_COORD, (1.0f + cos(f)) / 2.0f, (1.0f + sin(f)) / 2.0f);

			f += (PI / 2.0f) / (segments + 2);
		}

		context->drawPrimitives(GraphicsContext::PRIMITIVE_TRIANGLE_FAN, &vertices[0], segments + 2);
	}
};


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{

	// DrawTexturedQuad
	/*{
		DrawTexturedQuad game;
		if(game.run() != SAUCE_OK) return EXIT_FAILURE;
	}*/

	{
		Testing game;
		if(game.run() != SAUCE_OK) return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}