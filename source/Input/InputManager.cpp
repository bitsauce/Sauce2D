//     _____                        ______             _            
//    / ____|                      |  ____|           (_)           
//   | (___   __ _ _   _  ___ ___  | |__   _ __   __ _ _ _ __   ___ 
//    \___ \ / _` | | | |/ __/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \
//    ____) | (_| | |_| | (_|  __/ | |____| | | | (_| | | | | |  __/
//   |_____/ \__,_|\__,_|\___\___| |______|_| |_|\__, |_|_| |_|\___|
//                                                __/ |             
//                                               |___/              
// Made by Marcus "Bitsauce" Loo Vergara
// 2011-2018 (C)

#include <Sauce/Common.h>
#include <Sauce/Input.h>

BEGIN_SAUCE_NAMESPACE

InputManager::InputManager(string contextFile) :
	m_context(0),
	m_x(0),
	m_y(0),
	m_defaultController(0),
	m_leftTrigger(false),
	m_rightTrigger(false),
	m_triggerThreshold(0.7)
{
	// Set all str to key mappings
	m_strToKey["space"] = SAUCE_SCANCODE_SPACE;
	m_strToKey["quote"] = m_strToKey["apostrophe"] = SAUCE_SCANCODE_APOSTROPHE;
	m_strToKey["comma"] = SAUCE_SCANCODE_COMMA;
	m_strToKey["minus"] = SAUCE_SCANCODE_MINUS;
	m_strToKey["period"] = SAUCE_SCANCODE_PERIOD;
	m_strToKey["slash"] = SAUCE_SCANCODE_SLASH;
	m_strToKey["0"] = SAUCE_SCANCODE_0;
	m_strToKey["1"] = SAUCE_SCANCODE_1;
	m_strToKey["2"] = SAUCE_SCANCODE_2;
	m_strToKey["3"] = SAUCE_SCANCODE_3;
	m_strToKey["4"] = SAUCE_SCANCODE_4;
	m_strToKey["5"] = SAUCE_SCANCODE_5;
	m_strToKey["6"] = SAUCE_SCANCODE_6;
	m_strToKey["7"] = SAUCE_SCANCODE_7;
	m_strToKey["8"] = SAUCE_SCANCODE_8;
	m_strToKey["9"] = SAUCE_SCANCODE_9;
	m_strToKey["semicolon"] = SAUCE_SCANCODE_SEMICOLON;
	m_strToKey["equals"] = SAUCE_SCANCODE_EQUALS;
	m_strToKey["a"] = SAUCE_SCANCODE_A;
	m_strToKey["b"] = SAUCE_SCANCODE_B;
	m_strToKey["c"] = SAUCE_SCANCODE_C;
	m_strToKey["d"] = SAUCE_SCANCODE_D;
	m_strToKey["e"] = SAUCE_SCANCODE_E;
	m_strToKey["f"] = SAUCE_SCANCODE_F;
	m_strToKey["g"] = SAUCE_SCANCODE_G;
	m_strToKey["h"] = SAUCE_SCANCODE_H;
	m_strToKey["i"] = SAUCE_SCANCODE_I;
	m_strToKey["j"] = SAUCE_SCANCODE_J;
	m_strToKey["k"] = SAUCE_SCANCODE_K;
	m_strToKey["l"] = SAUCE_SCANCODE_L;
	m_strToKey["m"] = SAUCE_SCANCODE_M;
	m_strToKey["n"] = SAUCE_SCANCODE_N;
	m_strToKey["o"] = SAUCE_SCANCODE_O;
	m_strToKey["p"] = SAUCE_SCANCODE_P;
	m_strToKey["q"] = SAUCE_SCANCODE_Q;
	m_strToKey["r"] = SAUCE_SCANCODE_R;
	m_strToKey["s"] = SAUCE_SCANCODE_S;
	m_strToKey["t"] = SAUCE_SCANCODE_T;
	m_strToKey["u"] = SAUCE_SCANCODE_U;
	m_strToKey["v"] = SAUCE_SCANCODE_V;
	m_strToKey["w"] = SAUCE_SCANCODE_W;
	m_strToKey["x"] = SAUCE_SCANCODE_X;
	m_strToKey["y"] = SAUCE_SCANCODE_Y;
	m_strToKey["z"] = SAUCE_SCANCODE_Z;
	m_strToKey["lbracket"] = m_strToKey["leftbracket"] = SAUCE_SCANCODE_LEFTBRACKET;
	m_strToKey["backslash"] = SAUCE_SCANCODE_BACKSLASH;
	m_strToKey["rightbracket"] = SAUCE_SCANCODE_RIGHTBRACKET;
	m_strToKey["tilde"] = SAUCE_SCANCODE_GRAVE;
	m_strToKey["escape"] = SAUCE_SCANCODE_ESCAPE;
	m_strToKey["enter"] = m_strToKey["return"] = SAUCE_SCANCODE_RETURN;
	m_strToKey["tab"] = SAUCE_SCANCODE_TAB;
	m_strToKey["backspace"] = SAUCE_SCANCODE_BACKSPACE;
	m_strToKey["insert"] = SAUCE_SCANCODE_INSERT;
	m_strToKey["delete"] = SAUCE_SCANCODE_DELETE;
	m_strToKey["right"] = SAUCE_SCANCODE_RIGHT;
	m_strToKey["left"] = SAUCE_SCANCODE_LEFT;
	m_strToKey["down"] = SAUCE_SCANCODE_DOWN;
	m_strToKey["up"] = SAUCE_SCANCODE_UP;
	m_strToKey["pageup"] = SAUCE_SCANCODE_PAGEUP;
	m_strToKey["pagedown"] = SAUCE_SCANCODE_PAGEDOWN;
	m_strToKey["home"] = SAUCE_SCANCODE_HOME;
	m_strToKey["end"] = SAUCE_SCANCODE_END;
	m_strToKey["capslock"] = SAUCE_SCANCODE_CAPSLOCK;
	m_strToKey["scrolllock"] = SAUCE_SCANCODE_SCROLLLOCK;
	m_strToKey["numlock"] = SAUCE_SCANCODE_NUMLOCKCLEAR;
	m_strToKey["printscreen"] = SAUCE_SCANCODE_PRINTSCREEN;
	m_strToKey["pause"] = SAUCE_SCANCODE_PAUSE;
	m_strToKey["f1"] = SAUCE_SCANCODE_F1;
	m_strToKey["f2"] = SAUCE_SCANCODE_F2;
	m_strToKey["f3"] = SAUCE_SCANCODE_F3;
	m_strToKey["f4"] = SAUCE_SCANCODE_F4;
	m_strToKey["f5"] = SAUCE_SCANCODE_F5;
	m_strToKey["f6"] = SAUCE_SCANCODE_F6;
	m_strToKey["f7"] = SAUCE_SCANCODE_F7;
	m_strToKey["f8"] = SAUCE_SCANCODE_F8;
	m_strToKey["f9"] = SAUCE_SCANCODE_F9;
	m_strToKey["f10"] = SAUCE_SCANCODE_F10;
	m_strToKey["f11"] = SAUCE_SCANCODE_F11;
	m_strToKey["f12"] = SAUCE_SCANCODE_F12;
	m_strToKey["f13"] = SAUCE_SCANCODE_F13;
	m_strToKey["f14"] = SAUCE_SCANCODE_F14;
	m_strToKey["f15"] = SAUCE_SCANCODE_F15;
	m_strToKey["f16"] = SAUCE_SCANCODE_F16;
	m_strToKey["f17"] = SAUCE_SCANCODE_F17;
	m_strToKey["f18"] = SAUCE_SCANCODE_F18;
	m_strToKey["f19"] = SAUCE_SCANCODE_F19;
	m_strToKey["f20"] = SAUCE_SCANCODE_F20;
	m_strToKey["f21"] = SAUCE_SCANCODE_F21;
	m_strToKey["f22"] = SAUCE_SCANCODE_F22;
	m_strToKey["f23"] = SAUCE_SCANCODE_F23;
	m_strToKey["f24"] = SAUCE_SCANCODE_F24;
	m_strToKey["kp0"] = SAUCE_SCANCODE_KP_0;
	m_strToKey["kp1"] = SAUCE_SCANCODE_KP_1;
	m_strToKey["kp2"] = SAUCE_SCANCODE_KP_2;
	m_strToKey["kp3"] = SAUCE_SCANCODE_KP_3;
	m_strToKey["kp4"] = SAUCE_SCANCODE_KP_4;
	m_strToKey["kp5"] = SAUCE_SCANCODE_KP_5;
	m_strToKey["kp6"] = SAUCE_SCANCODE_KP_6;
	m_strToKey["kp7"] = SAUCE_SCANCODE_KP_7;
	m_strToKey["kp8"] = SAUCE_SCANCODE_KP_8;
	m_strToKey["kp9"] = SAUCE_SCANCODE_KP_9;
	m_strToKey["kpdecimal"] = SAUCE_SCANCODE_KP_DECIMAL;
	m_strToKey["kpdivide"] = SAUCE_SCANCODE_KP_DIVIDE;
	m_strToKey["kpmultiply"] = SAUCE_SCANCODE_KP_MULTIPLY;
	m_strToKey["kpminus"] = SAUCE_SCANCODE_KP_MINUS;
	m_strToKey["kpplus"] = SAUCE_SCANCODE_KP_PLUS;
	m_strToKey["kpenter"] = SAUCE_SCANCODE_KP_ENTER;
	m_strToKey["kpequals"] = SAUCE_SCANCODE_KP_EQUALS;
	m_strToKey["lshift"] = m_strToKey["leftshift"] = SAUCE_SCANCODE_LSHIFT;
	m_strToKey["lctrl"] = m_strToKey["leftctrl"] = SAUCE_SCANCODE_LCTRL;
	m_strToKey["lalt"] = m_strToKey["leftalt"] = SAUCE_SCANCODE_LALT;
	m_strToKey["lgui"] = m_strToKey["leftgui"] = SAUCE_SCANCODE_LGUI;
	m_strToKey["rshift"] = m_strToKey["rightshift"] = SAUCE_SCANCODE_RSHIFT;
	m_strToKey["rctrl"] = m_strToKey["rightctrl"] = SAUCE_SCANCODE_RCTRL;
	m_strToKey["ralt"] = m_strToKey["rightalt"] = SAUCE_SCANCODE_RALT;
	m_strToKey["rgui"] = m_strToKey["rightgui"] = SAUCE_SCANCODE_RGUI;
	m_strToKey["menu"] = SAUCE_SCANCODE_MENU;

	m_strToKey["lmb"] = SAUCE_MOUSE_BUTTON_LEFT;
	m_strToKey["rmb"] = SAUCE_MOUSE_BUTTON_RIGHT;
	m_strToKey["wheel"] = SAUCE_MOUSE_BUTTON_MIDDLE;

	m_strToKey["controller_a"] = SAUCE_CONTROLLER_BUTTON_A;
	m_strToKey["controller_b"] = SAUCE_CONTROLLER_BUTTON_B;
	m_strToKey["controller_x"] = SAUCE_CONTROLLER_BUTTON_X;
	m_strToKey["controller_y"] = SAUCE_CONTROLLER_BUTTON_Y;
	m_strToKey["controller_back"] = SAUCE_CONTROLLER_BUTTON_BACK;
	m_strToKey["controller_guide"] = SAUCE_CONTROLLER_BUTTON_GUIDE;
	m_strToKey["controller_start"] = SAUCE_CONTROLLER_BUTTON_START;
	m_strToKey["controller_left_stick"] = SAUCE_CONTROLLER_BUTTON_LEFT_STICK;
	m_strToKey["controller_right_stick"] = SAUCE_CONTROLLER_BUTTON_RIGHT_STICK;
	m_strToKey["controller_left_shoulder"] = SAUCE_CONTROLLER_BUTTON_LEFT_SHOULDER;
	m_strToKey["controller_right_shoulder"] = SAUCE_CONTROLLER_BUTTON_RIGHT_SHOULDER;
	m_strToKey["controller_dpad_up"] = SAUCE_CONTROLLER_BUTTON_DPAD_UP;
	m_strToKey["controller_dpad_down"] = SAUCE_CONTROLLER_BUTTON_DPAD_DOWN;
	m_strToKey["controller_dpad_left"] = SAUCE_CONTROLLER_BUTTON_DPAD_LEFT;
	m_strToKey["controller_dpad_right"] = SAUCE_CONTROLLER_BUTTON_DPAD_RIGHT;

	m_strToKey["controller_axis_leftx"] = SAUCE_CONTROLLER_AXIS_LEFTX;
	m_strToKey["controller_axis_lefty"] = SAUCE_CONTROLLER_AXIS_LEFTY;
	m_strToKey["controller_axis_rightx"] = SAUCE_CONTROLLER_AXIS_RIGHTX;
	m_strToKey["controller_axis_righty"] = SAUCE_CONTROLLER_AXIS_RIGHTY;
	m_strToKey["controller_axis_trigger_left"] = SAUCE_CONTROLLER_AXIS_TRIGGER_LEFT;
	m_strToKey["controller_axis_trigger_right"] = SAUCE_CONTROLLER_AXIS_TRIGGER_RIGHT;

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
	return SAUCE_SCANCODE_UNKNOWN;
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

void InputManager::addController(const uint id)
{
	if(SDL_IsGameController(id))
	{
		// Open game controller
		SDL_GameController *controller = SDL_GameControllerOpen(id);

		// Map joy id to controller pointer
		SDL_Joystick* joystick = SDL_GameControllerGetJoystick(controller);
		SDL_JoystickID joyId = SDL_JoystickInstanceID(joystick);
		m_controllers[joyId] = controller;

		// Set new default controller
		if(!m_defaultController)
		{
			m_defaultController = controller;
		}
	}
}

void InputManager::removeController(const uint joyId)
{
	// Get controller by id
	SDL_GameController *controller = static_cast<SDL_GameController*>(m_controllers[joyId]);

	// Remove controller from map
	m_controllers.erase(joyId);

	// If it was the default controller, pick a new default controller
	if(controller == m_defaultController)
	{
		m_defaultController = m_controllers.empty() ? 0 : m_controllers.begin()->second;
	}

	// Close controller
	SDL_GameControllerClose(controller);
}

void InputManager::updateKeybinds(InputEvent *e)
{
	//if(m_game->isEnabled(SAUCE_BLOCK_BACKGROUND_INPUT) && !m_game->getWindow()->checkFlags(SDL_WINDOW_INPUT_FOCUS)) return;

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

bool InputManager::getKeyState(const InputButton inputButton, ControllerDevice *controller) const
{
	//if(m_game->isEnabled(SAUCE_BLOCK_BACKGROUND_INPUT) && !m_game->getWindow()->checkFlags(SDL_WINDOW_INPUT_FOCUS)) return false;
	switch(inputButton.getType())
	{
		case InputButton::KEYBOARD: return SDL_GetKeyboardState(NULL)[inputButton.getCode()];
		case InputButton::MOUSE: return (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(inputButton.getCode())) != 0;
		case InputButton::CONTROLLER_BUTTON:
		{
			if(inputButton.getCode() == SAUCE_CONTROLLER_BUTTON_LEFT_TRIGGER) return m_leftTrigger/*[controller]*/;
			else if(inputButton.getCode() == SAUCE_CONTROLLER_BUTTON_RIGHT_TRIGGER) return m_rightTrigger/*[controller]*/;
			else return SDL_GameControllerGetButton(static_cast<SDL_GameController*>(controller ? controller : m_defaultController), (SDL_GameControllerButton)inputButton.getCode()) != 0;
		}
		case InputButton::CONTROLLER_AXIS: return getAxisValue((const ControllerAxis)inputButton.getCode(), controller) > m_triggerThreshold;
	}
	return false;
}

bool InputManager::getButtonState(const InputButton inputButton, ControllerDevice *controller) const
{
	return getKeyState(inputButton, controller);
}

float InputManager::getAxisValue(const ControllerAxis axis, ControllerDevice *controller) const
{
	return AXIS_VALUE_TO_FLOAT(SDL_GameControllerGetAxis(static_cast<SDL_GameController*>(controller ? controller : m_defaultController), (SDL_GameControllerAxis)axis));
}

Keybind::Keybind() :
	m_inputButton(),
	m_function()
{
}

Keybind::Keybind(InputButton inputButton, function<void(InputEvent*)> func, const uint flags) :
	m_inputButton(inputButton),
	m_function(func)
{
}

/*Keycode Keybind::getKeycode() const
{
	return (Keycode) SDL_GetKeyFromScancode((SDL_Scancode) m_scancode);
}

void Keybind::setKeycode(const Keycode keycode)
{
	m_scancode = (Scancode) SDL_GetScancodeFromKey((SDL_Keycode) keycode);
}*/

END_SAUCE_NAMESPACE
