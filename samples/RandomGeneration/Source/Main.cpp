#include <CGF/CGF.h>

using namespace cgf;

class RandomNoiseGPU : public Game
{
	Resource<Shader> m_noiseShader;
	Resource<RenderTarget2D> m_renderTarget;
	Resource<Font> m_font;
	float m_time, m_scale;
	bool m_showNoise;
	Vector2F m_position;
	SpriteBatch *m_spriteBatch;

public:
	RandomNoiseGPU() :
		Game("RandomNoiseGPU")
	{
	}

	void onStart(GameEvent *e)
	{
		m_noiseShader = getResourceManager()->get<Shader>("Generation");
		m_font = getResourceManager()->get<Font>("Font");
		m_renderTarget = Resource<RenderTarget2D>(new RenderTarget2D(getWindow()->getWidth(), getWindow()->getHeight()));
		m_spriteBatch = new SpriteBatch(getWindow()->getGraphicsContext());

		getWindow()->setVSync(0);

		m_scale = 1.0f;
		m_showNoise = false;

		Game::onStart(e);
	}

	void onEnd(GameEvent *e)
	{
		Game::onEnd(e);
	}

	void onTick(TickEvent *e)
	{
		m_time += e->getDelta();

		m_position.x += (getInputManager()->getKeyState(CGF_KEY_RIGHT) - getInputManager()->getKeyState(CGF_KEY_LEFT)) * 10.0f * ((getInputManager()->getKeyState(CGF_KEY_LSHIFT) || getInputManager()->getKeyState(CGF_KEY_RSHIFT)) ? 5.0f : 1.0f) * m_scale;
		m_position.y += (getInputManager()->getKeyState(CGF_KEY_DOWN) - getInputManager()->getKeyState(CGF_KEY_UP)) * 10.0f * ((getInputManager()->getKeyState(CGF_KEY_LSHIFT) || getInputManager()->getKeyState(CGF_KEY_RSHIFT)) ? 5.0f : 1.0f) * m_scale;

		float prevScale = m_scale;
		if(getInputManager()->getKeyState(CGF_KEY_PLUS))
		{
			m_scale /= 1.1;
			m_position -= Vector2F(getWindow()->getSize()) * 0.5f * (m_scale - prevScale);
		}
		else if(getInputManager()->getKeyState(CGF_KEY_MINUS))
		{
			m_scale *= 1.1;
			m_position -= Vector2F(getWindow()->getSize()) * 0.5f * (m_scale - prevScale);
		}

		m_showNoise = getInputManager()->getKeyState(CGF_KEY_SPACE);

		Game::onTick(e);
	}
	
	void onDraw(DrawEvent *e)
	{
		GraphicsContext *context = e->getGraphicsContext();

		m_noiseShader->setUniform1i("u_ShowNoise", m_showNoise);
		m_noiseShader->setUniform2f("u_Resolution", m_renderTarget->getWidth(), m_renderTarget->getHeight());
		m_noiseShader->setUniform1f("u_Scale", m_scale);
		m_noiseShader->setUniform2f("u_Position", m_position.x, m_position.y);

		context->setRenderTarget(m_renderTarget.get());
		context->setShader(m_noiseShader);
		context->drawRectangle(Vector2F(0, 0), getWindow()->getSize());
		context->setShader(0);
		context->setRenderTarget(0);

		context->setTexture(m_renderTarget->getTexture());
		context->drawRectangle(Vector2F(0, 0), getWindow()->getSize());
		context->setTexture(0);

		m_spriteBatch->begin();
		m_font->setColor(Color(255));

		stringstream ss;
		ss << "FPS: " << getFPS() << endl;
		ss << "Position: " << m_position.toString() << endl;
		ss << "Scale: " << m_scale << endl;

		m_font->draw(m_spriteBatch, 5, 5, ss.str());

		m_spriteBatch->end();

		Game::onDraw(e);
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	RandomNoiseGPU game;
	return game.run();
}