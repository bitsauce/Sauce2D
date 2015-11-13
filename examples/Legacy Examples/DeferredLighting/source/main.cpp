//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/x2d.h>

struct Light
{
	Vector3 position, color;
	float radius, strength;
};

class GameManager
{
public:
	static void main()
	{
		XWindow::setSize(Vector2i(512, 512));

		screenQuad = new xd::Vertex[4];

		screenQuad[0].set4f(xd::VERTEX_POSITION, 0.0f, 0.0f);
		screenQuad[1].set4f(xd::VERTEX_POSITION, 512.0f, 0.0f);
		screenQuad[2].set4f(xd::VERTEX_POSITION, 0.0f, 512.0f);
		screenQuad[3].set4f(xd::VERTEX_POSITION, 512.0f, 512.0f);

		screenQuad[0].set4ub(xd::VERTEX_COLOR, 255, 255, 255, 255);
		screenQuad[1].set4ub(xd::VERTEX_COLOR, 255, 255, 255, 255);
		screenQuad[2].set4ub(xd::VERTEX_COLOR, 255, 255, 255, 255);
		screenQuad[3].set4ub(xd::VERTEX_COLOR, 255, 255, 255, 255);
		
		screenQuad[0].set4f(xd::VERTEX_TEX_COORD, 0.0f, 1.0f);
		screenQuad[1].set4f(xd::VERTEX_TEX_COORD, 1.0f, 1.0f);
		screenQuad[2].set4f(xd::VERTEX_TEX_COORD, 0.0f, 0.0f);
		screenQuad[3].set4f(xd::VERTEX_TEX_COORD, 1.0f, 0.0f);

		font = xd::ResourceManager::get<xd::Font>("Tahoma 18");

		sceneColorMap = xd::ResourceManager::get<xd::Texture2D>(":/sprites/scene_colormap.png");
		sceneLightMap = xd::ResourceManager::get<xd::Texture2D>(":/sprites/scene_lightmap.png");
		sceneNormalMap = xd::ResourceManager::get<xd::Texture2D>(":/sprites/scene_normalmap.png");

		sceneRenderTarget = new xd::RenderTarget2D(512, 512, 3); // 3 = attachment layers
		shadowMapRenderTarget = new xd::RenderTarget2D(512, 512, 1);

		colorMapTexture = sceneRenderTarget->getTexture(0);
		normalMapTexture = sceneRenderTarget->getTexture(1);
		depthMapTexture = sceneRenderTarget->getTexture(2);
		shadowMapTexture = shadowMapRenderTarget->getTexture();
		
		textureCombineShader = xd::ShaderPtr(new xd::Shader(":/shaders/texture_combine.vert", ":/shaders/texture_combine.frag"));
		pointLightShader = xd::ShaderPtr(new xd::Shader(":/shaders/point_light.vert", ":/shaders/point_light.frag"));
		//spotLightShader = xd::ShaderPtr(new xd::Shader(":/shaders/spot_light.vert", ":/shaders/spot_light.frag"));
		lightCombineShader = xd::ShaderPtr(new xd::Shader(":/shaders/combine_light.vert", ":/shaders/combine_light.frag"));

		Light *l = cursorLight = new Light;
		l->radius = 500.0f;
		l->strength = 1.0f;
		l->color.set(1.0f, 1.0f, 1.0f);
		l->position.set(400.0f, 300.0f, 0.0f);
		lights.push_back(l);
	}

	static Light *cursorLight;

