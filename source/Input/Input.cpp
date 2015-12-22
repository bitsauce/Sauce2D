//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <CGF/Common.h>
#include <CGF/Math.h>
#include <CGF/Input.h>

BEGIN_CGF_NAMESPACE

Vector2 Input::s_position;
InputContext *Input::s_context = 0;
map<string, InputContext*> Input::s_contextMap;
map<string, VirtualKey> Input::s_strToKey;
map<VirtualKey, int> Input::s_mouseButtonState;
Game *Input::s_game = 0;

void Input::init(string file)
{
	// Set all str to key mappings
	s_strToKey["space"] = CGF_KEY_SPACE;
	s_strToKey["apostrophe"] = CGF_KEY_APOSTROPHE;
	s_strToKey["comma"] = CGF_KEY_COMMA;
	s_strToKey["minus"] = CGF_KEY_MINUS;
	s_strToKey["period"] = CGF_KEY_PERIOD;
	s_strToKey["slash"] = CGF_KEY_SLASH;
	s_strToKey["0"] = CGF_KEY_0;
	s_strToKey["1"] = CGF_KEY_1;
	s_strToKey["2"] = CGF_KEY_2;
	s_strToKey["3"] = CGF_KEY_3;
	s_strToKey["4"] = CGF_KEY_4;
	s_strToKey["5"] = CGF_KEY_5;
	s_strToKey["6"] = CGF_KEY_6;
	s_strToKey["7"] = CGF_KEY_7;
	s_strToKey["8"] = CGF_KEY_8;
	s_strToKey["9"] = CGF_KEY_9;
	s_strToKey["semicolon"] = CGF_KEY_SEMICOLON;
	s_strToKey["equal"] = CGF_KEY_EQUAL;
	s_strToKey["a"] = CGF_KEY_A;
	s_strToKey["b"] = CGF_KEY_B;
	s_strToKey["c"] = CGF_KEY_C;
	s_strToKey["d"] = CGF_KEY_D;
	s_strToKey["e"] = CGF_KEY_E;
	s_strToKey["f"] = CGF_KEY_F;
	s_strToKey["g"] = CGF_KEY_G;
	s_strToKey["h"] = CGF_KEY_H;
	s_strToKey["i"] = CGF_KEY_I;
	s_strToKey["j"] = CGF_KEY_J;
	s_strToKey["k"] = CGF_KEY_K;
	s_strToKey["l"] = CGF_KEY_L;
	s_strToKey["m"] = CGF_KEY_M;
	s_strToKey["n"] = CGF_KEY_N;
	s_strToKey["o"] = CGF_KEY_O;
	s_strToKey["p"] = CGF_KEY_P;
	s_strToKey["q"] = CGF_KEY_Q;
	s_strToKey["r"] = CGF_KEY_R;
	s_strToKey["s"] = CGF_KEY_S;
	s_strToKey["t"] = CGF_KEY_T;
	s_strToKey["u"] = CGF_KEY_U;
	s_strToKey["v"] = CGF_KEY_V;
	s_strToKey["w"] = CGF_KEY_W;
	s_strToKey["x"] = CGF_KEY_X;
	s_strToKey["y"] = CGF_KEY_Y;
	s_strToKey["z"] = CGF_KEY_Z;
	s_strToKey["left_bracket"] = CGF_KEY_LEFT_BRACKET;
	s_strToKey["backslash"] = CGF_KEY_BACKSLASH;
	s_strToKey["right_bracket"] = CGF_KEY_RIGHT_BRACKET;
	s_strToKey["accent"] = CGF_KEY_GRAVE_ACCENT;
	s_strToKey["world_1"] = CGF_KEY_WORLD_1;
	s_strToKey["world_2"] = CGF_KEY_WORLD_2;
	s_strToKey["escape"] = CGF_KEY_ESCAPE;
	s_strToKey["enter"] = s_strToKey["return"] = CGF_KEY_ENTER;
	s_strToKey["tab"] = CGF_KEY_TAB;
	s_strToKey["backspace"] = CGF_KEY_BACKSPACE;
	s_strToKey["insert"] = CGF_KEY_INSERT;
	s_strToKey["delete"] = CGF_KEY_DELETE;
	s_strToKey["right"] = CGF_KEY_RIGHT;
	s_strToKey["left"] = CGF_KEY_LEFT;
	s_strToKey["down"] = CGF_KEY_DOWN;
	s_strToKey["up"] = CGF_KEY_UP;
	s_strToKey["page_up"] = CGF_KEY_PAGE_UP;
	s_strToKey["page_down"] = CGF_KEY_PAGE_DOWN;
	s_strToKey["home"] = CGF_KEY_HOME;
	s_strToKey["end"] = CGF_KEY_END;
	s_strToKey["caps_lock"] = CGF_KEY_CAPS_LOCK;
	s_strToKey["scroll_lock"] = CGF_KEY_SCROLL_LOCK;
	s_strToKey["num_lock"] = CGF_KEY_NUM_LOCK;
	s_strToKey["print_screen"] = CGF_KEY_PRINT_SCREEN;
	s_strToKey["pause"] = CGF_KEY_PAUSE;
	s_strToKey["f1"] = CGF_KEY_F1;
	s_strToKey["f2"] = CGF_KEY_F2;
	s_strToKey["f3"] = CGF_KEY_F3;
	s_strToKey["f4"] = CGF_KEY_F4;
	s_strToKey["f5"] = CGF_KEY_F5;
	s_strToKey["f6"] = CGF_KEY_F6;
	s_strToKey["f7"] = CGF_KEY_F7;
	s_strToKey["f8"] = CGF_KEY_F8;
	s_strToKey["f9"] = CGF_KEY_F9;
	s_strToKey["f10"] = CGF_KEY_F10;
	s_strToKey["f11"] = CGF_KEY_F11;
	s_strToKey["f12"] = CGF_KEY_F12;
	s_strToKey["f13"] = CGF_KEY_F13;
	s_strToKey["f14"] = CGF_KEY_F14;
	s_strToKey["f15"] = CGF_KEY_F15;
	s_strToKey["f16"] = CGF_KEY_F16;
	s_strToKey["f17"] = CGF_KEY_F17;
	s_strToKey["f18"] = CGF_KEY_F18;
	s_strToKey["f19"] = CGF_KEY_F19;
	s_strToKey["f20"] = CGF_KEY_F20;
	s_strToKey["f21"] = CGF_KEY_F21;
	s_strToKey["f22"] = CGF_KEY_F22;
	s_strToKey["f23"] = CGF_KEY_F23;
	s_strToKey["f24"] = CGF_KEY_F24;
	s_strToKey["f25"] = CGF_KEY_F25;
	s_strToKey["keypad_0"] = CGF_KEY_KP_0;
	s_strToKey["keypad_1"] = CGF_KEY_KP_1;
	s_strToKey["keypad_2"] = CGF_KEY_KP_2;
	s_strToKey["keypad_3"] = CGF_KEY_KP_3;
	s_strToKey["keypad_4"] = CGF_KEY_KP_4;
	s_strToKey["keypad_5"] = CGF_KEY_KP_5;
	s_strToKey["keypad_6"] = CGF_KEY_KP_6;
	s_strToKey["keypad_7"] = CGF_KEY_KP_7;
	s_strToKey["keypad_8"] = CGF_KEY_KP_8;
	s_strToKey["keypad_9"] = CGF_KEY_KP_9;
	s_strToKey["keypad_decimal"] = CGF_KEY_KP_DECIMAL;
	s_strToKey["keypad_divide"] = CGF_KEY_KP_DIVIDE;
	s_strToKey["keypad_multiply"] = CGF_KEY_KP_MULTIPLY;
	s_strToKey["keypad_subtract"] = CGF_KEY_KP_SUBTRACT;
	s_strToKey["keypad_add"] = CGF_KEY_KP_ADD;
	s_strToKey["keypad_enter"] = CGF_KEY_KP_ENTER;
	s_strToKey["keypad_equal"] = CGF_KEY_KP_EQUAL;
	s_strToKey["left_shift"] = CGF_KEY_LEFT_SHIFT;
	s_strToKey["left_control"] = CGF_KEY_LEFT_CONTROL;
	s_strToKey["left_alt"] = CGF_KEY_LEFT_ALT;
	s_strToKey["left_super"] = CGF_KEY_LEFT_SUPER;
	s_strToKey["right_shift"] = CGF_KEY_RIGHT_SHIFT;
	s_strToKey["right_control"] = CGF_KEY_RIGHT_CONTROL;
	s_strToKey["right_alt"] = CGF_KEY_RIGHT_ALT;
	s_strToKey["right_super"] = CGF_KEY_RIGHT_SUPER;
	s_strToKey["menu"] = CGF_KEY_MENU;
	s_strToKey["mouse_button_1"] = CGF_MOUSE_BUTTON_1;
	s_strToKey["mouse_button_2"] = CGF_MOUSE_BUTTON_2;
	s_strToKey["mouse_button_3"] = CGF_MOUSE_BUTTON_3;
	s_strToKey["mouse_button_4"] = CGF_MOUSE_BUTTON_4;
	s_strToKey["mouse_button_5"] = CGF_MOUSE_BUTTON_5;
	s_strToKey["mouse_button_6"] = CGF_MOUSE_BUTTON_6;
	s_strToKey["mouse_button_7"] = CGF_MOUSE_BUTTON_7;
	s_strToKey["mouse_button_8"] = CGF_MOUSE_BUTTON_8;
	s_strToKey["mouse_button_left"] = CGF_MOUSE_BUTTON_LEFT;
	s_strToKey["mouse_button_right"] = CGF_MOUSE_BUTTON_RIGHT;
	s_strToKey["mouse_button_middle"] = CGF_MOUSE_BUTTON_MIDDLE;

	// Set button states
	for(uint i = CGF_MOUSE_BUTTON_1; i < CGF_MOUSE_BUTTON_LAST; ++i)
	{
		//s_mouseButtonState[i] = GLFW_RELEASE;
	}

	// Load input config file
	if(util::fileExists(file))
	{
		tinyxml2::XMLDocument doc;
		util::toAbsoluteFilePath(file);
		doc.LoadFile(file.c_str());

		// Get root node
		tinyxml2::XMLNode *contextNode = doc.FirstChild();
		if(!contextNode)
		{
			LOG("Invalid input config file");
			return;
		}

		// For each context
		contextNode = contextNode->FirstChild();
		while(contextNode)
		{
			InputContext *inputContext = new InputContext();

			// For each key bind
			tinyxml2::XMLNode *node = contextNode->FirstChild();
			while(node)
			{
				// Get name and key
				tinyxml2::XMLElement *name = node->FirstChildElement("name");
				tinyxml2::XMLElement *key = node->FirstChildElement("key");
				if(name && key)
				{
					VirtualKey vk = strToKey(key->GetText());
					if(vk != CGF_KEY_UNKNOWN)
					{
						inputContext->m_nameToKey[name->GetText()] = vk;
					}
					else
					{
						LOG("Input::loadInputConfig(): Unknown key '%s'", key->GetText());
					}
				}
				node = node->NextSibling();
			}
			s_contextMap[contextNode->Value()] = inputContext;
			contextNode = contextNode->NextSibling();
		}
	}
}

