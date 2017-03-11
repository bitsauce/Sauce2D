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

		context->drawRectangleOutline(100, 100, 100, 100, Color(0, 0, 0, 255));
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
	RenderTarget2D *tileColorRenderTarget, *tileDepthRenderTarget;
	Resource<Shader> tileDepthShader;

	StaticVertexBuffer *buildingVBO;
	StaticIndexBuffer *buildingIBO;
	RenderTarget2D *buildingColorRenderTarget, *buildingDepthRenderTarget;
	//Resource<Shader> buildingDepthShader;

	StaticVertexBuffer *unitVBO;
	StaticIndexBuffer *unitIBO;
	RenderTarget2D *unitColorRenderTarget, *unitDepthRenderTarget;
	Resource<Shader> unitDepthShader;

	Resource<Shader> finalShader;

	bool showBuildings;
	bool showUnits;
	bool showTiles;

public:
	Testing() :
		Game("Testing", SAUCE_RUN_IN_BACKGROUND),
		showBuildings(true),
		showUnits(true),
		showTiles(true)
	{
	}

	void onStart(GameEvent *)
	{
		getWindow()->setSize(1280, 720);

		tileTexture = Resource<Texture2D>("Tile");
		buildingTexture = Resource<Texture2D>("Building");
		unitATexture = Resource<Texture2D>("UnitA");
		unitBTexture = Resource<Texture2D>("UnitB");

		buildingColorRenderTarget = new RenderTarget2D(WORLD_WIDTH * TILE_WIDTH, WORLD_HEIGHT * TILE_HEIGHT);
		buildingDepthRenderTarget = new RenderTarget2D(WORLD_WIDTH * TILE_WIDTH, WORLD_HEIGHT * TILE_HEIGHT);
		//buildingDepthShader = Resource<Shader>("BuildingDepth");

		tileColorRenderTarget = new RenderTarget2D(WORLD_WIDTH * TILE_WIDTH, WORLD_HEIGHT * TILE_HEIGHT);
		tileDepthRenderTarget = new RenderTarget2D(WORLD_WIDTH * TILE_WIDTH, WORLD_HEIGHT * TILE_HEIGHT);
		tileDepthShader = Resource<Shader>("TileDepth");

		unitColorRenderTarget = new RenderTarget2D(WORLD_WIDTH * TILE_WIDTH, WORLD_HEIGHT * TILE_HEIGHT);
		unitDepthRenderTarget = new RenderTarget2D(WORLD_WIDTH * TILE_WIDTH, WORLD_HEIGHT * TILE_HEIGHT);
		unitDepthShader = Resource<Shader>("UnitDepth");

		finalShader = Resource<Shader>("FinalShader");

		VertexFormat fmt;
		fmt.set(VERTEX_POSITION, 3);
		fmt.set(VERTEX_TEX_COORD, 2);
		fmt.set(VERTEX_COLOR, 4, SAUCE_UBYTE);

		Vertex *vertices = fmt.createVertices(WORLD_WIDTH * WORLD_HEIGHT * 4);
		uint *indices = new uint[WORLD_WIDTH * WORLD_HEIGHT * 6];
		for(int i = 0; i < WORLD_HEIGHT * WORLD_WIDTH; i++)
		{
			int y = i / WORLD_HEIGHT;
			int x = i - y * WORLD_HEIGHT;

			// Get tile coordinate
			Vector3F origin = Vector3F((x - y) * TILE_WIDTH / 2 - TILE_WIDTH, (x + y) * math::ceil(TILE_HEIGHT / 2.0f), 0);

			vertices[i * 4 + 0].set3f(VERTEX_POSITION, origin.x, origin.y, (x + y) / 18.0f);
			vertices[i * 4 + 1].set3f(VERTEX_POSITION, TILE_WIDTH*2 + origin.x, origin.y, (x + y) / 18.0f);
			vertices[i * 4 + 3].set3f(VERTEX_POSITION, TILE_WIDTH*2 + origin.x, TILE_HEIGHT*2 + origin.y, (x + y) / 18.0f);
			vertices[i * 4 + 2].set3f(VERTEX_POSITION, origin.x, TILE_HEIGHT*2 + origin.y, (x + y) / 18.0f);

			if((x + y) % 2 == 0)
			{
				vertices[i * 4 + 0].set2f(VERTEX_TEX_COORD, 0.0f, 0.0f);
				vertices[i * 4 + 1].set2f(VERTEX_TEX_COORD, 0.5f, 0.0f);
				vertices[i * 4 + 2].set2f(VERTEX_TEX_COORD, 0.0f, 1.0f);
				vertices[i * 4 + 3].set2f(VERTEX_TEX_COORD, 0.5f, 1.0f);
			}
			else
			{
				vertices[i * 4 + 0].set2f(VERTEX_TEX_COORD, 0.5f, 0.0f);
				vertices[i * 4 + 1].set2f(VERTEX_TEX_COORD, 1.0f, 0.0f);
				vertices[i * 4 + 2].set2f(VERTEX_TEX_COORD, 0.5f, 1.0f);
				vertices[i * 4 + 3].set2f(VERTEX_TEX_COORD, 1.0f, 1.0f);
			}

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

		//delete[] vertices;
		//delete[] indices;

		int buildingCount = 0;
		vertices = fmt.createVertices(2 * 4);
		indices = new uint[2 * 6];

		{
			int y = 5;
			int x = 3 - 1;

			Vector2F origin = Vector2F((x - y) * TILE_WIDTH / 2, (x + y) * math::ceil(TILE_HEIGHT / 2.0f));

			vertices[buildingCount * 4 + 0].set3f(VERTEX_POSITION, origin.x, origin.y, (x + y + 1) / 18.0f);
			vertices[buildingCount * 4 + 1].set3f(VERTEX_POSITION, TILE_WIDTH + origin.x, origin.y, (x + y + 1) / 18.0f);
			vertices[buildingCount * 4 + 3].set3f(VERTEX_POSITION, TILE_WIDTH + origin.x, TILE_HEIGHT * 2 + origin.y, (x + y + 1) / 18.0f);
			vertices[buildingCount * 4 + 2].set3f(VERTEX_POSITION, origin.x, TILE_HEIGHT * 2 + origin.y, (x + y + 1) / 18.0f);

			vertices[buildingCount * 4 + 0].set2f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[buildingCount * 4 + 1].set2f(VERTEX_TEX_COORD, 1.0f, 0.0f);
			vertices[buildingCount * 4 + 2].set2f(VERTEX_TEX_COORD, 0.0f, 1.0f);
			vertices[buildingCount * 4 + 3].set2f(VERTEX_TEX_COORD, 1.0f, 1.0f);

			vertices[buildingCount * 4 + 0].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
			vertices[buildingCount * 4 + 1].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
			vertices[buildingCount * 4 + 2].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
			vertices[buildingCount * 4 + 3].set4ub(VERTEX_COLOR, 255, 255, 255, 255);

			indices[buildingCount * 6 + 0] = buildingCount * 4 + 0;
			indices[buildingCount * 6 + 1] = buildingCount * 4 + 2;
			indices[buildingCount * 6 + 2] = buildingCount * 4 + 1;
			indices[buildingCount * 6 + 3] = buildingCount * 4 + 1;
			indices[buildingCount * 6 + 4] = buildingCount * 4 + 2;
			indices[buildingCount * 6 + 5] = buildingCount * 4 + 3;

			buildingCount++;
		}

		{
			int y = 3;
			int x = 5 - 1;

			Vector2F origin = Vector2F((x - y) * TILE_WIDTH / 2, (x + y) * math::ceil(TILE_HEIGHT / 2.0f));

			vertices[buildingCount * 4 + 0].set3f(VERTEX_POSITION, origin.x, origin.y, (x + y + 1) / 18.0f);
			vertices[buildingCount * 4 + 1].set3f(VERTEX_POSITION, TILE_WIDTH + origin.x, origin.y, (x + y + 1) / 18.0f);
			vertices[buildingCount * 4 + 3].set3f(VERTEX_POSITION, TILE_WIDTH + origin.x, TILE_HEIGHT * 2 + origin.y, (x + y + 1) / 18.0f);
			vertices[buildingCount * 4 + 2].set3f(VERTEX_POSITION, origin.x, TILE_HEIGHT * 2 + origin.y, (x + y + 1) / 18.0f);

			vertices[buildingCount * 4 + 0].set2f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[buildingCount * 4 + 1].set2f(VERTEX_TEX_COORD, 1.0f, 0.0f);
			vertices[buildingCount * 4 + 2].set2f(VERTEX_TEX_COORD, 0.0f, 1.0f);
			vertices[buildingCount * 4 + 3].set2f(VERTEX_TEX_COORD, 1.0f, 1.0f);

			vertices[buildingCount * 4 + 0].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
			vertices[buildingCount * 4 + 1].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
			vertices[buildingCount * 4 + 2].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
			vertices[buildingCount * 4 + 3].set4ub(VERTEX_COLOR, 255, 255, 255, 255);

			indices[buildingCount * 6 + 0] = buildingCount * 4 + 0;
			indices[buildingCount * 6 + 1] = buildingCount * 4 + 2;
			indices[buildingCount * 6 + 2] = buildingCount * 4 + 1;
			indices[buildingCount * 6 + 3] = buildingCount * 4 + 1;
			indices[buildingCount * 6 + 4] = buildingCount * 4 + 2;
			indices[buildingCount * 6 + 5] = buildingCount * 4 + 3;

			buildingCount++;
		}

		buildingVBO = new StaticVertexBuffer(vertices, buildingCount * 4);
		buildingIBO = new StaticIndexBuffer(indices, buildingCount * 6);

		//delete[] vertices;
		//delete[] indices;

		int unitCount = 0;
		vertices = fmt.createVertices(1 * 4);
		indices = new uint[1 * 6];

		{
			int y = 4;
			int x = 2 - 1;

			Vector2F origin = Vector2F((x - y) * TILE_WIDTH / 2, (x + y) * math::ceil(TILE_HEIGHT / 2.0f));

			vertices[unitCount * 4 + 0].set3f(VERTEX_POSITION, origin.x, origin.y, (x + y - 0) / 18.0f);
			vertices[unitCount * 4 + 1].set3f(VERTEX_POSITION, TILE_WIDTH * 3 + origin.x, origin.y, (x + y - 0) / 18.0f);
			vertices[unitCount * 4 + 3].set3f(VERTEX_POSITION, TILE_WIDTH * 3 + origin.x, TILE_HEIGHT * 4 + origin.y, (x + y + 6) / 18.0f);
			vertices[unitCount * 4 + 2].set3f(VERTEX_POSITION, origin.x, TILE_HEIGHT * 4 + origin.y, (x + y + 6) / 18.0f);

			vertices[unitCount * 4 + 0].set2f(VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[unitCount * 4 + 1].set2f(VERTEX_TEX_COORD, 1.0f, 0.0f);
			vertices[unitCount * 4 + 2].set2f(VERTEX_TEX_COORD, 0.0f, 1.0f);
			vertices[unitCount * 4 + 3].set2f(VERTEX_TEX_COORD, 1.0f, 1.0f);

			vertices[unitCount * 4 + 0].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
			vertices[unitCount * 4 + 1].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
			vertices[unitCount * 4 + 2].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
			vertices[unitCount * 4 + 3].set4ub(VERTEX_COLOR, 255, 255, 255, 255);

			indices[unitCount * 6 + 0] = unitCount * 4 + 0;
			indices[unitCount * 6 + 1] = unitCount * 4 + 2;
			indices[unitCount * 6 + 2] = unitCount * 4 + 1;
			indices[unitCount * 6 + 3] = unitCount * 4 + 1;
			indices[unitCount * 6 + 4] = unitCount * 4 + 2;
			indices[unitCount * 6 + 5] = unitCount * 4 + 3;

			unitCount++;
		}

		unitVBO = new StaticVertexBuffer(vertices, unitCount * 4);
		unitIBO = new StaticIndexBuffer(indices, unitCount * 6);
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

	void onDraw(DrawEvent *e)
	{
		GraphicsContext *context = e->getGraphicsContext();

		Matrix4 mat;
		mat.translate(5.25f * TILE_WIDTH, 0.0f, 0.0f);

		// Quick note: You might be able to draw both the color and the depth
		// in one pass using multiple render targets

		// Draw tile depth
		context->setRenderTarget(tileDepthRenderTarget);
		context->pushMatrix(mat);
		context->setShader(tileDepthShader);
		tileDepthShader->setSampler2D("u_Texture", tileTexture);
		context->drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, tileVBO, tileIBO);
		context->setShader(0);
		context->popMatrix();
		context->setRenderTarget(0);

		// Draw tile color
		context->setRenderTarget(tileColorRenderTarget);
		context->pushMatrix(mat);
		context->setTexture(tileTexture);
		context->drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, tileVBO, tileIBO);
		context->popMatrix();
		context->setRenderTarget(0);

		// Draw building depth
		context->setRenderTarget(buildingDepthRenderTarget);
		context->pushMatrix(mat);
		context->setShader(tileDepthShader);
		tileDepthShader->setSampler2D("u_Texture", buildingTexture);
		context->drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, buildingVBO, buildingIBO);
		context->setShader(0);
		context->popMatrix();
		context->setRenderTarget(0);

		// Draw building color
		context->setRenderTarget(buildingColorRenderTarget);
		context->pushMatrix(mat);
		context->setTexture(buildingTexture);
		context->drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, buildingVBO, buildingIBO);
		context->popMatrix();
		context->setRenderTarget(0);

		// Draw unit depth
		context->setRenderTarget(unitDepthRenderTarget);
		context->pushMatrix(mat);
		context->setShader(unitDepthShader);
		unitDepthShader->setSampler2D("u_Texture", unitATexture);
		unitDepthShader->setSampler2D("u_TileDepthTexture", tileDepthRenderTarget->getTexture());
		context->drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, unitVBO, unitIBO);
		context->setShader(0);
		context->popMatrix();
		context->setRenderTarget(0);

		// Draw unit color
		context->setRenderTarget(unitColorRenderTarget);
		context->pushMatrix(mat);
		context->setTexture(unitATexture);
		context->drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, unitVBO, unitIBO);
		context->popMatrix();
		context->setRenderTarget(0);


		if(getInputManager()->getKeyState(SAUCE_KEY_K))
		{
			context->setTexture(unitDepthRenderTarget->getTexture());
			context->drawRectangle(0, 0, unitDepthRenderTarget->getTexture()->getWidth(), unitDepthRenderTarget->getTexture()->getHeight());
		}
		else
		{

			// Draw final
			context->setShader(finalShader);
			finalShader->setSampler2D("u_TileColorTexture", tileColorRenderTarget->getTexture());
			finalShader->setSampler2D("u_TileDepthTexture", tileDepthRenderTarget->getTexture());
			finalShader->setSampler2D("u_BuildingColorTexture", buildingColorRenderTarget->getTexture());
			finalShader->setSampler2D("u_BuildingDepthTexture", buildingDepthRenderTarget->getTexture());
			finalShader->setSampler2D("u_UnitColorTexture", unitColorRenderTarget->getTexture());
			finalShader->setSampler2D("u_UnitDepthTexture", unitDepthRenderTarget->getTexture());
			context->drawRectangle(0, 0, WORLD_WIDTH * TILE_WIDTH, WORLD_HEIGHT * TILE_HEIGHT);
			context->setShader(0);
		}
	}
};

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