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

InputContext::InputContext(InputManager *mgr) :
	m_inputManager(mgr)
{
}

void InputContext::addKeybind(KeybindPtr keybind)
{
	if(keybind->getKeyname().isValid())
	{
		m_keybinds[keybind->getKeyname().getName()] = keybind;
	}
}

void InputContext::removeKeybind(KeybindPtr keybind)
{
	if(keybind->getKeyname().isValid())
	{
		map<string, KeybindPtr>::iterator itr;
		if((itr = m_keybinds.find(keybind->getKeyname().getName())) == m_keybinds.end())
		{
			m_keybinds.erase(itr);
		}
	}
}

bool InputContext::getKeyState(string name) const
{
	map<string, KeybindPtr>::const_iterator itr;
	if((itr = m_keybinds.find(name)) == m_keybinds.end())
	{
		return false;
	}
	return m_inputManager->getKeyState(itr->second->getKeyname().getKeycode());
}

void InputContext::updateKeybinds(KeyEvent *e)
{
	for(map<string, KeybindPtr>::iterator itr = m_keybinds.begin(); itr != m_keybinds.end(); ++itr)
	{
		KeybindPtr kb = itr->second;
		if(kb->getKeyname().getKeycode() == e->getKeycode() && kb->getFunction())
		{
			kb->getFunction()(e);
		}
	}
}

END_CGF_NAMESPACE
