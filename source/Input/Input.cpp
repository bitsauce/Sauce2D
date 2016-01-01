#include "..\..\include\CGF\Input.h"
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

InputManager::InputManager(string contextFile) :
	m_context(0)
{
	// Set all str to key mappings
	m_strToKey["space"] = CGF_KEY_SPACE;
	m_strToKey["quote"] = m_strToKey["apostrophe"] = CGF_KEY_QUOTE;
	m_strToKey["comma"] = CGF_KEY_COMMA;
	m_strToKey["minus"] = CGF_KEY_MINUS;
	m_strToKey["period"] = CGF_KEY_PERIOD;
	m_strToKey["slash"] = CGF_KEY_SLASH;
	m_strToKey["0"] = CGF_KEY_0;
	m_strToKey["1"] = CGF_KEY_1;
	m_strToKey["2"] = CGF_KEY_2;
	m_strToKey["3"] = CGF_KEY_3;
	m_strToKey["4"] = CGF_KEY_4;
	m_strToKey["5"] = CGF_KEY_5;
	m_strToKey["6"] = CGF_KEY_6;
	m_strToKey["7"] = CGF_KEY_7;
	m_strToKey["8"] = CGF_KEY_8;
	m_strToKey["9"] = CGF_KEY_9;
	m_strToKey["semicolon"] = CGF_KEY_SEMICOLON;
	m_strToKey["equals"] = CGF_KEY_EQUALS;
	m_strToKey["a"] = CGF_KEY_A;
	m_strToKey["b"] = CGF_KEY_B;
	m_strToKey["c"] = CGF_KEY_C;
	m_strToKey["d"] = CGF_KEY_D;
	m_strToKey["e"] = CGF_KEY_E;
	m_strToKey["f"] = CGF_KEY_F;
	m_strToKey["g"] = CGF_KEY_G;
	m_strToKey["h"] = CGF_KEY_H;
	m_strToKey["i"] = CGF_KEY_I;
	m_strToKey["j"] = CGF_KEY_J;
	m_strToKey["k"] = CGF_KEY_K;
	m_strToKey["l"] = CGF_KEY_L;
	m_strToKey["m"] = CGF_KEY_M;
	m_strToKey["n"] = CGF_KEY_N;
	m_strToKey["o"] = CGF_KEY_O;
	m_strToKey["p"] = CGF_KEY_P;
	m_strToKey["q"] = CGF_KEY_Q;
	m_strToKey["r"] = CGF_KEY_R;
	m_strToKey["s"] = CGF_KEY_S;
	m_strToKey["t"] = CGF_KEY_T;
	m_strToKey["u"] = CGF_KEY_U;
	m_strToKey["v"] = CGF_KEY_V;
	m_strToKey["w"] = CGF_KEY_W;
	m_strToKey["x"] = CGF_KEY_X;
	m_strToKey["y"] = CGF_KEY_Y;
	m_strToKey["z"] = CGF_KEY_Z;
	m_strToKey["lbracket"] = m_strToKey["leftbracket"] = CGF_KEY_LEFTBRACKET;
	m_strToKey["backslash"] = CGF_KEY_BACKSLASH;
	m_strToKey["rightbracket"] = CGF_KEY_RIGHTBRACKET;
	m_strToKey["backquote"] = CGF_KEY_BACKQUOTE;
	m_strToKey["underscore"] = CGF_KEY_UNDERSCORE;
	m_strToKey["at"] = CGF_KEY_AT;
	m_strToKey["escape"] = CGF_KEY_ESCAPE;
	m_strToKey["enter"] = m_strToKey["return"] = CGF_KEY_RETURN;
	m_strToKey["tab"] = CGF_KEY_TAB;
	m_strToKey["backspace"] = CGF_KEY_BACKSPACE;
	m_strToKey["insert"] = CGF_KEY_INSERT;
	m_strToKey["delete"] = CGF_KEY_DELETE;
	m_strToKey["right"] = CGF_KEY_RIGHT;
	m_strToKey["left"] = CGF_KEY_LEFT;
	m_strToKey["down"] = CGF_KEY_DOWN;
	m_strToKey["up"] = CGF_KEY_UP;
	m_strToKey["pageup"] = CGF_KEY_PAGEUP;
	m_strToKey["pagedown"] = CGF_KEY_PAGEDOWN;
	m_strToKey["home"] = CGF_KEY_HOME;
	m_strToKey["end"] = CGF_KEY_END;
	m_strToKey["capslock"] = CGF_KEY_CAPSLOCK;
	m_strToKey["scrolllock"] = CGF_KEY_SCROLLLOCK;
	m_strToKey["numlock"] = CGF_KEY_NUMLOCKCLEAR;
	m_strToKey["printscreen"] = CGF_KEY_PRINTSCREEN;
	m_strToKey["pause"] = CGF_KEY_PAUSE;
	m_strToKey["f1"] = CGF_KEY_F1;
	m_strToKey["f2"] = CGF_KEY_F2;
	m_strToKey["f3"] = CGF_KEY_F3;
	m_strToKey["f4"] = CGF_KEY_F4;
	m_strToKey["f5"] = CGF_KEY_F5;
	m_strToKey["f6"] = CGF_KEY_F6;
	m_strToKey["f7"] = CGF_KEY_F7;
	m_strToKey["f8"] = CGF_KEY_F8;
	m_strToKey["f9"] = CGF_KEY_F9;
	m_strToKey["f10"] = CGF_KEY_F10;
	m_strToKey["f11"] = CGF_KEY_F11;
	m_strToKey["f12"] = CGF_KEY_F12;
	m_strToKey["f13"] = CGF_KEY_F13;
	m_strToKey["f14"] = CGF_KEY_F14;
	m_strToKey["f15"] = CGF_KEY_F15;
	m_strToKey["f16"] = CGF_KEY_F16;
	m_strToKey["f17"] = CGF_KEY_F17;
	m_strToKey["f18"] = CGF_KEY_F18;
	m_strToKey["f19"] = CGF_KEY_F19;
	m_strToKey["f20"] = CGF_KEY_F20;
	m_strToKey["f21"] = CGF_KEY_F21;
	m_strToKey["f22"] = CGF_KEY_F22;
	m_strToKey["f23"] = CGF_KEY_F23;
	m_strToKey["f24"] = CGF_KEY_F24;
	m_strToKey["kp0"] = CGF_KEY_KP_0;
	m_strToKey["kp1"] = CGF_KEY_KP_1;
	m_strToKey["kp2"] = CGF_KEY_KP_2;
	m_strToKey["kp3"] = CGF_KEY_KP_3;
	m_strToKey["kp4"] = CGF_KEY_KP_4;
	m_strToKey["kp5"] = CGF_KEY_KP_5;
	m_strToKey["kp6"] = CGF_KEY_KP_6;
	m_strToKey["kp7"] = CGF_KEY_KP_7;
	m_strToKey["kp8"] = CGF_KEY_KP_8;
	m_strToKey["kp9"] = CGF_KEY_KP_9;
	m_strToKey["kpdecimal"] = CGF_KEY_KP_DECIMAL;
	m_strToKey["kpdivide"] = CGF_KEY_KP_DIVIDE;
	m_strToKey["kpmultiply"] = CGF_KEY_KP_MULTIPLY;
	m_strToKey["kpminus"] = CGF_KEY_KP_MINUS;
	m_strToKey["kpplus"] = CGF_KEY_KP_PLUS;
	m_strToKey["kpenter"] = CGF_KEY_KP_ENTER;
	m_strToKey["kpequals"] = CGF_KEY_KP_EQUALS;
	m_strToKey["lshift"] = m_strToKey["leftshift"] = CGF_KEY_LSHIFT;
	m_strToKey["lctrl"] = m_strToKey["leftctrl"] = CGF_KEY_LCTRL;
	m_strToKey["lalt"] = m_strToKey["leftalt"] = CGF_KEY_LALT;
	m_strToKey["lgui"] = m_strToKey["leftgui"] = CGF_KEY_LGUI;
	m_strToKey["rshift"] = m_strToKey["rightshift"] = CGF_KEY_RSHIFT;
	m_strToKey["rctrl"] = m_strToKey["rightctrl"] = CGF_KEY_RCTRL;
	m_strToKey["ralt"] = m_strToKey["rightalt"] = CGF_KEY_RALT;
	m_strToKey["rgui"] = m_strToKey["rightgui"] = CGF_KEY_RGUI;
	m_strToKey["menu"] = CGF_KEY_MENU;

	// Load input config file
	if(util::fileExists(contextFile))
	{
		tinyxml2::XMLDocument doc;
		util::toAbsoluteFilePath(contextFile);
		doc.LoadFile(contextFile.c_str());

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
			InputContext *inputContext = new InputContext(this);

			// For each key bind
			tinyxml2::XMLNode *node = contextNode->FirstChild();
			while(node)
			{
				// Get name and key
				tinyxml2::XMLElement *name = node->FirstChildElement("name");
				tinyxml2::XMLElement *key = node->FirstChildElement("key");
				if(name && key)
				{
					Keycode vk = strToKey(key->GetText());
					if(vk != CGF_KEY_UNKNOWN)
					{
						//inputContext->m_nameToKey[name->GetText()] = vk;
					}
					else
					{
						LOG("Input::loadInputConfig(): Unknown key '%s'", key->GetText());
					}
				}
				node = node->NextSibling();
			}
			m_contextMap[contextNode->Value()] = inputContext;
			contextNode = contextNode->NextSibling();
		}
	}
}

