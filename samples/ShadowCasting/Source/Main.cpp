#include <Sauce/Sauce.h>

using namespace sauce;

Random random;

// This sample implements 2D shadow casting based on this technique:
// http://www.catalinzima.com/2010/07/my-technique-for-the-shader-based-dynamic-2d-shadows/
class ShadowCastingGame : public Game
{
private:
	SpriteBatch *spriteBatch;
	Resource<Font> font;

	RenderTarget2D* castersRenderTarget;
	RenderTarget2D* distancesRenderTarget;
	RenderTarget2D* distortRenderTarget;
	RenderTarget2D** reductionRenderTargets;
	RenderTarget2D* shadowsRenderTarget;
	RenderTarget2D* blurHRenderTarget;
	RenderTarget2D* blurVRenderTarget;
	RenderTarget2D* resultRenderTarget;

	Resource<Texture2D> sceneTexture;
	Resource<Texture2D> tileTexture;
	Resource<Shader> calcDistanceShader;
	Resource<Shader> distortShader;
	Resource<Shader> reductionShader;
	Resource<Shader> shadowShader;
	Resource<Shader> blurShadowVShader;
	Resource<Shader> blurShadowHShader;
	int reductionCount;
	int baseSize;
	int lightMapSize;

	struct Light
	{
		Light()
		{
			randomize();
		}

		void randomize()
		{
			this->radius = random.nextInt(200, 1000);
			this->color = Color(random.nextInt(80, 255), random.nextInt(80, 255), random.nextInt(80, 255), 255);
		}

		Vector2F position;
		int radius;
		Color color;
	};

	Light *light;
	vector<Light*> lights;

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
		state(0),
		castersRenderTarget(0),
		distancesRenderTarget(0),
		distortRenderTarget(0),
		reductionRenderTargets(0),
		shadowsRenderTarget(0),
		blurHRenderTarget(0),
		blurVRenderTarget(0),
		resultRenderTarget(0),
		reductionCount(0)
	{
	}

	void onStart(GameEvent *e)
	{
		spriteBatch = new SpriteBatch(getWindow()->getGraphicsContext());

		font = getResourceManager()->get<Font>("Fonts/Debug_Font");

		sceneTexture = getResourceManager()->get<Texture2D>("Sprites/Scene");
		tileTexture = getResourceManager()->get<Texture2D>("Sprites/Tile");
		tileTexture->setWrapping(Texture2D::REPEAT);

		calcDistanceShader = getResourceManager()->get<Shader>("Shaders/Calculate_Distance");
		distortShader = getResourceManager()->get<Shader>("Shaders/Distort");
		reductionShader = getResourceManager()->get<Shader>("Shaders/Reduce");
		shadowShader = getResourceManager()->get<Shader>("Shaders/Draw_Shadows");
		blurShadowHShader = getResourceManager()->get<Shader>("Shaders/Blur_Shadows_H");
		blurShadowVShader = getResourceManager()->get<Shader>("Shaders/Blur_Shadows_V");

		light = new Light();
		lights.push_back(light);

		lightMapSize = Size512;
		setLightMapResolution(lightMapSize);

		Game::onStart(e);
	}

	void setLightMapResolution(const int size)
	{
		for(int i = 0; i < reductionCount; i++)
		{
			delete reductionRenderTargets[i];
		}

		reductionCount = size;
		baseSize = 2 << size;

		delete castersRenderTarget;
		delete distancesRenderTarget;
		delete distortRenderTarget;
		delete[] reductionRenderTargets;
		delete shadowsRenderTarget;
		delete blurHRenderTarget;
		delete blurVRenderTarget;
		delete resultRenderTarget;

		Vector2I windowSize = getWindow()->getSize();

		castersRenderTarget = new RenderTarget2D(baseSize, baseSize);
		distancesRenderTarget = new RenderTarget2D(baseSize, baseSize);
		distortRenderTarget = new RenderTarget2D(baseSize, baseSize);
		reductionRenderTargets = new RenderTarget2D*[reductionCount];
		for(int i = 0; i < reductionCount; i++)
		{
			reductionRenderTargets[i] = new RenderTarget2D(2 << i, baseSize);
		}
		shadowsRenderTarget = new RenderTarget2D(baseSize, baseSize);
		blurHRenderTarget = new RenderTarget2D(baseSize, baseSize);
		blurVRenderTarget = new RenderTarget2D(baseSize, baseSize);
		resultRenderTarget = new RenderTarget2D(windowSize.x, windowSize.y);
	}

	void onKeyEvent(KeyEvent *e)
	{
		if(e->getType() == KeyEvent::DOWN)
		{
			Keycode k = e->getKeycode();
			if(k >= SAUCE_KEY_0 && k <= SAUCE_KEY_9)
			{
				state = k - SAUCE_KEY_0;
			}

			if(e->getKeycode() == SAUCE_KEY_UP)
			{
				setLightMapResolution(++lightMapSize);
			}
			else if(e->getKeycode() == SAUCE_KEY_DOWN)
			{
				setLightMapResolution(--lightMapSize);
			}
		}

		if(e->getType() != KeyEvent::UP && e->getKeycode() == SAUCE_KEY_SPACE)
		{
			light->randomize();
		}
	}

	void onMouseMove(MouseEvent *e)
	{
		light->position = e->getPosition();
	}

