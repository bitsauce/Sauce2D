//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2015 (C)

#include <CGF/Common.h>

BEGIN_CGF_NAMESPACE

void InputContext::updateBindings()
{
	if(m_game->isEnabled(CGF_BLOCK_BACKGROUND_INPUT) && !m_game->getWindow()->checkFlags(SDL_WINDOW_INPUT_FOCUS)) return;

	// Iterate key bindings
	//map<VirtualKey, KeyBind> mutableCopy(s_keyBindings);
	/*for(map<string, KeyBind>::iterator itr = m_nameToFunc.begin(); itr != m_nameToFunc.end(); ++itr)
	{
		KeyBind &key = itr->second;
		if(key.singleShot)
		{
			if(Input::getKeyState(m_nameToKey[itr->first]) == GLFW_PRESS)
			{
				if(!key.pressed)
				{
					key.function(GLFW_PRESS);
				}
				key.pressed = true;
			}
			else
			{
				if(key.pressed)
				{
					key.function(GLFW_RELEASE);
				}
				key.pressed = false;
			}
		}
		else
		{
			key.function(Input::getKeyState(m_nameToKey[itr->first]));
		}
	}*/
}

void InputContext::bind(const string &key, function<void(int)> function, const bool singleShot)
{
	if(m_nameToKey.find(key) != m_nameToKey.end())
	{
		if(function)
		{
			// Bind function to key
			m_nameToFunc[key].function = function;
			m_nameToFunc[key].pressed = false;
			m_nameToFunc[key].singleShot = singleShot;
		}
		else if(m_nameToFunc.find(key) != m_nameToFunc.end())
		{
			// Remove key binding
			m_nameToFunc.erase(key);
		}
	}
	else
	{
		LOG("InputContext::bind(): No key bound to name '%s'", key.c_str());
	}
}

END_CGF_NAMESPACE
