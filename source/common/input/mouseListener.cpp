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

list<MouseListener*> MouseListener::s_mouseListeners;

MouseListener::MouseListener()
{
	s_mouseListeners.push_back(this);
}

MouseListener::~MouseListener()
{
	s_mouseListeners.remove(this);
}

void MouseListener::callMouseWheelEvent(const int delta)
{
	if(Engine::isEnabled(XD_BLOCK_BACKGROUND_INPUT) && !Window::hasFocus()) return;
	for(list<MouseListener*>::iterator itr = s_mouseListeners.begin(); itr != s_mouseListeners.end(); ++itr)
	{
		(*itr)->mouseWheelEvent(delta);
	}
}

END_XD_NAMESPACE