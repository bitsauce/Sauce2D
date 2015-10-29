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

list<KeyListener*> KeyListener::s_keyListeners;

KeyListener::KeyListener()
{
	s_keyListeners.push_back(this);
}

KeyListener::~KeyListener()
{
	s_keyListeners.remove(this);
}

void KeyListener::callCharEvent(const uint c)
{
	if(Engine::isEnabled(XD_BLOCK_BACKGROUND_INPUT) && !Window::hasFocus()) return;
	for(list<KeyListener*>::iterator itr = s_keyListeners.begin(); itr != s_keyListeners.end(); ++itr)
	{
		(*itr)->charEvent(c);
	}
}

void KeyListener::callKeyEvent(const KeyEvent & event)
{
	if(Engine::isEnabled(XD_BLOCK_BACKGROUND_INPUT) && !Window::hasFocus()) return;
	for(list<KeyListener*>::iterator itr = s_keyListeners.begin(); itr != s_keyListeners.end(); ++itr)
	{
		(*itr)->keyEvent(event);
	}
}

END_XD_NAMESPACE