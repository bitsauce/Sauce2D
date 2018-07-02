/* Include the SauceEngine framework */
#include <Sauce/Sauce.h>
using namespace sauce;

class RenderTargetsGame : public Game
{
	RenderTarget2D *m_renderTarget;
	RenderTarget2D *m_renderTarget2;
	shared_ptr<Texture2D> m_texture;

public:
	/** 
	 * Game constructor.
	 * This method is generally empty since
	 * initialization of resources need to happen in onStart()
	 */
	RenderTargetsGame() :
		/* Title of the game. Used as window title and name of folder containing save files */
		Game("RenderTargets")
	{
	}

	/** 
	 * onStart() event.
	 * This event called once the framework has initialized, but before the first tick.
	 * Load resources and setup the window here.
	 */
	void onStart(GameEvent *e)
	{
		GraphicsContext *graphicsContext = getWindow()->getGraphicsContext();
		m_renderTarget = graphicsContext->createRenderTarget(128, 128);
		m_renderTarget2 = graphicsContext->createRenderTarget(128, 128);
		m_texture = shared_ptr<Texture2D>(getWindow()->getGraphicsContext()->createTexture(Pixmap("Image.png")));
		Game::onStart(e);
	}

	/**
	 * onEnd() event.
	 * This event is called when the game is closing.
	 * Free all resources and save game state here.
	 */
	void onEnd(GameEvent *e)
	{
		Game::onEnd(e);
	}

	/**
	 * onTick() event.
	 * This event is called when the framework should update the game state.
	 * Things like physics and other simulations should go here.
	 */
	void onTick(TickEvent *e)
	{
		Game::onTick(e);
	}

	/**
	 * onDraw event.
	 * This event is called then the framework should draw the current game state to the screen.
	 * Using the graphics context provided by e->getGraphicsContext() is generally how we draw to the screen. 
	 */
	void onDraw(DrawEvent *e)
	{
		GraphicsContext *graphicsContext = e->getGraphicsContext();

		graphicsContext->setTexture(0);
		graphicsContext->pushRenderTarget(m_renderTarget2);
			graphicsContext->clear(GraphicsContext::COLOR_BUFFER);
			graphicsContext->pushRenderTarget(m_renderTarget);
				graphicsContext->clear(GraphicsContext::COLOR_BUFFER);
				graphicsContext->setTexture(m_texture);
				graphicsContext->drawRectangle(0, 0, 64, 64, Color::White, TextureRegion(0.5,0.5,0.9,0.9));
			graphicsContext->popRenderTarget();
			graphicsContext->drawRectangle(64, 0, 64, 64, Color(0, 255, 0, 255));
		graphicsContext->popRenderTarget();

		m_renderTarget->getTexture()->exportToFile("../RenderTarget.png");
		
		graphicsContext->drawRectangle(0, 0, graphicsContext->getWidth(), graphicsContext->getHeight(), Color(127, 127, 127, 255));
		
		graphicsContext->setTexture(m_renderTarget->getTexture());
		graphicsContext->drawRectangle(0, 0, 128, 128);
		graphicsContext->setTexture(m_renderTarget2->getTexture());
		graphicsContext->drawRectangle(0, 0, 128, 128);

		graphicsContext->setTexture(m_renderTarget->getTexture());
		graphicsContext->drawRectangle(300, 300, 64, 64);

		Game::onDraw(e);
	}
};

/* Main entry point. This is where our program first starts executing. */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	/** 
	 * To start the game we simply create a game object and call run().
	 * This function should typically not require modification.
	 */
	RenderTargetsGame game;
	return game.run();
}