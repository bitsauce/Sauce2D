//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>

BEGIN_XD_NAMESPACE

Vector2 Input::s_position;
InputContext *Input::s_context = 0;
map<string, VirtualKey> Input::s_strToKey;
map<VirtualKey, int> Input::s_mouseButtonState;

void Input::init()
{
	s_strToKey["space"] = XD_KEY_SPACE;
	s_strToKey["apostrophe"] = XD_KEY_APOSTROPHE;
	s_strToKey["comma"] = XD_KEY_COMMA;
	s_strToKey["minus"] = XD_KEY_MINUS;
	s_strToKey["period"] = XD_KEY_PERIOD;
	s_strToKey["slash"] = XD_KEY_SLASH;
	s_strToKey["0"] = XD_KEY_0;
	s_strToKey["1"] = XD_KEY_1;
	s_strToKey["2"] = XD_KEY_2;
	s_strToKey["3"] = XD_KEY_3;
	s_strToKey["4"] = XD_KEY_4;
	s_strToKey["5"] = XD_KEY_5;
	s_strToKey["6"] = XD_KEY_6;
	s_strToKey["7"] = XD_KEY_7;
	s_strToKey["8"] = XD_KEY_8;
	s_strToKey["9"] = XD_KEY_9;
	s_strToKey["semicolon"] = XD_KEY_SEMICOLON;
	s_strToKey["equal"] = XD_KEY_EQUAL;
	s_strToKey["a"] = XD_KEY_A;
	s_strToKey["b"] = XD_KEY_B;
	s_strToKey["c"] = XD_KEY_C;
	s_strToKey["d"] = XD_KEY_D;
	s_strToKey["e"] = XD_KEY_E;
	s_strToKey["f"] = XD_KEY_F;
	s_strToKey["g"] = XD_KEY_G;
	s_strToKey["h"] = XD_KEY_H;
	s_strToKey["i"] = XD_KEY_I;
	s_strToKey["j"] = XD_KEY_J;
	s_strToKey["k"] = XD_KEY_K;
	s_strToKey["l"] = XD_KEY_L;
	s_strToKey["m"] = XD_KEY_M;
	s_strToKey["n"] = XD_KEY_N;
	s_strToKey["o"] = XD_KEY_O;
	s_strToKey["p"] = XD_KEY_P;
	s_strToKey["q"] = XD_KEY_Q;
	s_strToKey["r"] = XD_KEY_R;
	s_strToKey["s"] = XD_KEY_S;
	s_strToKey["t"] = XD_KEY_T;
	s_strToKey["u"] = XD_KEY_U;
	s_strToKey["v"] = XD_KEY_V;
	s_strToKey["w"] = XD_KEY_W;
	s_strToKey["x"] = XD_KEY_X;
	s_strToKey["y"] = XD_KEY_Y;
	s_strToKey["z"] = XD_KEY_Z;
	s_strToKey["left_bracket"] = XD_KEY_LEFT_BRACKET;
	s_strToKey["backslash"] = XD_KEY_BACKSLASH;
	s_strToKey["right_bracket"] = XD_KEY_RIGHT_BRACKET;
	s_strToKey["accent"] = XD_KEY_GRAVE_ACCENT;
	s_strToKey["world_1"] = XD_KEY_WORLD_1;
	s_strToKey["world_2"] = XD_KEY_WORLD_2;
	s_strToKey["escape"] = XD_KEY_ESCAPE;
	s_strToKey["enter"] = XD_KEY_ENTER;
	s_strToKey["tab"] = XD_KEY_TAB;
	s_strToKey["backspace"] = XD_KEY_BACKSPACE;
	s_strToKey["insert"] = XD_KEY_INSERT;
	s_strToKey["delete"] = XD_KEY_DELETE;
	s_strToKey["right"] = XD_KEY_RIGHT;
	s_strToKey["left"] = XD_KEY_LEFT;
	s_strToKey["down"] = XD_KEY_DOWN;
	s_strToKey["up"] = XD_KEY_UP;
	s_strToKey["page_up"] = XD_KEY_PAGE_UP;
	s_strToKey["page_down"] = XD_KEY_PAGE_DOWN;
	s_strToKey["home"] = XD_KEY_HOME;
	s_strToKey["end"] = XD_KEY_END;
	s_strToKey["caps_lock"] = XD_KEY_CAPS_LOCK;
	s_strToKey["scroll_lock"] = XD_KEY_SCROLL_LOCK;
	s_strToKey["num_lock"] = XD_KEY_NUM_LOCK;
	s_strToKey["print_screen"] = XD_KEY_PRINT_SCREEN;
	s_strToKey["pause"] = XD_KEY_PAUSE;
	s_strToKey["f1"] = XD_KEY_F1;
	s_strToKey["f2"] = XD_KEY_F2;
	s_strToKey["f3"] = XD_KEY_F3;
	s_strToKey["f4"] = XD_KEY_F4;
	s_strToKey["f5"] = XD_KEY_F5;
	s_strToKey["f6"] = XD_KEY_F6;
	s_strToKey["f7"] = XD_KEY_F7;
	s_strToKey["f8"] = XD_KEY_F8;
	s_strToKey["f9"] = XD_KEY_F9;
	s_strToKey["f10"] = XD_KEY_F10;
	s_strToKey["f11"] = XD_KEY_F11;
	s_strToKey["f12"] = XD_KEY_F12;
	s_strToKey["f13"] = XD_KEY_F13;
	s_strToKey["f14"] = XD_KEY_F14;
	s_strToKey["f15"] = XD_KEY_F15;
	s_strToKey["f16"] = XD_KEY_F16;
	s_strToKey["f17"] = XD_KEY_F17;
	s_strToKey["f18"] = XD_KEY_F18;
	s_strToKey["f19"] = XD_KEY_F19;
	s_strToKey["f20"] = XD_KEY_F20;
	s_strToKey["f21"] = XD_KEY_F21;
	s_strToKey["f22"] = XD_KEY_F22;
	s_strToKey["f23"] = XD_KEY_F23;
	s_strToKey["f24"] = XD_KEY_F24;
	s_strToKey["f25"] = XD_KEY_F25;
	s_strToKey["keypad_0"] = XD_KEY_KP_0;
	s_strToKey["keypad_1"] = XD_KEY_KP_1;
	s_strToKey["keypad_2"] = XD_KEY_KP_2;
	s_strToKey["keypad_3"] = XD_KEY_KP_3;
	s_strToKey["keypad_4"] = XD_KEY_KP_4;
	s_strToKey["keypad_5"] = XD_KEY_KP_5;
	s_strToKey["keypad_6"] = XD_KEY_KP_6;
	s_strToKey["keypad_7"] = XD_KEY_KP_7;
	s_strToKey["keypad_8"] = XD_KEY_KP_8;
	s_strToKey["keypad_9"] = XD_KEY_KP_9;
	s_strToKey["keypad_decimal"] = XD_KEY_KP_DECIMAL;
	s_strToKey["keypad_divide"] = XD_KEY_KP_DIVIDE;
	s_strToKey["keypad_multiply"] = XD_KEY_KP_MULTIPLY;
	s_strToKey["keypad_subtract"] = XD_KEY_KP_SUBTRACT;
	s_strToKey["keypad_add"] = XD_KEY_KP_ADD;
	s_strToKey["keypad_enter"] = XD_KEY_KP_ENTER;
	s_strToKey["keypad_equal"] = XD_KEY_KP_EQUAL;
	s_strToKey["left_shift"] = XD_KEY_LEFT_SHIFT;
	s_strToKey["left_control"] = XD_KEY_LEFT_CONTROL;
	s_strToKey["left_alt"] = XD_KEY_LEFT_ALT;
	s_strToKey["left_super"] = XD_KEY_LEFT_SUPER;
	s_strToKey["right_shift"] = XD_KEY_RIGHT_SHIFT;
	s_strToKey["right_control"] = XD_KEY_RIGHT_CONTROL;
	s_strToKey["right_alt"] = XD_KEY_RIGHT_ALT;
	s_strToKey["right_super"] = XD_KEY_RIGHT_SUPER;
	s_strToKey["menu"] = XD_KEY_MENU;
	s_strToKey["mouse_button_1"] = XD_MOUSE_BUTTON_1;
	s_strToKey["mouse_button_2"] = XD_MOUSE_BUTTON_2;
	s_strToKey["mouse_button_3"] = XD_MOUSE_BUTTON_3;
	s_strToKey["mouse_button_4"] = XD_MOUSE_BUTTON_4;
	s_strToKey["mouse_button_5"] = XD_MOUSE_BUTTON_5;
	s_strToKey["mouse_button_6"] = XD_MOUSE_BUTTON_6;
	s_strToKey["mouse_button_7"] = XD_MOUSE_BUTTON_7;
	s_strToKey["mouse_button_8"] = XD_MOUSE_BUTTON_8;
	s_strToKey["mouse_button_left"] = XD_MOUSE_BUTTON_LEFT;
	s_strToKey["mouse_button_right"] = XD_MOUSE_BUTTON_RIGHT;
	s_strToKey["mouse_button_middle"] = XD_MOUSE_BUTTON_MIDDLE;

	for(uint i = XD_MOUSE_BUTTON_1; i < XD_MOUSE_BUTTON_LAST; ++i)
	{
		s_mouseButtonState[i] = GLFW_RELEASE;
	}
}

