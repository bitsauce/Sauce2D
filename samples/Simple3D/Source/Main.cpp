#include <Sauce/Sauce.h>
#include "Camera.h"

using namespace sauce;

void drawRectangle(GraphicsContext *graphicsContext, const float x, const float y, const float width, const float height, const Color &color, const TextureRegion &textureRegion)
{
	VertexFormat format;
	format.set(VERTEX_POSITION, 3, SAUCE_FLOAT);
	format.set(VERTEX_COLOR, 4, SAUCE_UBYTE);
	//format.set(VERTEX_TEX_COORD, 2, SAUCE_FLOAT);

	Vertex *vertices = format.createVertices(4);

	vertices[0].set3f(VERTEX_POSITION, x, y,0);
	vertices[1].set3f(VERTEX_POSITION, x, y + height,0);
	vertices[2].set3f(VERTEX_POSITION, x + width, y,0);
	vertices[3].set3f(VERTEX_POSITION, x + width, y + height,0);

	vertices[0].set4ub(VERTEX_COLOR, color.getR(), color.getG(), color.getB(), color.getA());
	vertices[1].set4ub(VERTEX_COLOR, color.getR(), color.getG(), color.getB(), color.getA());
	vertices[2].set4ub(VERTEX_COLOR, color.getR(), color.getG(), color.getB(), color.getA());
	vertices[3].set4ub(VERTEX_COLOR, color.getR(), color.getG(), color.getB(), color.getA());

	//vertices[0].set2f(VERTEX_TEX_COORD, textureRegion.uv0.x, textureRegion.uv0.y);
	//vertices[1].set2f(VERTEX_TEX_COORD, textureRegion.uv0.x, textureRegion.uv1.y);
	//vertices[2].set2f(VERTEX_TEX_COORD, textureRegion.uv1.x, textureRegion.uv0.y);
	//vertices[3].set2f(VERTEX_TEX_COORD, textureRegion.uv1.x, textureRegion.uv1.y);

	graphicsContext->drawPrimitives(GraphicsContext::PRIMITIVE_TRIANGLE_STRIP, vertices, 4);
}

void drawCube(GraphicsContext* graphicsContext)
{
	VertexFormat format;
	format.set(VERTEX_POSITION, 3, SAUCE_FLOAT);
	format.set(VERTEX_COLOR, 4, SAUCE_UBYTE);
	//format.set(VERTEX_TEX_COORD, 2, SAUCE_FLOAT);
	
	Vertex *vertices = format.createVertices(8);

	float x = 0, y = 0, z = 0, w = 1, h = 1, d = 1;
	vertices[0].set3f(VERTEX_POSITION, x - w / 2, y - h / 2, z - d / 2);
	vertices[1].set3f(VERTEX_POSITION, x + w / 2, y - h / 2, z - d / 2);
	vertices[2].set3f(VERTEX_POSITION, x - w / 2, y + h / 2, z - d / 2);
	vertices[3].set3f(VERTEX_POSITION, x + w / 2, y + h / 2, z - d / 2);
	vertices[4].set3f(VERTEX_POSITION, x - w / 2, y - h / 2, z + d / 2);
	vertices[5].set3f(VERTEX_POSITION, x + w / 2, y - h / 2, z + d / 2);
	vertices[6].set3f(VERTEX_POSITION, x - w / 2, y + h / 2, z + d / 2);
	vertices[7].set3f(VERTEX_POSITION, x + w / 2, y + h / 2, z + d / 2);

	uchar r = 127, g = 127, b = 127, a = 255;
	for(int i = 0; i < 8; i++)
		vertices[i].set4ub(VERTEX_COLOR, r, g, b, a);

	uint indices[36];

	// Back
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 3; indices[4] = 2; indices[5] = 1;

	// Bottom
	indices[6] = 0; indices[7] = 1; indices[8] = 4;
	indices[9] = 4; indices[10] = 1; indices[11] = 5;

	// Left
	indices[12] = 0; indices[13] = 4; indices[14] = 6;
	indices[15] = 0; indices[16] = 6; indices[17] = 2;

	// Right
	indices[18] = 1; indices[19] = 5; indices[20] = 3;
	indices[21] = 5; indices[22] = 3; indices[23] = 7;

	// Top
	indices[24] = 2; indices[25] = 3; indices[26] = 6;
	indices[27] = 3; indices[28] = 6; indices[29] = 7;

	// Front
	indices[30] = 4; indices[31] = 5; indices[32] = 7;
	indices[33] = 4; indices[34] = 7; indices[35] = 6;

	// Draw triangles
	graphicsContext->drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, vertices, 8, indices, 36);

	delete[] vertices;
}

class Simple3DGame : public Game
{
	Camera camera;
	SpriteBatch *m_spriteBatch;
	Resource<Font> m_font;
	Resource<Shader> m_defaultShader;

public:
	Simple3DGame() :
		Game("Simple3D")
	{
	}

	void onStart(GameEvent *e)
	{
		m_spriteBatch = new SpriteBatch;
		m_defaultShader = Resource<Shader>("Shader/Default");
		m_font = Resource<Font>("Font/Arial");

		addChildLast(&camera);
		camera.setPosition(Vector3F(-2.0f, 0.0f, 0.0f));

		Game::onStart(e);
	}

	void onEnd(GameEvent *e)
	{
		Game::onEnd(e);
	}

	void onTick(TickEvent *e)
	{
		Game::onTick(e);
	}

	void onDraw(DrawEvent *e)
	{
		GraphicsContext *graphicsContext = e->getGraphicsContext();
		
		// Push 3D rendering state
		graphicsContext->pushState();
		graphicsContext->setShader(m_defaultShader);
		graphicsContext->pushMatrix(graphicsContext->createLookAtMatrix(camera.getPosition(), camera.getForwardVector() * -1));
		const float ar = float(getWindow()->getWidth()) / getWindow()->getHeight();
		graphicsContext->setProjectionMatrix(graphicsContext->createPerspectiveMatrix(45.0f, ar, 0.1f, 100.0f));
		drawCube(graphicsContext);
		graphicsContext->popState();

		// Draw 2D elements here
		//graphicsContext->drawText();

		stringstream ss;
		ss << "Position: " << camera.getPosition() << endl;
		ss << "Yaw: " << camera.getYaw() << endl;
		ss << "Pitch: " << camera.getPitch() << endl;

		m_spriteBatch->begin(e->getGraphicsContext());
		m_font->draw(m_spriteBatch, 10, 10, ss.str().c_str(), FONT_ALIGN_LEFT);
		m_spriteBatch->end();

		Game::onDraw(e);
	}
};

/* Main entry point. This is where our program first starts executing. */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	Simple3DGame game;
	return game.run();
}