Vector2i Input::getCursorPos()
{
	double x, y;
	//glfwGetCursorPos(Window::s_window, &x, &y);
	return Vector2i((int) x, (int) y);
}

void Input::setCursorPos(const Vector2i &pos)
{
	//glfwSetCursorPos(Window::s_window, pos.x, pos.y);
}

void Input::setCursorLimits(const Recti &area)
{
	RECT rect;
	rect.top    = (long)(area.getY());
	rect.bottom	= (long)(area.getY()+area.getHeight());
	rect.left   = (long)(area.getX());
	rect.right  = (long)(area.getX()+area.getHeight());
	ClipCursor(&rect);
}

Vector2 Input::getPosition()
{
	return s_position;
}

VirtualKey Input::strToKey(string name)
{
	transform(name.begin(), name.end(), name.begin(), ::tolower);
	map<string, VirtualKey>::iterator itr;
	if((itr = s_strToKey.find(name)) != s_strToKey.end())
	{
		return itr->second;
	}
	return CGF_KEY_UNKNOWN;
}

void Input::setContext(InputContext * inputContext)
{
	// Set current key state for the keys in the context
	if(inputContext)
	for(map<string, InputContext::KeyBind>::iterator itr = inputContext->m_nameToFunc.begin(); itr != inputContext->m_nameToFunc.end(); ++itr)
	{
		//itr->second.pressed = Input::getKeyState(inputContext->m_nameToKey[itr->first]) == GLFW_PRESS;
	}
	s_context = inputContext;
}

InputContext * Input::getContext(const string & name)
{
	if(s_contextMap.find(name) == s_contextMap.end())
	{
		LOG("No input context with name '%s'", name);
		return 0;
	}
	return s_contextMap[name];
}

string Input::getClipboardString()
{
	return SDL_GetClipboardText();
}

void Input::setClipboardString(const string str)
{
	SDL_SetClipboardText(str.c_str());
}

void Input::updateBindings()
{
	if(s_context)
	{
		s_context->updateBindings();
	}
}

int Input::getKeyState(const SDL_Scancode scancode)
{
	if(s_game->isEnabled(CGF_BLOCK_BACKGROUND_INPUT) && !s_game->getMainWindow()->checkFlags(SDL_WINDOW_INPUT_FOCUS)) return false;

	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	return currentKeyStates[scancode];
}

END_CGF_NAMESPACE
