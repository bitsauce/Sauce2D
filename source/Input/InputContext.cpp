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

BEGIN_SAUCE_NAMESPACE

InputContext::InputContext(InputManager *mgr) :
	m_inputManager(mgr)
{
}

void InputContext::addKeybind(const string &name, Keybind *keybind)
{
	if(!name.empty() && keybind)
	{
		m_keybinds[name] = keybind;
	}
}

void InputContext::removeKeybind(Keybind *keybind)
{
	for(map<string, Keybind*>::iterator itr = m_keybinds.begin(); itr != m_keybinds.end(); itr++)
	{
		if(itr->second == keybind)
		{
			m_keybinds.erase(itr);
			break;
		}
	}
}

void InputContext::removeKeybind(const string &name)
{
	map<string, Keybind*>::iterator itr;
	if((itr = m_keybinds.find(name)) != m_keybinds.end())
	{
		m_keybinds.erase(itr);
	}
}

bool InputContext::getKeyState(string name) const
{
	map<string, Keybind*>::const_iterator itr;
	if((itr = m_keybinds.find(name)) == m_keybinds.end())
	{
		return false;
	}
	return m_inputManager->getKeyState(itr->second->getInputButton());
}

void InputContext::updateKeybinds(InputEvent *e)
{
	for(map<string, Keybind*>::iterator itr = m_keybinds.begin(); itr != m_keybinds.end(); ++itr)
	{
		Keybind* kb = itr->second;
		if(kb->getInputButton() == e->getInputButton() && kb->getFunction())
		{
			kb->getFunction()(e);
		}
	}
}

END_SAUCE_NAMESPACE
