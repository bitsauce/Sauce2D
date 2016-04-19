#include <CGF/CGF.h>

using namespace cgf;

// This tests:
// Loading texture from image file
// Saving image to file
// Rendering said image to the screen
class DrawTexturedQuad : public Game
{
	Resource<Texture2D> texture, textureSaved;
	StaticVertexBuffer *vbo;
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

#define WORLD_HEIGHT 10
#define WORLD_WIDTH 10
#define TILE_WIDTH 120
#define TILE_HEIGHT 60

class Testing : public Game
{
	Resource<Texture2D> tileTexture, buildingTexture, unitATexture, unitBTexture;
	StaticVertexBuffer *tileVBO;
	StaticIndexBuffer *tileIBO;

public:
	Testing() :
		Game("Testing")
	{
		setFlags(CGF_RUN_IN_BACKGROUND);
	}

	void onStart(GameEvent *)
	{
		tileTexture = getResourceManager()->get<Texture2D>("Tile");
		buildingTexture = getResourceManager()->get<Texture2D>("Building");
		unitATexture = getResourceManager()->get<Texture2D>("UnitA");
		unitBTexture = getResourceManager()->get<Texture2D>("UnitB");

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

	void onTick(TickEvent *e)
	{

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

		// Draw building
		{
			int y = 3;
			int x = 3;

			float xf = x + (y % 2 == 0 ? 0.0f : 0.5f);
			float yf = y * 0.5f;
			context->setTexture(buildingTexture);
			context->drawRectangle(xf*TILE_WIDTH, yf*TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT * 2);
		}

		{
			int y = 3;
			int x = 5;

			float xf = x + (y % 2 == 0 ? 0.0f : 0.5f);
			float yf = y * 0.5f;
			context->setTexture(buildingTexture);
			context->drawRectangle(xf*TILE_WIDTH, yf*TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT * 2);
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

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// Testing
	{
		Testing game;
		if(game.run() != CGF_OK) return EXIT_FAILURE;
	}

	// DrawTexturedQuad
	{
		DrawTexturedQuad game;
		if(game.run() != CGF_OK) return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}