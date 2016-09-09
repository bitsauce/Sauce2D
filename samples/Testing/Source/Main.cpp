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

		Vertex *vertices = context->getVertices(5);

		vertices[0].set2f(VERTEX_POSITION, 0.0f, 0.0f);
		vertices[0].set4ub(VERTEX_COLOR, 255, 255, 255, 255);

		float dir = 0.0f;
		float ang = PI / 4.0f;
		float rad = 500.0f;

		float f3 = dir - ang;
		float f4 = dir + ang;
		vertices[2].set2f(VERTEX_POSITION, cos(f3), sin(f3));
		vertices[2].set4ub(VERTEX_COLOR, 0, 0, 0, 0);
		vertices[3].set2f(VERTEX_POSITION, cos(f4), sin(f4));
		vertices[3].set4ub(VERTEX_COLOR, 0, 0, 0, 0);

		vertices[1].set2f(VERTEX_POSITION, cos(dir + PI) * 0.5f, sin(dir + PI) * 0.5f);
		vertices[1].set4ub(VERTEX_COLOR, 0, 0, 0, 0);
		vertices[4].set2f(VERTEX_POSITION, cos(dir + PI) * 0.5f, sin(dir + PI) * 0.5f);
		vertices[4].set4ub(VERTEX_COLOR, 0, 0, 0, 0);

		Matrix4 mat;
		mat.scale(rad);
		mat.translate(100, 500, 0.0);
		context->pushMatrix(mat);
		context->drawPrimitives(GraphicsContext::PRIMITIVE_TRIANGLE_FAN, &vertices[0], 5);
		context->popMatrix();
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