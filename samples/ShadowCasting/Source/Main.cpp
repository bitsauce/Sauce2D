#include <Sauce/Sauce.h>

using namespace sauce;

Random random;

// This sample implements 2D shadow casting based on this technique:
// http://www.catalinzima.com/2010/07/my-technique-for-the-shader-based-dynamic-2d-shadows/
// https://github.com/mattdesl/lwjgl-basics/wiki/2D-Pixel-Perfect-Shadows
class ShadowCastingGame : public Game
{
private:
	SpriteBatch *m_spriteBatch;
	Resource<Font> m_font;

	Resource<Texture2D> m_sceneTexture;
	Resource<Texture2D> m_tileTexture;

	RenderTarget2D* m_occludersRenderTarget;
	RenderTarget2D* m_shadowMapRenderTarget;
	RenderTarget2D* m_shadowsRenderTarget;

	Resource<Shader> m_shadowMapShader;
	Resource<Shader> m_shadowRenderShader;

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

	Light *m_currentLight;
	list<Light*> m_lights;

	int m_lightMapSize;

	enum DebugState 
	{
		DEBUG_STATE_NONE,
		SHOW_OCCLUDER_MAP,
		SHOW_SHADOW_MAP,
		SHOW_SHADOW_RENDER,
		DEBUG_STATE_COUNT
	};
	DebugState m_debugState;

public:
	ShadowCastingGame() :
		Game("ShadowCasting"),
		m_spriteBatch(0),
		m_occludersRenderTarget(0),
		m_debugState(DEBUG_STATE_NONE)
	{
	}

	void onStart(GameEvent *e)
	{
		// Create sprite batch
		m_spriteBatch = new SpriteBatch();

		// Load font
		m_font = Resource<Font>("Fonts/Debug_Font");
		m_font->setColor(Color::White);

		// Load sprites
		m_sceneTexture = Resource<Texture2D>("Sprites/Scene");
		m_tileTexture = Resource<Texture2D>("Sprites/Tile");
		m_tileTexture->setWrapping(Texture2D::REPEAT);

		// Load shaders
		m_shadowMapShader = Resource<Shader>("Shaders/Shadow_Map");
		m_shadowRenderShader = Resource<Shader>("Shaders/Shadow_Render");

		// Create light object
		m_currentLight = new Light();
		m_lights.push_back(m_currentLight);

		// Set light map resolution
		setLightMapResolution(256);

		Game::onStart(e);
	}

	void setLightMapResolution(const int size)
	{
		Vector2I viewSize = getWindow()->getSize();
		
		delete m_occludersRenderTarget;
		delete m_shadowMapRenderTarget;
		delete m_shadowsRenderTarget;

		GraphicsContext *graphicsContext = getWindow()->getGraphicsContext();
		m_occludersRenderTarget = graphicsContext->createRenderTarget(size, size);
		m_shadowMapRenderTarget = graphicsContext->createRenderTarget(size, 1);
		m_shadowsRenderTarget   = graphicsContext->createRenderTarget(viewSize.x, viewSize.y);
		m_shadowMapRenderTarget->getTexture()->setWrapping(Texture2D::REPEAT);

		m_lightMapSize = size;
	}

	void onKeyEvent(KeyEvent *e)
	{
		if(e->getType() == KeyEvent::DOWN)
		{
			switch(e->getKeycode())
			{
				case SAUCE_KEY_LEFT: m_debugState = (DebugState) math::mod(m_debugState - 1, DEBUG_STATE_COUNT); break;
				case SAUCE_KEY_RIGHT: m_debugState = (DebugState) math::mod(m_debugState + 1, DEBUG_STATE_COUNT); break;
				case SAUCE_KEY_UP: setLightMapResolution(m_lightMapSize << 1); break;
				case SAUCE_KEY_DOWN: setLightMapResolution(m_lightMapSize >> 1); break;
			}
		}

		if(e->getType() != KeyEvent::UP && e->getKeycode() == SAUCE_KEY_SPACE)
		{
			m_currentLight->randomize();
		}
	}

	void onMouseMove(MouseEvent *e)
	{
		m_currentLight->position = e->getPosition();
	}

