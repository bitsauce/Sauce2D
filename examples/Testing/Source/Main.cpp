#include <CGF/cgf.h>

using namespace cgf;

// This tests:
// Loading texture from image file
// Saving image to file
// Rendering said image to the screen
class DrawTexturedQuad : public Game
{
	Texture2DPtr texture, textureSaved;
public:
	DrawTexturedQuad() :
		Game("DrawTexturedQuad")
	{
	}

	void onStart(GameEvent *e)
	{
		texture = ResourceManager::get<Texture2D>(":/Content/Texture.jpg");
		texture->exportToFile(":/Content/Texture_Saved.png");
		textureSaved = ResourceManager::get<Texture2D>(":/Content/Texture_Saved.png");
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
	KeybindPtr m_keybind;
public:
	Keybinding() :
		Game("KeyBinding")
	{
	}

	void BKeyFunc(KeyEvent *e)
	{
		switch(e->getType())
		{
			case EVENT_KEY_DOWN: LOG("B pressed"); break;
			case EVENT_KEY_REPEAT: LOG("B repeating"); break;
			case EVENT_KEY_UP: LOG("B released"); break;
		}
		// TODO: Allow updating keybinds
		//m_keybind->setKeycode(CGF_KEY_D);
	}

	void onStart(GameEvent *e)
	{
		InputManager *input = getInputManager();
		m_keybind = KeybindPtr(new Keybind(CGF_KEY_B, bind(&Keybinding::BKeyFunc, this, placeholders::_1)));
		input->addKeybind(m_keybind);

		// TODO:
		//input->getContext()->bind("jump", jumpFunc);
		// if(input->getContext()->getKeyState("jump"))
	}

	void onEnd(GameEvent *e)
	{
		InputManager *input = getInputManager();
		input->removeKeybind(m_keybind);
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
			LOG("A pressed");
		}
	}
};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// KeyBinding
	{
		Keybinding game;
		if(game.run() != CGF_OK) return EXIT_FAILURE;
	}

	// DrawTexturedQuad
	{
		DrawTexturedQuad game;
		if(game.run() != CGF_OK) return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}