void InputManager::getPosition(Sint32 &x, Sint32 &y) const
{
	x = m_x;
	y = m_y;
}

Sint32 InputManager::getX() const
{
	return m_x;
}

Sint32 InputManager::getY() const
{
	return m_y;
}

Keycode InputManager::strToKey(string name)
{
	transform(name.begin(), name.end(), name.begin(), ::tolower);
	map<string, Keycode>::iterator itr;
	if((itr = m_strToKey.find(name)) != m_strToKey.end())
	{
		return itr->second;
	}
	return CGF_KEY_UNKNOWN;
}

void InputManager::setContext(InputContext *inputContext)
{
	// Set current key state for the keys in the context
	if(inputContext)
	//for(map<string, InputContext::KeyBind>::iterator itr = inputContext->m_nameToFunc.begin(); itr != inputContext->m_nameToFunc.end(); ++itr)
	{
		//itr->second.pressed = Input::getKeyState(inputContext->m_nameToKey[itr->first]) == GLFW_PRESS;
	}
	m_context = inputContext;
}

InputContext *InputManager::getContext()
{
	return m_context;
	/*if(m_contextMap.find(name) == m_contextMap.end())
	{
		LOG("No input context with name '%s'", name);
		return 0;
	}
	return m_contextMap[name];*/
}

