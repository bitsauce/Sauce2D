#include <Sauce/Sauce.h>
#include "Camera.h"

using namespace sauce;

void drawCube(GraphicsContext* graphicsContext, const float x, const float y, const float z, const float w, const float h, const float d)
{
	VertexFormat format;
	format.set(VERTEX_POSITION, 3, SAUCE_FLOAT);
	format.set(VERTEX_COLOR, 4, SAUCE_UBYTE);
	format.set(VERTEX_TEX_COORD, 2, SAUCE_FLOAT);
	
	Vertex *vertices = format.createVertices(8);

	vertices[0].set3f(VERTEX_POSITION, x - w / 2, y - h / 2, z - d / 2);
	vertices[1].set3f(VERTEX_POSITION, x + w / 2, y - h / 2, z - d / 2);
	vertices[2].set3f(VERTEX_POSITION, x - w / 2, y + h / 2, z - d / 2);
	vertices[3].set3f(VERTEX_POSITION, x + w / 2, y + h / 2, z - d / 2);
	vertices[4].set3f(VERTEX_POSITION, x - w / 2, y - h / 2, z + d / 2);
	vertices[5].set3f(VERTEX_POSITION, x + w / 2, y - h / 2, z + d / 2);
	vertices[6].set3f(VERTEX_POSITION, x - w / 2, y + h / 2, z + d / 2);
	vertices[7].set3f(VERTEX_POSITION, x + w / 2, y + h / 2, z + d / 2);

	/*uchar r = 127, g = 127, b = 127, a = 255;
	for(int i = 0; i < 8; i++)
		vertices[i].set4ub(VERTEX_COLOR, r, g, b, a);*/

	vertices[0].set4ub(VERTEX_COLOR, 255, 0, 0, 255);
	vertices[1].set4ub(VERTEX_COLOR, 0, 255, 0, 255);
	vertices[2].set4ub(VERTEX_COLOR, 0, 0, 255, 255);
	vertices[3].set4ub(VERTEX_COLOR, 255, 255, 0, 255);
	vertices[4].set4ub(VERTEX_COLOR, 255, 0, 255, 255);
	vertices[5].set4ub(VERTEX_COLOR, 0, 255, 255, 255);
	vertices[6].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
	vertices[7].set4ub(VERTEX_COLOR, 127, 127, 127, 255);

	vertices[0].set2f(VERTEX_TEX_COORD, 0, 0);
	vertices[1].set2f(VERTEX_TEX_COORD, 1, 0);
	vertices[2].set2f(VERTEX_TEX_COORD, 0, 1);
	vertices[3].set2f(VERTEX_TEX_COORD, 1, 1);
	vertices[4].set2f(VERTEX_TEX_COORD, 0, 0);
	vertices[5].set2f(VERTEX_TEX_COORD, 1, 0);
	vertices[6].set2f(VERTEX_TEX_COORD, 0, 1);
	vertices[7].set2f(VERTEX_TEX_COORD, 1, 1);

	uint indices[36];

	// Back
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 3; indices[4] = 2; indices[5] = 1;

	// Bottom
	indices[6] = 4; indices[7] = 1; indices[8] = 0;
	indices[9] = 5; indices[10] = 1; indices[11] = 4;

	// Left
	indices[12] = 6; indices[13] = 4; indices[14] = 0;
	indices[15] = 2; indices[16] = 6; indices[17] = 0;

	// Right
	indices[18] = 1; indices[19] = 5; indices[20] = 3;
	indices[21] = 7; indices[22] = 3; indices[23] = 5;

	// Top
	indices[24] = 2; indices[25] = 3; indices[26] = 6;
	indices[27] = 7; indices[28] = 6; indices[29] = 3;

	// Front
	indices[30] = 7; indices[31] = 5; indices[32] = 4;
	indices[33] = 6; indices[34] = 7; indices[35] = 4;

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
	Resource<Texture2D> m_texture;

public:
	Simple3DGame() :
		Game("Simple3D")
	{
	}

	void onStart(GameEvent *e)
	{
		m_spriteBatch = new SpriteBatch;
		m_defaultShader = Resource<Shader>("Shader/Default");
		m_texture = Resource<Texture2D>("Texture/Sample");
		m_texture->setWrapping(Texture2D::REPEAT);
		m_font = Resource<Font>("Font/Arial");

		addChildLast(&camera);
		camera.setPosition(Vector3F(0.0f, 0.0f, 2.0f));
		camera.setYaw(-math::degToRad(90));

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
		
		m_defaultShader->setSampler2D("u_Texture", m_texture);
		graphicsContext->setShader(m_defaultShader);

		graphicsContext->pushMatrix(graphicsContext->createLookAtMatrix(camera.getDrawPosition(e->getAlpha()), camera.getForwardVector() * -1));
		const float ar = float(getWindow()->getWidth()) / getWindow()->getHeight();
		graphicsContext->setProjectionMatrix(graphicsContext->createPerspectiveMatrix(45.0f, ar, 0.1f, 100.0f));
		drawCube(graphicsContext, 0, 0, 0, 1, 1, 1);
		graphicsContext->popState();

		// Draw 2D elements here
		stringstream ss;
		ss << "Position: " << camera.getPosition() << endl;
		ss << "Yaw: " << camera.getYaw() << endl;
		ss << "Pitch: " << camera.getPitch() << endl;

		graphicsContext->disable(GraphicsContext::DEPTH_TEST);
		m_spriteBatch->begin(e->getGraphicsContext());
		m_font->draw(m_spriteBatch, 10, 10, ss.str().c_str(), FONT_ALIGN_LEFT);
		m_spriteBatch->end();
		graphicsContext->enable(GraphicsContext::DEPTH_TEST);

		Game::onDraw(e);
	}
};

/* Main entry point. This is where our program first starts executing. */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	Simple3DGame game;
	return game.run();
}