	static void update()
	{
		if(XInput::getKeyState(XD_KEY_W)) ambient += 0.1f;
		if(XInput::getKeyState(XD_KEY_Q)) ambient -= 0.1f;
		
		if(XInput::getKeyState(XD_KEY_S)) lightAmbient += 0.1f;
		if(XInput::getKeyState(XD_KEY_A)) lightAmbient -= 0.1f;
		
		if(XInput::getKeyState(XD_KEY_X)) cursorLight->strength += 0.1f;
		if(XInput::getKeyState(XD_KEY_Z)) cursorLight->strength -= 0.1f;
		
		if(XInput::getKeyState(XD_KEY_R)) cursorLight->radius += 10.0f;
		if(XInput::getKeyState(XD_KEY_E)) cursorLight->radius -= 10.0f;
		
		if(XInput::getKeyState(XD_KEY_1)) cursorLight->color.set(1.0f, 1.0f, 1.0f);
		if(XInput::getKeyState(XD_KEY_2)) cursorLight->color.set(1.0f, 0.0f, 0.0f);
		if(XInput::getKeyState(XD_KEY_3)) cursorLight->color.set(0.0f, 1.0f, 0.0f);
		if(XInput::getKeyState(XD_KEY_4)) cursorLight->color.set(0.0f, 0.0f, 1.0f);
		if(XInput::getKeyState(XD_KEY_5)) cursorLight->color.set(1.0f, 0.0f, 1.0f);
		if(XInput::getKeyState(XD_KEY_6)) cursorLight->color.set(1.0f, 1.0f, 0.0f);
		if(XInput::getKeyState(XD_KEY_7)) cursorLight->color.set(0.0f, 1.0f, 1.0f);

		cursorLight->position.set(XInput::getPosition().x, 512 - XInput::getPosition().y, 0.0f);
	}

	static void draw(xd::GraphicsContext &graphicsDevice)
	{
		if(!spriteBatch)
			spriteBatch = new xd::SpriteBatch(graphicsDevice);

		graphicsDevice.setRenderTarget(sceneRenderTarget);
		{
			// Draw scene
			textureCombineShader->begin();
			textureCombineShader->setSampler2D("u_colorMap", sceneColorMap);
			textureCombineShader->setSampler2D("u_normalMap", sceneNormalMap);
			textureCombineShader->setSampler2D("u_lightMap", sceneLightMap);
			graphicsDevice.drawPrimitives(xd::GraphicsContext::PRIMITIVE_TRIANGLE_STRIP, screenQuad, 4);
			textureCombineShader->end();
		}
		graphicsDevice.setRenderTarget(nullptr);

		// Generate shadow map
		generateShadowMap(graphicsDevice);

		// Draw color + shadows
		drawCombined(graphicsDevice);

		spriteBatch->begin();
		spriteBatch->drawText(Vector2(5.0f),
			"Ambient: " + util::floatToStr(ambient) +
			"\nLight Ambient: " + util::floatToStr(lightAmbient) +
			"\nLight Strength: " + util::floatToStr(cursorLight->strength) +
			"\nLight Radius: " + util::floatToStr(cursorLight->radius),
			font.get());
		spriteBatch->end();
	}

	static void generateShadowMap(xd::GraphicsContext &graphicsDevice)
	{
		graphicsDevice.setRenderTarget(shadowMapRenderTarget);
		graphicsDevice.clear(xd::GraphicsContext::COLOR_BUFFER);

		xd::BlendState blendState(xd::BlendState::BLEND_ONE, xd::BlendState::BLEND_ONE);
		blendState.begin();

		// For every light inside the current scene, you can optimize this
		// list to only draw the lights that are visible a.t.m.
		pointLightShader->begin();
		for(int i = 0; i < lights.size(); ++i)
		{
			Light &light = *lights[i];

			pointLightShader->setUniform1f("u_lightStrength", light.strength);
			pointLightShader->setUniform3f("u_lightPosition", light.position.x, light.position.y, light.position.z);
			pointLightShader->setUniform3f("u_lightColor", light.color.x, light.color.y, light.color.z);
			pointLightShader->setUniform1f("u_lightRadius", light.radius);
			pointLightShader->setUniform1f("u_screenWidth", graphicsDevice.getWidth());
			pointLightShader->setUniform1f("u_screenHeight", graphicsDevice.getHeight());
			pointLightShader->setSampler2D("u_normalMap", normalMapTexture);
			pointLightShader->setSampler2D("u_lightMap", depthMapTexture);

			graphicsDevice.drawPrimitives(xd::GraphicsContext::PRIMITIVE_TRIANGLE_STRIP, screenQuad, 4);
		}
		pointLightShader->end();

		blendState.end();

		// Deactive the rander targets to resolve them
		graphicsDevice.setRenderTarget(nullptr);
	}

