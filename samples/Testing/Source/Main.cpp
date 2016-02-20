#include <CGF/CGF.h>

using namespace cgf;

// This tests:
// Loading texture from image file
// Saving image to file
// Rendering said image to the screen
class DrawTexturedQuad : public Game
{
	Resource<Texture2D> texture, textureSaved;
public:
	DrawTexturedQuad() :
		Game("DrawTexturedQuad")
	{
	}

	void onStart(GameEvent *e)
	{
		texture = getResourceManager()->get<Texture2D>("Koala");
		texture->exportToFile(":/Content/Texture_Saved.png");
		textureSaved = Resource<Texture2D>(new Texture2D(Pixmap("Texture_Saved.png")));
	}

	void onEnd(GameEvent *e)
	{
	}

	void onUpdate(TickEvent *e)
	{
	}

	void onDraw(DrawEvent *e)
	{
		GraphicsContext *context = e->getGraphicsContext();
		context->setTexture(texture);
		context->drawRectangle(0, 0, context->getWidth() / 2, context->getHeight());
		context->setTexture(textureSaved);
		context->drawRectangle(context->getWidth() / 2, 0, context->getWidth() / 2, context->getHeight());
		context->setTexture(0);
	}
};

class Keybinding : public Game
{
	Keybind m_keybind;
	Keybind m_jumpKeybind;
	Keybind m_runKeybind;
public:
	Keybinding() :
		Game("KeyBinding")
	{
	}

	void onStart(GameEvent *e)
	{
		// Get input manager
		InputManager *input = getInputManager();

		// Create a keybind which will call the function
		// BKeyFunc when the B key is pressed.
		m_keybind = Keybind(CGF_KEY_B, bind(&Keybinding::BKeyFunc, this, placeholders::_1));
		//m_keybind = getInputManager()->ceateKeybind(CGF_KEY_B, bind(&Keybinding::BKeyFunc, this, placeholders::_1));
		input->addKeybind(&m_keybind); // Add it to the list of keybinds

		// Create an input context.
		// Input contexts map symbolic strings to keys.
		// For example: {{ "jump", SPACE }, {"run", LSHIFT }}
		InputContext *context = new InputContext(input);

		// Create a keybind by mapping the keyword 
		// "jump" to the SPACE key.
		m_jumpKeybind = Keybind(CGF_KEY_SPACE, bind(&Keybinding::jump, this, placeholders::_1));
		context->addKeybind("jump", &m_jumpKeybind); // Add keybind to input context

		// Create run keybind which has no funtion mapped to it.
		// We can stil use this keybind with context->getKeyState("run").
		m_runKeybind = Keybind(CGF_KEY_LSHIFT);
		context->addKeybind("run", &m_runKeybind);

		// Enable this context
		input->setContext(context);
	}

	void onEnd(GameEvent *e)
	{
		InputManager *input = getInputManager();
		input->removeKeybind(&m_keybind);
		input->getContext()->removeKeybind(&m_jumpKeybind);
	}

	void BKeyFunc(KeyEvent *e)
	{
		switch(e->getType())
		{
			case EVENT_KEY_DOWN: LOG("B pressed"); break;
			case EVENT_KEY_REPEAT: LOG("B repeating"); break;
			case EVENT_KEY_UP:
				LOG("B released");
				m_keybind.setFunction(bind(&Keybinding::DKeyFunc, this, placeholders::_1));
				m_keybind.setKeycode(CGF_KEY_D);
				break;
		}
	}

	void DKeyFunc(KeyEvent *e)
	{
		switch(e->getType())
		{
			case EVENT_KEY_DOWN: LOG("D pressed"); break;
			case EVENT_KEY_REPEAT: LOG("D repeating"); break;
			case EVENT_KEY_UP:
				LOG("D released");
				m_keybind.setFunction(bind(&Keybinding::BKeyFunc, this, placeholders::_1));
				m_keybind.setKeycode(CGF_KEY_B);
				break;
		}
	}

	void jump(KeyEvent *e)
	{
		switch(e->getType())
		{
			case EVENT_KEY_DOWN: LOG("Jump pressed"); break;
			case EVENT_KEY_REPEAT: LOG("Jump repeating"); break;
			case EVENT_KEY_UP: LOG("Jump released"); break;
		}
	}

	void onKeyEvent(KeyEvent *e)
	{
		if(e->getKeycode() == CGF_KEY_C)
		{
			switch(e->getType())
			{
				case EVENT_KEY_DOWN: LOG("C pressed"); break;
				case EVENT_KEY_REPEAT: LOG("C repeating"); break;
				case EVENT_KEY_UP: LOG("C released"); break;
			}
		}
	}

	void onTick(TickEvent *e)
	{
		InputManager *input = getInputManager();
		if(input->getKeyState(CGF_KEY_A))
		{
			LOG("onTick(): A pressed");
		}

		if(input->getContext()->getKeyState("run"))
		{
			LOG("onTick(): Run");
		}
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// DrawTexturedQuad
	{
		DrawTexturedQuad game;
		if(game.run() != CGF_OK) return EXIT_FAILURE;
	}

	// KeyBinding
	{
		Keybinding game;
		if(game.run() != CGF_OK) return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}