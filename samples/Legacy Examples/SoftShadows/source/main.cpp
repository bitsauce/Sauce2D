//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/x2d.h>

xd::RenderTarget2D* distancesRenderTarget;
xd::RenderTarget2D* distortRenderTarget;
xd::RenderTarget2D** reductionRenderTargets;
xd::RenderTarget2D* shadowsRenderTarget;
xd::Texture2DPtr sceneTexture;
xd::Texture2DPtr tileTexture;
xd::ShaderPtr calcDistanceShader;
xd::ShaderPtr distortShader;
xd::ShaderPtr reductionShader;
xd::ShaderPtr shadowShader;
xd::ShaderPtr blurShadowVShader;
xd::ShaderPtr blurShadowHShader;
int reductionChainCount;
int baseSize;
int depthBufferSize;

enum ShadowmapSize
{
    Size128 = 6,
    Size256 = 7,
    Size512 = 8,
    Size1024 = 9,
    Size2048 = 10,
    Size4192 = 11
};

class GameManager
{
public:
	static void main()
	{
		int size = Size512;
		reductionChainCount = size;
		baseSize = 2 << reductionChainCount;
		depthBufferSize = 2 << size;

		distancesRenderTarget = new xd::RenderTarget2D(baseSize, baseSize);
		distortRenderTarget = new xd::RenderTarget2D(baseSize, baseSize);
		reductionRenderTargets = new xd::RenderTarget2D*[reductionChainCount];
		for(int i = 0; i < reductionChainCount; i++)
		{
			reductionRenderTargets[i] = new xd::RenderTarget2D(2 << i, baseSize);
		}
		shadowsRenderTarget = new xd::RenderTarget2D(baseSize, baseSize);

		sceneTexture = xd::ResourceManager::get<xd::Texture2D>(":/sprites/scene.png");
		tileTexture = xd::ResourceManager::get<xd::Texture2D>(":/sprites/tile.png");
		tileTexture->setWrapping(xd::Texture2D::REPEAT);

		calcDistanceShader = xd::ResourceManager::get<xd::Shader>(":/shaders/calc_distance");
		distortShader = xd::ResourceManager::get<xd::Shader>(":/shaders/distort");
		reductionShader = xd::ResourceManager::get<xd::Shader>(":/shaders/reduction");
		shadowShader = xd::ResourceManager::get<xd::Shader>(":/shaders/draw_shadows");
		blurShadowHShader = xd::ResourceManager::get<xd::Shader>(":/shaders/blur_shadows_h");
		blurShadowVShader = xd::ResourceManager::get<xd::Shader>(":/shaders/blur_shadows_v");

		XWindow::setSize(Vector2i(512, 512));
	}

	static void update()
	{
	}

