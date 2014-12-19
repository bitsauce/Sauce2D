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
		sceneColorMap = xd::ResourceManager::get<xd::Texture2D>(":/sprites/scene_colormap.png");
		sceneLightMap = xd::ResourceManager::get<xd::Texture2D>(":/sprites/scene_lightmap.png");
		sceneNormalMap = xd::ResourceManager::get<xd::Texture2D>(":/sprites/scene_normalmap.png");

		sceneRenderTarget = new xd::RenderTarget2D(800, 600, 3); // 3 = attachment layers
		shadowMapRenderTarget = new xd::RenderTarget2D(800, 600, 1);

		colorMapTexture = sceneRenderTarget->getTexture(0);
		normalMapTexture = sceneRenderTarget->getTexture(1);
		depthMapTexture = sceneRenderTarget->getTexture(2);
		shadowMapTexture = shadowMapRenderTarget->getTexture();
		
		textureCombineShader = xd::ShaderPtr(new xd::Shader(":/shaders/texture_combine.vert", ":/shaders/texture_combine.frag"));
		lightShader1 = xd::ShaderPtr(new xd::Shader(":/shaders/point_light.vert", ":/shaders/point_light.frag"));
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
		if(XInput::getKeyState(XD_KEY_1)) state = 0;
		if(XInput::getKeyState(XD_KEY_2)) state = 1;
		if(XInput::getKeyState(XD_KEY_3)) state = 2;
		if(XInput::getKeyState(XD_KEY_4)) state = 3;
		if(XInput::getKeyState(XD_KEY_5)) state = 4;
		cursorLight->position.set(XInput::getPosition().x, 600-XInput::getPosition().y, 0.0f);
	}

	static void draw(xd::GraphicsContext &graphicsDevice)
	{
		graphicsDevice.setRenderTarget(sceneRenderTarget);
		//graphicsDevice.clear(xd::GraphicsContext::DEPTH_BUFFER, COLOR_BLACK);

		{
			// Setup batch
			xd::Batch batch;
			batch.setShader(textureCombineShader);

			// Draw scene
			xd::Shape shape(Rect(0, 0, 800, 600));
			textureCombineShader->setSampler2D("u_colorMap", sceneColorMap);
			textureCombineShader->setSampler2D("u_normalMap", sceneNormalMap);
			textureCombineShader->setSampler2D("u_lightMap", sceneLightMap);
			shape.draw(&batch);

			batch.setShader(nullptr);

			graphicsDevice.renderBatch(batch);
		}

		graphicsDevice.setRenderTarget(nullptr);

		// Generate shadow map
		generateShadowMap(graphicsDevice);
		
		xd::Batch batch;
		xd::Shape shape(Rect(0, 0, 800, 600));
		switch(state)
		{
		case 0: shape.setFillTexture(colorMapTexture); break;
		case 1: shape.setFillTexture(normalMapTexture); break;
		case 2: shape.setFillTexture(depthMapTexture); break;
		case 3: shape.setFillTexture(shadowMapTexture); break;
		case 4: drawCombined(graphicsDevice); return;
		}
		shape.draw(&batch);

		graphicsDevice.renderBatch(batch);
	}

	static void generateShadowMap(xd::GraphicsContext &graphicsDevice)
	{
		graphicsDevice.setRenderTarget(shadowMapRenderTarget);
		//graphicsDevice.Clear(ClearOptions.Target | ClearOptions.DepthBuffer, Color.Black, 1, 0);
 
		xd::Batch batch;
		//graphicsDevice.enable(xd::GraphicsContext::ALPHA_TEST);
		//graphicsDevice.setBlendMode(xd::GraphicsContext::BLEND_ONE, xd::GraphicsContext::BLEND_ONE);
		batch.setBlendFunc(xd::Batch::BLEND_ONE, xd::Batch::BLEND_ONE);
 
		// For every light inside the current scene, you can optimize this
		// list to only draw the lights that are visible a.t.m.
		batch.setShader(lightShader1);
		for(int i = 0; i < lights.size(); ++i)
		{
			Light &light = *lights[i];
			
			xd::Shape shape(Rect(0, 0, 800, 600));
			lightShader1->setUniform1f("u_lightStrength", light.strength);
			lightShader1->setUniform3f("u_lightPosition", light.position.x, light.position.y, light.position.z);
			lightShader1->setUniform3f("u_lightColor", light.color.x, light.color.y, light.color.z);
			lightShader1->setUniform1f("u_lightRadius", light.radius);
			lightShader1->setUniform1f("u_screenWidth", graphicsDevice.getWidth());
			lightShader1->setUniform1f("u_screenHeight", graphicsDevice.getHeight());
			lightShader1->setSampler2D("u_normalMap", normalMapTexture);
			lightShader1->setSampler2D("u_lightMap", depthMapTexture);
			shape.draw(&batch);
		}
		batch.setShader(nullptr);
 
		// Deactivate alpha blending...
		//graphicsDevice.disable(xd::GraphicsContext::ALPHA_TEST);

		graphicsDevice.renderBatch(batch);

		// Deactive the rander targets to resolve them
		graphicsDevice.setRenderTarget(nullptr);
	}

	static void drawCombined(xd::GraphicsContext &graphicsDevice)
	{
		xd::Batch batch;
		batch.setShader(lightCombineShader);
			xd::Shape shape(Rect(0, 0, 800, 600));
				lightCombineShader->setUniform1f("u_ambient", 1.0f);
				lightCombineShader->setUniform3f("u_ambientColor", 1.0f, 1.0f, 1.0f);
				lightCombineShader->setUniform1f("u_lightAmbient", 4.0f);
				lightCombineShader->setSampler2D("u_colorMap", colorMapTexture);
				lightCombineShader->setSampler2D("u_shadingMap", shadowMapTexture);
			shape.draw(&batch);
		batch.setShader(nullptr);
		graphicsDevice.renderBatch(batch);
	}

	
	static void exit()
	{
	}

private:
	static uint state;

	static xd::Texture2DPtr shadowMapTexture;
	static xd::Texture2DPtr colorMapTexture;
	static xd::Texture2DPtr normalMapTexture;
	static xd::Texture2DPtr depthMapTexture;

	static xd::RenderTarget2D *sceneRenderTarget;
	static xd::RenderTarget2D *shadowMapRenderTarget;

	static xd::ShaderPtr textureCombineShader;
	static xd::ShaderPtr lightShader1;
	static xd::ShaderPtr lightCombineShader;

	static xd::Texture2DPtr sceneColorMap;
	static xd::Texture2DPtr sceneLightMap;
	static xd::Texture2DPtr sceneNormalMap;

	static vector<Light*> lights;
};

uint GameManager::state = 0;

xd::Texture2DPtr GameManager::shadowMapTexture = nullptr;
xd::Texture2DPtr GameManager::colorMapTexture = nullptr;
xd::Texture2DPtr GameManager::normalMapTexture = nullptr;
xd::Texture2DPtr GameManager::depthMapTexture = nullptr;

xd::RenderTarget2D *GameManager::sceneRenderTarget = nullptr;
xd::RenderTarget2D *GameManager::shadowMapRenderTarget = nullptr;

xd::ShaderPtr GameManager::textureCombineShader = nullptr;
xd::ShaderPtr GameManager::lightShader1 = nullptr;
xd::ShaderPtr GameManager::lightCombineShader = nullptr;

xd::Texture2DPtr GameManager::sceneColorMap = nullptr;
xd::Texture2DPtr GameManager::sceneLightMap = nullptr;
xd::Texture2DPtr GameManager::sceneNormalMap = nullptr;

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