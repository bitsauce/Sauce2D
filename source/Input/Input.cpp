//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <CGF/Common.h>
#include <CGF/Input.h>

BEGIN_CGF_NAMESPACE

InputManager::InputManager(string contextFile) :
	m_context(0),
	m_x(0),
	m_y(0)
{
	// Set all str to key mappings
	m_strToKey["space"] = CGF_SCANCODE_SPACE;
	m_strToKey["quote"] = m_strToKey["apostrophe"] = CGF_SCANCODE_APOSTROPHE;
	m_strToKey["comma"] = CGF_SCANCODE_COMMA;
	m_strToKey["minus"] = CGF_SCANCODE_MINUS;
	m_strToKey["period"] = CGF_SCANCODE_PERIOD;
	m_strToKey["slash"] = CGF_SCANCODE_SLASH;
	m_strToKey["0"] = CGF_SCANCODE_0;
	m_strToKey["1"] = CGF_SCANCODE_1;
	m_strToKey["2"] = CGF_SCANCODE_2;
	m_strToKey["3"] = CGF_SCANCODE_3;
	m_strToKey["4"] = CGF_SCANCODE_4;
	m_strToKey["5"] = CGF_SCANCODE_5;
	m_strToKey["6"] = CGF_SCANCODE_6;
	m_strToKey["7"] = CGF_SCANCODE_7;
	m_strToKey["8"] = CGF_SCANCODE_8;
	m_strToKey["9"] = CGF_SCANCODE_9;
	m_strToKey["semicolon"] = CGF_SCANCODE_SEMICOLON;
	m_strToKey["equals"] = CGF_SCANCODE_EQUALS;
	m_strToKey["a"] = CGF_SCANCODE_A;
	m_strToKey["b"] = CGF_SCANCODE_B;
	m_strToKey["c"] = CGF_SCANCODE_C;
	m_strToKey["d"] = CGF_SCANCODE_D;
	m_strToKey["e"] = CGF_SCANCODE_E;
	m_strToKey["f"] = CGF_SCANCODE_F;
	m_strToKey["g"] = CGF_SCANCODE_G;
	m_strToKey["h"] = CGF_SCANCODE_H;
	m_strToKey["i"] = CGF_SCANCODE_I;
	m_strToKey["j"] = CGF_SCANCODE_J;
	m_strToKey["k"] = CGF_SCANCODE_K;
	m_strToKey["l"] = CGF_SCANCODE_L;
	m_strToKey["m"] = CGF_SCANCODE_M;
	m_strToKey["n"] = CGF_SCANCODE_N;
	m_strToKey["o"] = CGF_SCANCODE_O;
	m_strToKey["p"] = CGF_SCANCODE_P;
	m_strToKey["q"] = CGF_SCANCODE_Q;
	m_strToKey["r"] = CGF_SCANCODE_R;
	m_strToKey["s"] = CGF_SCANCODE_S;
	m_strToKey["t"] = CGF_SCANCODE_T;
	m_strToKey["u"] = CGF_SCANCODE_U;
	m_strToKey["v"] = CGF_SCANCODE_V;
	m_strToKey["w"] = CGF_SCANCODE_W;
	m_strToKey["x"] = CGF_SCANCODE_X;
	m_strToKey["y"] = CGF_SCANCODE_Y;
	m_strToKey["z"] = CGF_SCANCODE_Z;
	m_strToKey["lbracket"] = m_strToKey["leftbracket"] = CGF_SCANCODE_LEFTBRACKET;
	m_strToKey["backslash"] = CGF_SCANCODE_BACKSLASH;
	m_strToKey["rightbracket"] = CGF_SCANCODE_RIGHTBRACKET;
	m_strToKey["tilde"] = CGF_SCANCODE_GRAVE;
	m_strToKey["escape"] = CGF_SCANCODE_ESCAPE;
	m_strToKey["enter"] = m_strToKey["return"] = CGF_SCANCODE_RETURN;
	m_strToKey["tab"] = CGF_SCANCODE_TAB;
	m_strToKey["backspace"] = CGF_SCANCODE_BACKSPACE;
	m_strToKey["insert"] = CGF_SCANCODE_INSERT;
	m_strToKey["delete"] = CGF_SCANCODE_DELETE;
	m_strToKey["right"] = CGF_SCANCODE_RIGHT;
	m_strToKey["left"] = CGF_SCANCODE_LEFT;
	m_strToKey["down"] = CGF_SCANCODE_DOWN;
	m_strToKey["up"] = CGF_SCANCODE_UP;
	m_strToKey["pageup"] = CGF_SCANCODE_PAGEUP;
	m_strToKey["pagedown"] = CGF_SCANCODE_PAGEDOWN;
	m_strToKey["home"] = CGF_SCANCODE_HOME;
	m_strToKey["end"] = CGF_SCANCODE_END;
	m_strToKey["capslock"] = CGF_SCANCODE_CAPSLOCK;
	m_strToKey["scrolllock"] = CGF_SCANCODE_SCROLLLOCK;
	m_strToKey["numlock"] = CGF_SCANCODE_NUMLOCKCLEAR;
	m_strToKey["printscreen"] = CGF_SCANCODE_PRINTSCREEN;
	m_strToKey["pause"] = CGF_SCANCODE_PAUSE;
	m_strToKey["f1"] = CGF_SCANCODE_F1;
	m_strToKey["f2"] = CGF_SCANCODE_F2;
	m_strToKey["f3"] = CGF_SCANCODE_F3;
	m_strToKey["f4"] = CGF_SCANCODE_F4;
	m_strToKey["f5"] = CGF_SCANCODE_F5;
	m_strToKey["f6"] = CGF_SCANCODE_F6;
	m_strToKey["f7"] = CGF_SCANCODE_F7;
	m_strToKey["f8"] = CGF_SCANCODE_F8;
	m_strToKey["f9"] = CGF_SCANCODE_F9;
	m_strToKey["f10"] = CGF_SCANCODE_F10;
	m_strToKey["f11"] = CGF_SCANCODE_F11;
	m_strToKey["f12"] = CGF_SCANCODE_F12;
	m_strToKey["f13"] = CGF_SCANCODE_F13;
	m_strToKey["f14"] = CGF_SCANCODE_F14;
	m_strToKey["f15"] = CGF_SCANCODE_F15;
	m_strToKey["f16"] = CGF_SCANCODE_F16;
	m_strToKey["f17"] = CGF_SCANCODE_F17;
	m_strToKey["f18"] = CGF_SCANCODE_F18;
	m_strToKey["f19"] = CGF_SCANCODE_F19;
	m_strToKey["f20"] = CGF_SCANCODE_F20;
	m_strToKey["f21"] = CGF_SCANCODE_F21;
	m_strToKey["f22"] = CGF_SCANCODE_F22;
	m_strToKey["f23"] = CGF_SCANCODE_F23;
	m_strToKey["f24"] = CGF_SCANCODE_F24;
	m_strToKey["kp0"] = CGF_SCANCODE_KP_0;
	m_strToKey["kp1"] = CGF_SCANCODE_KP_1;
	m_strToKey["kp2"] = CGF_SCANCODE_KP_2;
	m_strToKey["kp3"] = CGF_SCANCODE_KP_3;
	m_strToKey["kp4"] = CGF_SCANCODE_KP_4;
	m_strToKey["kp5"] = CGF_SCANCODE_KP_5;
	m_strToKey["kp6"] = CGF_SCANCODE_KP_6;
	m_strToKey["kp7"] = CGF_SCANCODE_KP_7;
	m_strToKey["kp8"] = CGF_SCANCODE_KP_8;
	m_strToKey["kp9"] = CGF_SCANCODE_KP_9;
	m_strToKey["kpdecimal"] = CGF_SCANCODE_KP_DECIMAL;
	m_strToKey["kpdivide"] = CGF_SCANCODE_KP_DIVIDE;
	m_strToKey["kpmultiply"] = CGF_SCANCODE_KP_MULTIPLY;
	m_strToKey["kpminus"] = CGF_SCANCODE_KP_MINUS;
	m_strToKey["kpplus"] = CGF_SCANCODE_KP_PLUS;
	m_strToKey["kpenter"] = CGF_SCANCODE_KP_ENTER;
	m_strToKey["kpequals"] = CGF_SCANCODE_KP_EQUALS;
	m_strToKey["lshift"] = m_strToKey["leftshift"] = CGF_SCANCODE_LSHIFT;
	m_strToKey["lctrl"] = m_strToKey["leftctrl"] = CGF_SCANCODE_LCTRL;
	m_strToKey["lalt"] = m_strToKey["leftalt"] = CGF_SCANCODE_LALT;
	m_strToKey["lgui"] = m_strToKey["leftgui"] = CGF_SCANCODE_LGUI;
	m_strToKey["rshift"] = m_strToKey["rightshift"] = CGF_SCANCODE_RSHIFT;
	m_strToKey["rctrl"] = m_strToKey["rightctrl"] = CGF_SCANCODE_RCTRL;
	m_strToKey["ralt"] = m_strToKey["rightalt"] = CGF_SCANCODE_RALT;
	m_strToKey["rgui"] = m_strToKey["rightgui"] = CGF_SCANCODE_RGUI;
	m_strToKey["menu"] = CGF_SCANCODE_MENU;

	m_strToKey["lmb"] = CGF_MOUSE_BUTTON_LEFT;
	m_strToKey["rmb"] = CGF_MOUSE_BUTTON_RIGHT;
	m_strToKey["wheel"] = CGF_MOUSE_BUTTON_MIDDLE;

	// Load input config file
	if(util::fileExists(contextFile))
	{
		tinyxml2::XMLDocument doc;
		util::toAbsoluteFilePath(contextFile);
		doc.LoadFile(contextFile.c_str());

		// Get root node
		tinyxml2::XMLNode *contextNode = doc.FirstChildElement();
		if(!contextNode)
		{
			LOG("Invalid input config file");
			return;
		}

		// For each context
		contextNode = contextNode->FirstChildElement();
		while(contextNode)
		{
			InputContext *inputContext = new InputContext(this);

			// For each key bind
			tinyxml2::XMLNode *node = contextNode->FirstChildElement();
			while(node)
			{
				// Get name and key
				tinyxml2::XMLElement *name = node->FirstChildElement("name");
				tinyxml2::XMLElement *key = node->FirstChildElement("key");
				if(name && key)
				{
					InputButton inputButton = toInputButton(key->GetText());
					if(inputButton.getType() != InputButton::NONE)
					{
						Keybind *kb = new Keybind(inputButton);
						inputContext->addKeybind(name->GetText(), kb);
						m_contextKeybinds.push_back(kb);
					}
					else
					{
						LOG("Unknown key '%s'", key->GetText());
					}
				}
				node = node->NextSibling();
			}
			m_contextMap[contextNode->Value()] = inputContext;
			contextNode = contextNode->NextSibling();
		}
	}
}