	static void draw(xd::GraphicsContext &graphicsDevice)
	{
		// Draw tiles using repeat
		{
			xd::Vertex vertices[4];

			vertices[0].set4f(xd::VERTEX_POSITION, 0.0f,					0.0f);
			vertices[1].set4f(xd::VERTEX_POSITION, XWindow::getSize().x,	0.0f);
			vertices[2].set4f(xd::VERTEX_POSITION, 0.0f,					XWindow::getSize().y);
			vertices[3].set4f(xd::VERTEX_POSITION, XWindow::getSize().x,	XWindow::getSize().y);

			vertices[0].set4ub(xd::VERTEX_COLOR, 255, 255, 255, 255);
			vertices[1].set4ub(xd::VERTEX_COLOR, 255, 255, 255, 255);
			vertices[2].set4ub(xd::VERTEX_COLOR, 255, 255, 255, 255);
			vertices[3].set4ub(xd::VERTEX_COLOR, 255, 255, 255, 255);
	
			float u = (float)XWindow::getSize().x/tileTexture->getWidth(),
				v = (float)XWindow::getSize().y/tileTexture->getHeight();
			vertices[0].set4f(xd::VERTEX_TEX_COORD, 0.0f, v);
			vertices[1].set4f(xd::VERTEX_TEX_COORD, u,    v);
			vertices[2].set4f(xd::VERTEX_TEX_COORD, 0.0f, 0.0f);
			vertices[3].set4f(xd::VERTEX_TEX_COORD, u,    0.0f);
			
			graphicsDevice.setTexture(tileTexture);
			graphicsDevice.drawPrimitives(xd::GraphicsContext::PRIMITIVE_TRIANGLE_STRIP, vertices, 4);
		}

		// Draw scene
		graphicsDevice.setTexture(sceneTexture);
		graphicsDevice.drawRectangle(0, 0, 512, 512);

		graphicsDevice.setRenderTarget(distancesRenderTarget);
		graphicsDevice.clear(xd::GraphicsContext::COLOR_BUFFER, xd::Color(255));

			graphicsDevice.setShader(calcDistanceShader);
			calcDistanceShader->setSampler2D("u_texture", sceneTexture);
			calcDistanceShader->setUniform1f("u_width", baseSize);
			graphicsDevice.drawRectangle(0, 0, baseSize, baseSize);
		
		graphicsDevice.setRenderTarget(distortRenderTarget);
		graphicsDevice.clear(xd::GraphicsContext::COLOR_BUFFER);

			graphicsDevice.setShader(distortShader);
			distortShader->setSampler2D("u_texture", distancesRenderTarget->getTexture());
			graphicsDevice.drawRectangle(0, 0, baseSize, baseSize);

		xd::Texture2DPtr reducedTexture = applyHorizontalReduction(graphicsDevice, distortRenderTarget);		
		graphicsDevice.setRenderTarget(shadowsRenderTarget);
		graphicsDevice.clear(xd::GraphicsContext::COLOR_BUFFER);

			graphicsDevice.setShader(shadowShader);
			shadowShader->setSampler2D("u_texture", reducedTexture);
			graphicsDevice.drawRectangle(0, 0, baseSize, baseSize);
			
		/*graphicsDevice.setRenderTarget(distortRenderTarget);
		graphicsDevice.clear(xd::GraphicsContext::COLOR_BUFFER);

			graphicsDevice.setShader(blurShadowHShader);
			blurShadowHShader->setSampler2D("u_texture", shadowsRenderTarget->getTexture());
			blurShadowHShader->setUniform1f("u_width", baseSize);
			graphicsDevice.drawRectangle(0, 0, baseSize, baseSize);
			
		graphicsDevice.setRenderTarget(shadowsRenderTarget);
		graphicsDevice.clear(xd::GraphicsContext::COLOR_BUFFER);

			graphicsDevice.setShader(blurShadowVShader);
			blurShadowVShader->setSampler2D("u_texture", distortRenderTarget->getTexture());
			blurShadowVShader->setUniform1f("u_width", baseSize);
			graphicsDevice.drawRectangle(0, 0, baseSize, baseSize);*/

		graphicsDevice.setRenderTarget(nullptr);
		graphicsDevice.setShader(nullptr);

		graphicsDevice.setBlendState(xd::BlendState::PRESET_MULTIPLY);
		graphicsDevice.setTexture(shadowsRenderTarget->getTexture());
		graphicsDevice.drawRectangle(0, 0, 512, 512);
		graphicsDevice.setBlendState(xd::BlendState::PRESET_ALPHA_BLEND);
	}
	
	static xd::Texture2DPtr applyHorizontalReduction(xd::GraphicsContext &graphicsDevice, xd::RenderTarget2D *source)
	{
		int step = reductionChainCount-1;
		xd::RenderTarget2D *s = source;
		xd::RenderTarget2D *d = reductionRenderTargets[step];
		graphicsDevice.setShader(reductionShader);

		while(step >= 0)
		{
			d = reductionRenderTargets[step];

			graphicsDevice.setRenderTarget(d);
			graphicsDevice.clear(xd::GraphicsContext::COLOR_BUFFER, xd::Color(255));

			reductionShader->setSampler2D("u_texture", s->getTexture());
			Vector2 textureDimentions(1.0f / (float)s->getWidth(), 1.0f / (float)s->getHeight());
			reductionShader->setUniform2f("u_textureDimentions", textureDimentions.x, textureDimentions.y);

			graphicsDevice.drawRectangle(0, 0, d->getWidth() + 1, baseSize);
			
			s = d;
			step--;
		}

		graphicsDevice.setShader(nullptr);
		graphicsDevice.setRenderTarget(nullptr);

		//copy to destination
		return d->getTexture();
	}
	
	static void exit()
	{
	}
};

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