	void onMouseDown(MouseEvent *e)
	{
		if(e->getButton() == SAUCE_MOUSE_BUTTON_LEFT)
		{
			light = new Light();
			light->position = e->getPosition();
			lights.push_back(light);
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

		graphicsContext->setRenderTarget(resultRenderTarget);
		graphicsContext->clear(GraphicsContext::COLOR_BUFFER, Color(0, 0, 0, 255));

		for(Light *l : lights)
		{
			graphicsContext->disable(GraphicsContext::BLEND);

			graphicsContext->setRenderTarget(castersRenderTarget);
			graphicsContext->setShader(0);
			graphicsContext->setTexture(sceneTexture);
			graphicsContext->drawRectangle(((Vector2F(l->radius * 0.5f) - l->position) / l->radius) * baseSize, sceneTexture->getSize() * baseSize / l->radius);

			graphicsContext->setRenderTarget(distancesRenderTarget);
			graphicsContext->setShader(calcDistanceShader);
			calcDistanceShader->setSampler2D("u_Texture", castersRenderTarget->getTexture());
			graphicsContext->drawRectangle(0, 0, baseSize, baseSize);

			graphicsContext->setRenderTarget(distortRenderTarget);
			graphicsContext->setShader(distortShader);
			distortShader->setSampler2D("u_Texture", distancesRenderTarget->getTexture());
			graphicsContext->drawRectangle(0, 0, baseSize, baseSize);

			Resource<Texture2D> reducedTexture = applyHorizontalReduction(graphicsContext, distortRenderTarget);

			graphicsContext->setRenderTarget(shadowsRenderTarget);
			graphicsContext->setShader(shadowShader);
			shadowShader->setSampler2D("u_Texture", reducedTexture);
			graphicsContext->drawRectangle(0, 0, baseSize, baseSize);

			graphicsContext->setRenderTarget(blurHRenderTarget);
			graphicsContext->setShader(blurShadowHShader);
			blurShadowHShader->setSampler2D("u_Texture", shadowsRenderTarget->getTexture());
			blurShadowHShader->setUniform1f("u_Width", baseSize);
			graphicsContext->drawRectangle(0, 0, baseSize, baseSize);

			graphicsContext->setRenderTarget(blurVRenderTarget);
			graphicsContext->setShader(blurShadowVShader);
			blurShadowVShader->setSampler2D("u_Texture", blurHRenderTarget->getTexture());
			blurShadowVShader->setUniform1f("u_Width", baseSize);
			graphicsContext->drawRectangle(0, 0, baseSize, baseSize);

			graphicsContext->enable(GraphicsContext::BLEND);

			graphicsContext->setRenderTarget(resultRenderTarget);
			graphicsContext->setShader(0);
			graphicsContext->setBlendState(BlendState::PRESET_ADDITIVE);
			graphicsContext->setTexture(blurVRenderTarget->getTexture());
			graphicsContext->drawRectangle(l->position - Vector2F(l->radius * 0.5f), Vector2F(l->radius), l->color);
		}

		graphicsContext->setTexture(0);
		graphicsContext->setRenderTarget(0);
		graphicsContext->enable(GraphicsContext::BLEND);
		graphicsContext->setBlendState(BlendState::PRESET_ALPHA_BLEND);

		Vector2F windowSize = getWindow()->getSize();

		// Draw tiles using repeat
		float u = (float) windowSize.x / tileTexture->getWidth(), v = (float) windowSize.y / tileTexture->getHeight();
		graphicsContext->setTexture(tileTexture);
		graphicsContext->drawRectangle(0, 0, windowSize.x, windowSize.y, Color(255), TextureRegion(0, 0, u, v));

		// Draw scene
		graphicsContext->setTexture(sceneTexture);
		graphicsContext->drawRectangle(0, 0, windowSize.x, windowSize.y);

		// Draw shadows
		graphicsContext->setRenderTarget(0);
		graphicsContext->setBlendState(BlendState::PRESET_MULTIPLY);
		graphicsContext->setTexture(resultRenderTarget->getTexture());
		graphicsContext->drawRectangle(0, 0, windowSize.x, windowSize.y);
		graphicsContext->setBlendState(BlendState::PRESET_ALPHA_BLEND);

		// Draw info
		stringstream ss;
		ss << "FPS: " << getFPS() << endl;
		ss << "Lights: " << lights.size() << endl;
		ss << "Light map resolution: " << baseSize << "x" << baseSize << endl;
		spriteBatch->begin();
		font->setColor(Color(255));
		font->draw(spriteBatch, Vector2F(5.0f), ss.str());
		spriteBatch->end();

		Game::onDraw(e);
	}

	Resource<Texture2D> applyHorizontalReduction(GraphicsContext *graphicsContext, RenderTarget2D *source)
	{
		int step = reductionCount - 1;
		RenderTarget2D *s = source;
		RenderTarget2D *d = reductionRenderTargets[step];
		graphicsContext->setShader(reductionShader);

		while(step >= 0)
		{
			d = reductionRenderTargets[step];

			graphicsContext->setRenderTarget(d);

			reductionShader->setSampler2D("u_Texture", s->getTexture());
			Vector2F textureDimentions(1.0f / (float) s->getWidth(), 1.0f / (float) s->getHeight());
			reductionShader->setUniform2f("u_TextureDimentions", textureDimentions.x, textureDimentions.y);

			graphicsContext->drawRectangle(0, 0, d->getWidth() + 1, baseSize);

			s = d;
			step--;
		}

		graphicsContext->setShader(nullptr);
		graphicsContext->setRenderTarget(nullptr);

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