InputManager::~InputManager()
{
	for(Keybind *kb : m_contextKeybinds)
	{
		delete kb;
	}
	m_contextKeybinds.clear();
}

void InputManager::getPosition(Sint32 *x, Sint32 *y) const
{
	*x = m_x;
	*y = m_y;
}

Vector2I InputManager::getPosition() const
{
	return Vector2I(m_x, m_y);
}

Sint32 InputManager::getX() const
{
	return m_x;
}

Sint32 InputManager::getY() const
{
	return m_y;
}

InputButton InputManager::toInputButton(string name) const
{
	transform(name.begin(), name.end(), name.begin(), ::tolower);
	map<string, InputButton>::const_iterator itr;
	if((itr = m_strToKey.find(name)) != m_strToKey.end())
	{
		return itr->second;
	}
	return CGF_SCANCODE_UNKNOWN;
}

void InputManager::setContext(InputContext *inputContext)
{
	m_context = inputContext;
}

InputContext *InputManager::getContextByName(const string &name)
{
	if(m_contextMap.find(name) == m_contextMap.end())
	{
		LOG("No input context with name '%s'", name);
		return 0;
	}
	return m_contextMap[name];
}

string InputManager::getClipboardString()
{
	return SDL_GetClipboardText();
}

void InputManager::setClipboardString(const string str)
{
	SDL_SetClipboardText(str.c_str());
}

