#include <Sauce/Sauce.h>

using namespace sauce;

class RandomNoiseGPU : public Game
{
	Resource<Shader> m_noiseShader;
	Resource<Texture2D> m_gradientTexture;
	shared_ptr<RenderTarget2D> m_renderTarget;
	float m_time;

public:
	RandomNoiseGPU() :
		Game("RandomNoiseGPU", SAUCE_EXPORT_LOG)
	{
	}

	void onStart(GameEvent *e)
	{
		m_noiseShader = Resource<Shader>("Fractal2D");
		//m_noiseShader = Resource<Shader>("Fractal2D_Gradient");
		//m_noiseShader = Resource<Shader>("Voronoise");
		m_gradientTexture = Resource<Texture2D>("Gradient");
		m_renderTarget = shared_ptr<RenderTarget2D>(new RenderTarget2D(getWindow()->getWidth(), getWindow()->getHeight()));

		m_noiseShader->setUniform1f("u_Frequency", 0.5f);
		m_noiseShader->setUniform1f("u_Gain", 0.5f);
		m_noiseShader->setUniform1f("u_Lacunarity", 2.0f);
		m_noiseShader->setUniform1i("u_Octaves", 8);
		m_noiseShader->setSampler2D("u_Gradient", m_gradientTexture);

		Game::onStart(e);
	}

	void onEnd(GameEvent *e)
	{
		Game::onEnd(e);
	}

	void onTick(TickEvent *e)
	{
		m_time += e->getDelta();
		Game::onTick(e);
	}
	
	void onDraw(DrawEvent *e)
	{
		GraphicsContext *context = e->getGraphicsContext();

		m_noiseShader->setUniform1f("u_Time", m_time + e->getAlpha() * 1.0f / 30.0f);

		context->setRenderTarget(m_renderTarget.get());
		context->setShader(m_noiseShader);
		context->drawRectangle(Vector2F(0, 0), getWindow()->getSize());
		context->setShader(0);
		context->setRenderTarget(0);

		context->setTexture(m_renderTarget->getTexture());
		context->drawRectangle(Vector2F(0, 0), getWindow()->getSize());
		context->setTexture(0);

		if(getInputManager()->getKeyState(SAUCE_KEY_SPACE))
		{
			Pixmap pixmap = m_renderTarget->getTexture()->getPixmap();
			uchar data[4];
			uint histogram[256] = { 0 };
			for(int y = 0; y < pixmap.getHeight(); y++)
			{
				for(int x = 0; x < pixmap.getWidth(); x++)
				{
					pixmap.getPixel(x, y, data);
					histogram[data[0]]++;
				}
			}
			context->setLineWidth(2);

			uint maxValue = 0;
			for(int i = 0; i < 256; i++)
			{
				maxValue = max(histogram[i], maxValue);
			}

			Vertex vertices[256];
			for(int i = 0; i < 256; i++)
			{
				vertices[i].set2f(VERTEX_POSITION, i, 256 - (histogram[i] / float(maxValue)) * 256);
				vertices[i].set4ub(VERTEX_COLOR, 255, 0, 0, 255);
			}
			context->drawRectangle(0, 0, 256, 256, Color(0, 0, 0, 200));
			context->drawPrimitives(GraphicsContext::PRIMITIVE_LINE_STRIP, vertices, 256);
		}

		Game::onDraw(e);
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	RandomNoiseGPU game;
	return game.run();
}