string InputManager::getClipboardString()
{
	return SDL_GetClipboardText();
}

void InputManager::setClipboardString(const string str)
{
	SDL_SetClipboardText(str.c_str());
}

void InputManager::addKeybind(KeybindPtr keybind)
{
	m_keybinds.push_back(keybind);
}

void InputManager::removeKeybind(KeybindPtr keybind)
{
	m_keybinds.remove(keybind);
}

void InputManager::updateKeybinds(KeyEvent *e)
{
	//if(m_game->isEnabled(CGF_BLOCK_BACKGROUND_INPUT) && !m_game->getWindow()->checkFlags(SDL_WINDOW_INPUT_FOCUS)) return;

	// Update keybinds
	for(KeybindPtr kb : m_keybinds)
	{
		if(kb->getKeyname().getKeycode() == e->getKeycode() && kb->getFunction())
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

bool InputManager::getKeyState(const Keycode keycode) const
{
	return getKeyState((Scancode) SDL_GetScancodeFromKey(keycode));
}

bool InputManager::getKeyState(const Scancode scancode) const
{
	//if(m_game->isEnabled(CGF_BLOCK_BACKGROUND_INPUT) && !m_game->getWindow()->checkFlags(SDL_WINDOW_INPUT_FOCUS)) return false;
 
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	return currentKeyStates[scancode];
}

END_CGF_NAMESPACE