void InputManager::addKeybind(Keybind *keybind)
{
	m_keybinds.push_back(keybind);
}

void InputManager::removeKeybind(Keybind *keybind)
{
	m_keybinds.remove(keybind);
}

void InputManager::updateKeybinds(KeyEvent *e)
{
	//if(m_game->isEnabled(CGF_BLOCK_BACKGROUND_INPUT) && !m_game->getWindow()->checkFlags(SDL_WINDOW_INPUT_FOCUS)) return;

	// Update keybinds
	for(Keybind *kb : m_keybinds)
	{
		if(kb->getInputButton() == e->getInputButton() && kb->getFunction())
		{
			kb->getFunction()(e);
		}
	}

	// Update keybinds of current context
	if(m_context)
	{
		m_context->updateKeybinds(e);
	}
}

bool InputManager::getKeyState(const InputButton inputButton) const
{
	//if(m_game->isEnabled(CGF_BLOCK_BACKGROUND_INPUT) && !m_game->getWindow()->checkFlags(SDL_WINDOW_INPUT_FOCUS)) return false;
	switch(inputButton.getType())
	{
		case InputButton::KEYBOARD: return SDL_GetKeyboardState(NULL)[inputButton.getCode()];
		case InputButton::MOUSE: return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(inputButton.getCode())) != 0;
	}
	return false;
}

END_CGF_NAMESPACE