	void onMouseDown(MouseEvent *e)
	{
		if(e->getButton() == SAUCE_MOUSE_BUTTON_LEFT)
		{
			m_currentLight = new Light();
			m_currentLight->position = e->getPosition();
			m_lights.push_back(m_currentLight);
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
		GraphicsContext *context = e->getGraphicsContext();
		Vector2F viewSize = context->getSize();

		// Draw scene
		drawScene(context);

		// Fill shadows render target with black
		context->pushRenderTarget(m_shadowsRenderTarget);
		context->clear(GraphicsContext::COLOR_BUFFER, Color(20, 20, 20, 255));
		context->popRenderTarget();

		// Draw lights
		for(Light *l : m_lights)
		{
			drawLight(l, m_shadowsRenderTarget, context);
		}

		// Debug draw
		switch(m_debugState)
		{
			case SHOW_OCCLUDER_MAP:
			{
				context->setTexture(0); context->drawRectangle(0, 0, m_lightMapSize, m_lightMapSize, Color(0, 0, 0, 255));
				context->setTexture(m_occludersRenderTarget->getTexture()); context->drawRectangle(0, 0, m_lightMapSize, m_lightMapSize);
			}
			break;

			case SHOW_SHADOW_MAP:
			{
				context->setTexture(0); context->drawRectangle(0, 0, m_lightMapSize, m_lightMapSize, Color(0, 0, 0, 255));
				context->setTexture(m_shadowMapRenderTarget->getTexture()); context->drawRectangle(0, 0, m_lightMapSize, m_lightMapSize);
			}
			break;

			case SHOW_SHADOW_RENDER:
			{
				context->setTexture(0); context->drawRectangle(0, 0, m_lightMapSize, m_lightMapSize, Color(0, 0, 0, 255));
				context->setTexture(m_shadowsRenderTarget->getTexture()); context->drawRectangle(0, 0, m_lightMapSize, m_lightMapSize);
			}
			break;

			default:
				break;
		}

		// Draw shadows
		context->setBlendState(BlendState::PRESET_MULTIPLY);
		context->setTexture(m_shadowsRenderTarget->getTexture());
		context->drawRectangle(0, 0, viewSize.x, viewSize.y);
		context->setTexture(0);
		context->setBlendState(BlendState::PRESET_ALPHA_BLEND);

		// Draw info
		drawInfo(context);

		Game::onDraw(e);
	}

	void drawScene(GraphicsContext *context)
	{
		// Get view size
		Vector2F viewSize = context->getSize();

		// Draw tiles using repeat
		context->setTexture(m_tileTexture);
		context->drawRectangle(0, 0, viewSize.x, viewSize.y, Color::White, TextureRegion(0, 0, (float) viewSize.x / m_tileTexture->getWidth(), (float) viewSize.y / m_tileTexture->getHeight()));

		// Draw scene
		context->setTexture(m_sceneTexture);
		context->drawRectangle(0, 0, viewSize.x, viewSize.y);

		// Reset
		context->setTexture(0);
	}

	void drawLight(Light *light, RenderTarget2D *dest, GraphicsContext *context)
	{
		context->disable(GraphicsContext::BLEND);

		// Draw occluders to render target
		context->pushRenderTarget(m_occludersRenderTarget);
		context->clear(GraphicsContext::COLOR_BUFFER);
		context->setTexture(m_sceneTexture);
		context->drawRectangle(((Vector2F(light->radius * 0.5f) - light->position) / light->radius) * m_lightMapSize, m_sceneTexture->getSize() * m_lightMapSize / light->radius);
		context->popRenderTarget();

		// Create 1D shadow map
		context->pushRenderTarget(m_shadowMapRenderTarget);
		context->setShader(m_shadowMapShader);
		m_shadowMapShader->setUniform2f("u_Resolution", m_lightMapSize, m_lightMapSize);
		m_shadowMapShader->setUniform1f("u_Scale", 1.0f);
		m_shadowMapShader->setSampler2D("u_Texture", m_occludersRenderTarget->getTexture());
		context->drawRectangle(0.0f, 0.0f, m_lightMapSize, m_shadowMapRenderTarget->getHeight());
		context->popRenderTarget();

		// Render the shadows
		context->pushRenderTarget(dest);
		context->enable(GraphicsContext::BLEND);
		context->setBlendState(BlendState::PRESET_ADDITIVE);
		context->setShader(m_shadowRenderShader);
		m_shadowRenderShader->setUniform2f("u_Resolution", m_lightMapSize, m_lightMapSize);
		m_shadowRenderShader->setUniform3f("u_Color", light->color.getR() / 255.0f, light->color.getG() / 255.0f, light->color.getB() / 255.0f);
		m_shadowRenderShader->setUniform1f("u_SoftShadows", 1.0f);
		m_shadowRenderShader->setSampler2D("u_Texture", m_shadowMapRenderTarget->getTexture());
		context->drawRectangle(light->position - Vector2F(light->radius * 0.5f), Vector2F(light->radius));
		context->popRenderTarget();
	}

	void drawInfo(GraphicsContext *context)
	{
		stringstream ss;
		ss << "FPS: " << getFPS() << endl;
		ss << "Debug: ";
		switch(m_debugState)
		{
			case DEBUG_STATE_NONE: ss << "Off"; break;
			case SHOW_OCCLUDER_MAP: ss << "Occluder map"; break;
			case SHOW_SHADOW_MAP: ss << "Shadow map"; break;
			case SHOW_SHADOW_RENDER: ss << "Shadow render"; break;
			default: break;
		}
		ss << endl;
		ss << "Lights: " << m_lights.size() << endl;
		ss << "Light map resolution: " << m_lightMapSize << "x" << m_lightMapSize << endl;
		m_spriteBatch->begin(context);
		m_font->draw(m_spriteBatch, Vector2F(5.0f), ss.str());
		m_spriteBatch->end();
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