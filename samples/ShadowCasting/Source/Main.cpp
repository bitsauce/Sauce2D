#include <Sauce/Sauce.h>

using namespace sauce;

// This sample implements 2D shadow casting based on this technique:
// http://www.catalinzima.com/2010/07/my-technique-for-the-shader-based-dynamic-2d-shadows/
class ShadowCastingGame : public Game
{
private:
	RenderTarget2D* castersRenderTarget;
	RenderTarget2D* distancesRenderTarget;
	RenderTarget2D* distortRenderTarget;
	RenderTarget2D** reductionRenderTargets;
	RenderTarget2D* shadowsRenderTarget;

	RenderTarget2D* blurHRenderTarget;

	RenderTarget2D* blurVRenderTarget;

	Resource<Texture2D> sceneTexture;
	Resource<Texture2D> tileTexture;
	Resource<Shader> calcDistanceShader;
	Resource<Shader> distortShader;
	Resource<Shader> reductionShader;
	Resource<Shader> shadowShader;
	Resource<Shader> blurShadowVShader;
	Resource<Shader> blurShadowHShader;
	int reductionChainCount;
	int baseSize;
	int depthBufferSize;

	int state;
	
	enum ShadowmapSize
	{
		Size128 = 6,
		Size256 = 7,
		Size512 = 8,
		Size1024 = 9,
		Size2048 = 10,
		Size4192 = 11
	};

public:
	ShadowCastingGame() :
		Game("ShadowCasting"),
		state(0)
	{
	}

	void onStart(GameEvent *e)
	{
		int size = Size512;
		reductionChainCount = size;
		baseSize = 2 << reductionChainCount;
		depthBufferSize = 2 << size;

		castersRenderTarget = new RenderTarget2D(baseSize, baseSize);
		distancesRenderTarget = new RenderTarget2D(baseSize, baseSize);
		distortRenderTarget = new RenderTarget2D(baseSize, baseSize);
		reductionRenderTargets = new RenderTarget2D*[reductionChainCount];
		for(int i = 0; i < reductionChainCount; i++)
		{
			reductionRenderTargets[i] = new RenderTarget2D(2 << i, baseSize);
		}
		shadowsRenderTarget = new RenderTarget2D(baseSize, baseSize);
		blurHRenderTarget = new RenderTarget2D(baseSize, baseSize);
		blurVRenderTarget = new RenderTarget2D(baseSize, baseSize);

		sceneTexture = getResourceManager()->get<Texture2D>("Sprites/Scene");
		tileTexture = getResourceManager()->get<Texture2D>("Sprites/Tile");
		tileTexture->setWrapping(Texture2D::REPEAT);

		calcDistanceShader = getResourceManager()->get<Shader>("Shaders/Calculate_Distance");
		distortShader = getResourceManager()->get<Shader>("Shaders/Distort");
		reductionShader = getResourceManager()->get<Shader>("Shaders/Reduce");
		shadowShader = getResourceManager()->get<Shader>("Shaders/Draw_Shadows");
		blurShadowHShader = getResourceManager()->get<Shader>("Shaders/Blur_Shadows_H");
		blurShadowVShader = getResourceManager()->get<Shader>("Shaders/Blur_Shadows_V");

		Game::onStart(e);
	}