Vector2i Input::getCursorPos()
{
	double x, y;
	glfwGetCursorPos(Window::s_window, &x, &y);
	return Vector2i((int) x, (int) y);
}

void Input::setCursorPos(const Vector2i &pos)
{
	glfwSetCursorPos(Window::s_window, pos.x, pos.y);
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
	return XD_KEY_UNKNOWN;
}

vector<InputContext*> Input::loadInputConfig(string file)
{
	tinyxml2::XMLDocument doc;
	util::toAbsoluteFilePath(file);
	doc.LoadFile(file.c_str());

	vector<InputContext*> inputContexts;

	// For each context
	tinyxml2::XMLNode *contextNode = doc.FirstChild();
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
				if(vk != XD_KEY_UNKNOWN)
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

		inputContexts.push_back(inputContext);
		contextNode = contextNode->NextSibling();
	}

	return inputContexts;
}

void Input::updateBindings()
{
	if(s_context)
	{
		s_context->updateBindings();
	}
}

int Input::getKeyState(const VirtualKey key)
{
	if(Engine::isEnabled(XD_BLOCK_BACKGROUND_INPUT) && !Window::hasFocus()) return false;
	if(key >= XD_MOUSE_BUTTON_1 && key <= XD_MOUSE_BUTTON_LAST)
	{
		return s_mouseButtonState[key];
	}
	return glfwGetKey(Window::s_window, key);
}

END_XD_NAMESPACE