	static void drawCombined(xd::GraphicsContext &graphicsDevice)
	{
		lightCombineShader->begin();
		lightCombineShader->setUniform1f("u_ambient", ambient);
		lightCombineShader->setUniform3f("u_ambientColor", 1.0f, 1.0f, 1.0f);
		lightCombineShader->setUniform1f("u_lightAmbient", lightAmbient);
		lightCombineShader->setSampler2D("u_colorMap", colorMapTexture);
		lightCombineShader->setSampler2D("u_shadingMap", shadowMapTexture);
		graphicsDevice.drawPrimitives(xd::GraphicsContext::PRIMITIVE_TRIANGLE_STRIP, screenQuad, 4);
		lightCombineShader->end();
	}

	
	static void exit()
	{
	}

private:
	static xd::FontPtr font;
	static xd::SpriteBatch *spriteBatch;

	static xd::Texture2DPtr shadowMapTexture;
	static xd::Texture2DPtr colorMapTexture;
	static xd::Texture2DPtr normalMapTexture;
	static xd::Texture2DPtr depthMapTexture;

	static xd::RenderTarget2D *sceneRenderTarget;
	static xd::RenderTarget2D *shadowMapRenderTarget;

	static xd::ShaderPtr textureCombineShader;
	static xd::ShaderPtr pointLightShader;
	static xd::ShaderPtr lightCombineShader;

	static xd::Texture2DPtr sceneColorMap;
	static xd::Texture2DPtr sceneLightMap;
	static xd::Texture2DPtr sceneNormalMap;

	static xd::Vertex *screenQuad;

	static float ambient;
	static float lightAmbient;

	static vector<Light*> lights;
};

xd::FontPtr GameManager::font = nullptr;
xd::SpriteBatch *GameManager::spriteBatch = nullptr;

xd::Texture2DPtr GameManager::shadowMapTexture = nullptr;
xd::Texture2DPtr GameManager::colorMapTexture = nullptr;
xd::Texture2DPtr GameManager::normalMapTexture = nullptr;
xd::Texture2DPtr GameManager::depthMapTexture = nullptr;

xd::RenderTarget2D *GameManager::sceneRenderTarget = nullptr;
xd::RenderTarget2D *GameManager::shadowMapRenderTarget = nullptr;

xd::ShaderPtr GameManager::textureCombineShader = nullptr;
xd::ShaderPtr GameManager::pointLightShader = nullptr;
xd::ShaderPtr GameManager::lightCombineShader = nullptr;

xd::Texture2DPtr GameManager::sceneColorMap = nullptr;
xd::Texture2DPtr GameManager::sceneLightMap = nullptr;
xd::Texture2DPtr GameManager::sceneNormalMap = nullptr;

xd::Vertex *GameManager::screenQuad = nullptr;

float GameManager::ambient = 1.0f;
float GameManager::lightAmbient = 1.0f;

vector<Light*> GameManager::lights;
Light* GameManager::cursorLight;

// Win32 entry point
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// Process the command-line
	int flags = 0;
	string workDir;
	for(int i = 0; i < __argc; i++)
	{
		if(__argv[i][0] == '-')
		{
			switch(__argv[i][1])
			{
			case 'v': flags |= XD_EXPORT_LOG; break;
			case 'w': workDir = string(__argv[i]+3); break;
			}
		}
	}
	flags |= XD_EXPORT_LOG; // For now we force this flag

	XEngine *engine = CreateEngine();

	XConfig config;
	config.main = &GameManager::main;
	config.update = &GameManager::update;
	config.draw = &GameManager::draw;
	config.end = &GameManager::exit;
#ifdef X2D_DEBUG
	config.workDir = "..\\game\\";
#endif
	config.flags = flags;

	if(engine->init(config) != X2D_OK)
	{
		delete engine;
		return -1;
	}

	int r = engine->run();
	delete engine;
	return r;
}