	void onKeyDown(KeyEvent *e)
	{
		Keycode k = e->getKeycode();
		if(k >= SAUCE_KEY_0 && k <= SAUCE_KEY_9)
		{
			state = k - SAUCE_KEY_0;
		}
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

		graphicsContext->setRenderTarget(castersRenderTarget);
		graphicsContext->clear(GraphicsContext::COLOR_BUFFER);
		graphicsContext->setTexture(sceneTexture);
		graphicsContext->drawRectangle(((Vector2F(512 * 0.5f) - getInputManager()->getPosition()) / 512.0f) * baseSize, Vector2F(baseSize));

		graphicsContext->setRenderTarget(distancesRenderTarget);
		graphicsContext->clear(GraphicsContext::COLOR_BUFFER, Color(255));

		graphicsContext->setShader(calcDistanceShader);
		calcDistanceShader->setSampler2D("u_Texture", castersRenderTarget->getTexture());
		graphicsContext->drawRectangle(0, 0, baseSize, baseSize);

		graphicsContext->setRenderTarget(distortRenderTarget);
		graphicsContext->clear(GraphicsContext::COLOR_BUFFER);

		graphicsContext->setShader(distortShader);
		distortShader->setSampler2D("u_Texture", distancesRenderTarget->getTexture());
		graphicsContext->drawRectangle(0, 0, baseSize, baseSize);

		Resource<Texture2D> reducedTexture = applyHorizontalReduction(graphicsContext, distortRenderTarget);
		graphicsContext->setRenderTarget(shadowsRenderTarget);
		graphicsContext->clear(GraphicsContext::COLOR_BUFFER);

		graphicsContext->setShader(shadowShader);
		shadowShader->setSampler2D("u_Texture", reducedTexture);
		graphicsContext->drawRectangle(0, 0, baseSize, baseSize);

		graphicsContext->setRenderTarget(blurHRenderTarget);
		graphicsContext->clear(GraphicsContext::COLOR_BUFFER);

		graphicsContext->setShader(blurShadowHShader);
		blurShadowHShader->setSampler2D("u_Texture", shadowsRenderTarget->getTexture());
		blurShadowHShader->setUniform1f("u_Width", baseSize);
		graphicsContext->drawRectangle(0, 0, baseSize, baseSize);

		graphicsContext->setRenderTarget(blurVRenderTarget);
		graphicsContext->clear(GraphicsContext::COLOR_BUFFER);

		graphicsContext->setShader(blurShadowVShader);
		blurShadowVShader->setSampler2D("u_Texture", blurHRenderTarget->getTexture());
		blurShadowVShader->setUniform1f("u_Width", baseSize);
		graphicsContext->drawRectangle(0, 0, baseSize, baseSize);

		graphicsContext->setRenderTarget(nullptr);
		graphicsContext->setShader(nullptr);

		switch(state)
		{
			case 0:
			{
				// Draw tiles using repeat
				{
					Vector2F windowSize = getWindow()->getSize();
					float u = (float) windowSize.x / tileTexture->getWidth(), v = (float) windowSize.y / tileTexture->getHeight();
					graphicsContext->setTexture(tileTexture);
					graphicsContext->drawRectangle(0, 0, windowSize.x, windowSize.y, Color(255), TextureRegion(0, 0, u, v));
				}

				// Draw scene
				graphicsContext->setTexture(sceneTexture);
				graphicsContext->drawRectangle(0, 0, 512, 512);

				graphicsContext->setBlendState(BlendState::PRESET_MULTIPLY);
				graphicsContext->setTexture(blurVRenderTarget->getTexture());
				graphicsContext->drawRectangle(getInputManager()->getPosition() - Vector2F(512 * 0.5f), Vector2F(512));
				graphicsContext->setBlendState(BlendState::PRESET_ALPHA_BLEND);
			}
			break;

			case 1:
			{
				graphicsContext->setTexture(distancesRenderTarget->getTexture());
				graphicsContext->drawRectangle(0, 0, 512, 512);
			}
			break;

			case 2:
			{
				graphicsContext->setTexture(distortRenderTarget->getTexture());
				graphicsContext->drawRectangle(0, 0, 512, 512);
			}
			break;

			case 3:
			{
				graphicsContext->setTexture(shadowsRenderTarget->getTexture());
				graphicsContext->drawRectangle(0, 0, 512, 512);
			}
			break;

			case 4:
			{
				graphicsContext->setTexture(castersRenderTarget->getTexture());
				graphicsContext->drawRectangle(0, 0, 512, 512);
			}
			break;

			case 5:
			{
				graphicsContext->setTexture(blurHRenderTarget->getTexture());
				graphicsContext->drawRectangle(0, 0, 512, 512);
			}
			break;

			case 6:
			{
				graphicsContext->setTexture(blurVRenderTarget->getTexture());
				graphicsContext->drawRectangle(0, 0, 512, 512);
			}
			break;
		}
		graphicsContext->setTexture(0);
		graphicsContext->drawCircle(Vector2F(baseSize * 0.5f), 10, 10, Color(255));

		Game::onDraw(e);
	}

	Resource<Texture2D> applyHorizontalReduction(GraphicsContext *graphicsContext, RenderTarget2D *source)
	{
		int step = reductionChainCount - 1;
		RenderTarget2D *s = source;
		RenderTarget2D *d = reductionRenderTargets[step];
		graphicsContext->setShader(reductionShader);

		while(step >= 0)
		{
			d = reductionRenderTargets[step];

			graphicsContext->setRenderTarget(d);
			graphicsContext->clear(GraphicsContext::COLOR_BUFFER, Color(255));

			reductionShader->setSampler2D("u_Texture", s->getTexture());
			Vector2F textureDimentions(1.0f / (float) s->getWidth(), 1.0f / (float) s->getHeight());
			reductionShader->setUniform2f("u_TextureDimentions", textureDimentions.x, textureDimentions.y);

			graphicsContext->drawRectangle(0, 0, d->getWidth() + 1, baseSize);

			s = d;
			step--;
		}

		graphicsContext->setShader(nullptr);
		graphicsContext->setRenderTarget(nullptr);

		//copy to destination
		return d->getTexture();
	}
};

/* Main entry point. This is where our program first starts executing. */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	/* To start our game we simply create our game object
	   and call run() */
	ShadowCastingGame game;
	return game.run();
}