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

/*
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
		getWindow()->setSize(1280, 720);

		tileTexture = getResourceManager()->get<Texture2D>("Tile");
		buildingTexture = getResourceManager()->get<Texture2D>("Building");
		unitATexture = getResourceManager()->get<Texture2D>("UnitA");
		unitBTexture = getResourceManager()->get<Texture2D>("UnitB");

		buildingsShader = getResourceManager()->get<Shader>("BuildingsShader");
		unitShader = getResourceManager()->get<Shader>("UnitShader");

		Vertex *vertices = new Vertex[WORLD_WIDTH * WORLD_HEIGHT * 4];
		uint *indices = new uint[WORLD_WIDTH * WORLD_HEIGHT * 6];
		for(int i = 0; i < WORLD_HEIGHT * WORLD_WIDTH; i++)
		{
			int y = i / WORLD_HEIGHT;
			int x = i - y * WORLD_HEIGHT;

			float xf = x + (y % 2 == 0 ? 0.0f : 0.5f);
			float yf = y * 0.5f;

			vertices[i * 4 + 0].set2f(VERTEX_POSITION, xf,     yf);
			vertices[i * 4 + 1].set2f(VERTEX_POSITION, xf + 1, yf);
			vertices[i * 4 + 2].set2f(VERTEX_POSITION, xf,     yf + 1);
			vertices[i * 4 + 3].set2f(VERTEX_POSITION, xf + 1, yf + 1);

			vertices[i * 4 + 0].set2f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[i * 4 + 1].set2f(VERTEX_TEX_COORD, 1.0f, 0.0f);
			vertices[i * 4 + 2].set2f(VERTEX_TEX_COORD, 0.0f, 1.0f);
			vertices[i * 4 + 3].set2f(VERTEX_TEX_COORD, 1.0f, 1.0f);

			vertices[i * 4 + 0].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
			vertices[i * 4 + 1].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
			vertices[i * 4 + 2].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
			vertices[i * 4 + 3].set4ub(VERTEX_COLOR, 255, 255, 255, 255);

			indices[i * 6 + 0] = i * 4 + 0;
			indices[i * 6 + 1] = i * 4 + 2;
			indices[i * 6 + 2] = i * 4 + 1;
			indices[i * 6 + 3] = i * 4 + 1;
			indices[i * 6 + 4] = i * 4 + 2;
			indices[i * 6 + 5] = i * 4 + 3;
		}

		tileVBO = new StaticVertexBuffer(vertices, WORLD_WIDTH * WORLD_HEIGHT * 4);
		tileIBO = new StaticIndexBuffer(indices, WORLD_WIDTH * WORLD_HEIGHT * 6);
	}

	void onWindowSizeChanged(WindowEvent *e)
	{
		buildingsRT = new RenderTarget2D(e->getWidth(), e->getHeight());
		unitsRT = new RenderTarget2D(e->getWidth(), e->getHeight());
	}

	void onTick(TickEvent *e)
	{

	}

	void onKeyEvent(KeyEvent *e)
	{
		if(e->getType() != KeyEvent::DOWN) return;
		if(e->getScancode() == SAUCE_SCANCODE_1) showBuildings = !showBuildings;
		else if(e->getScancode() == SAUCE_SCANCODE_2) showUnits = !showUnits;
		else if(e->getScancode() == SAUCE_SCANCODE_3) showTiles = !showTiles;
	}

	void drawBuildings(GraphicsContext *context)
	{
		context->setRenderTarget(buildingsRT);

		if(getInputManager()->getKeyState(SAUCE_SCANCODE_Q)) context->setShader(buildingsShader);

		// Draw buildings
		{
			int y = 3;
			int x = 3;

			float xf = x + (y % 2 == 0 ? 0.0f : 0.5f);
			float yf = y * 0.5f;
			context->setTexture(buildingTexture);
			buildingsShader->setSampler2D("u_Texture", buildingTexture);
			context->drawRectangle(xf * TILE_WIDTH, yf * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT * 2);
		}

		{
			int y = 3;
			int x = 5;

			float xf = x + (y % 2 == 0 ? 0.0f : 0.5f);
			float yf = y * 0.5f;
			context->setTexture(buildingTexture);
			context->drawRectangle(xf * TILE_WIDTH, yf * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT * 2);
		}

		context->setShader(0);

		context->setRenderTarget(0);

		context->setTexture(buildingsRT->getTexture());
		context->drawRectangle(0, 0, buildingsRT->getWidth(), buildingsRT->getHeight());
	}

	void onDraw(DrawEvent *e)
	{
		GraphicsContext *context = e->getGraphicsContext();

		// Draw tiles
		Matrix4 tileMatrix;
		tileMatrix.scale(TILE_WIDTH, TILE_HEIGHT, 1.0f);
		context->pushMatrix(tileMatrix);
		context->setTexture(tileTexture);
		context->drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, tileVBO, tileIBO);
		context->popMatrix();

		if(showBuildings)
		{
			drawBuildings(e->getGraphicsContext());
		}

		// Draw units
		{
			int y = 1;
			int x = 3;

			float xf = x + (y % 2 == 0 ? 0.0f : 0.5f);
			float yf = y * 0.5f;
			context->setTexture(unitATexture);
			context->drawRectangle(xf*TILE_WIDTH, yf*TILE_HEIGHT, TILE_WIDTH*3, TILE_HEIGHT * 4);
		}


		{
			int y = 1;
			int x = 3;

			float xf = x + (y % 2 == 0 ? 0.0f : 0.5f);
			float yf = y * 0.5f;
			context->setTexture(unitBTexture);
			context->drawRectangle(xf*TILE_WIDTH, yf*TILE_HEIGHT, TILE_WIDTH * 3, TILE_HEIGHT * 4);
		}
	}
};
*/

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{

	// DrawTexturedQuad
	{
		DrawTexturedQuad game;
		if(game.run() != SAUCE_OK) return EXIT_FAILURE;
	}

	// Isometric
	/*{
		Testing game;
		if(game.run() != SAUCE_OK) return EXIT_FAILURE;
	}*/

	return EXIT_SUCCESS;
}