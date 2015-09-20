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

map<VirtualKey, Input::KeyBind> Input::s_keyBindings;
list<KeyListener*> Input::s_keyListeners;
list<MouseListener*> Input::s_mouseListeners;

Vector2 Input::s_position;

KeyListener::KeyListener()
{
	Input::addKeyListener(this);
}

KeyListener::~KeyListener()
{
	Input::removeKeyListener(this);
}

MouseListener::MouseListener()
{
	Input::addMouseListener(this);
}

MouseListener::~MouseListener()
{
	Input::removeMouseListener(this);
}

void Input::bind(const VirtualKey key, function<void()> function)
{
	if(function)
	{
		// Bind function to key
		s_keyBindings[key].function = function;
		s_keyBindings[key].pressed = false;
	}
	else if(s_keyBindings.find(key) != s_keyBindings.end())
	{
		// Remove key binding
		s_keyBindings.erase(key);
	}
}

void Input::unbind(const VirtualKey key)
{
	bind(key, function<void()>());
}

void Input::unbindAll()
{
	s_keyBindings.clear();
}

void Input::addKeyListener(KeyListener *object)
{
	// Add keyboard listener
	if(object)
	{
		s_keyListeners.push_back(object);
	}
}

void Input::removeKeyListener(KeyListener *object)
{
	s_keyListeners.remove(object);
}

void Input::charEvent(const uint c)
{
	if(Engine::isEnabled(XD_BLOCK_BACKGROUND_INPUT) && !Window::hasFocus()) return;
	for(list<KeyListener*>::iterator itr = s_keyListeners.begin(); itr != s_keyListeners.end(); ++itr)
	{
		(*itr)->charEvent(c);
	}
}

void Input::keyPressed(const VirtualKey key)
{
	if(Engine::isEnabled(XD_BLOCK_BACKGROUND_INPUT) && !Window::hasFocus()) return;
	for(list<KeyListener*>::iterator itr = s_keyListeners.begin(); itr != s_keyListeners.end(); ++itr)
	{
		(*itr)->keyPressEvent(key);
	}
}

void Input::keyReleased(const VirtualKey key)
{
	if(Engine::isEnabled(XD_BLOCK_BACKGROUND_INPUT) && !Window::hasFocus()) return;
	for(list<KeyListener*>::iterator itr = s_keyListeners.begin(); itr != s_keyListeners.end(); ++itr)
	{
		(*itr)->keyReleaseEvent(key);
	}
}

void Input::addMouseListener(MouseListener *object)
{
	// Add keyboard listener
	if(object) {
		s_mouseListeners.push_back(object);
	}
}

void Input::removeMouseListener(MouseListener *object)
{
	s_mouseListeners.remove(object);
}

void Input::mouseScroll(const int dt)
{
	if(Engine::isEnabled(XD_BLOCK_BACKGROUND_INPUT) && !Window::hasFocus()) return;
	for(list<MouseListener*>::iterator itr = s_mouseListeners.begin(); itr != s_mouseListeners.end(); ++itr)
	{
		(*itr)->mouseWheelEvent(dt);
	}
}

void Input::checkBindings()
{
	if(Engine::isEnabled(XD_BLOCK_BACKGROUND_INPUT) && !Window::hasFocus()) return;

	// Iterate key bindings
	//map<VirtualKey, KeyBind> mutableCopy(s_keyBindings);
	for(map<VirtualKey, KeyBind>::iterator itr = s_keyBindings.begin(); itr != s_keyBindings.end(); ++itr)
	{
		KeyBind &key = itr->second;
		if(getKeyState(itr->first))
		{
			if(!key.pressed)
			{
				// Key was pressed, call function
				key.function();
			}
			key.pressed = true;
		}
		else
		{
			key.pressed = false;
		}
	}
}

Vector2i Input::getCursorPos()
{
	POINT p;
	GetCursorPos(&p);
	return Vector2i(p.x, p.y);
}

void Input::setCursorPos(const Vector2i &pos)
{
	SetCursorPos(pos.x, pos.y);
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

bool Input::getKeyState(const VirtualKey key)
{
	return (Engine::isEnabled(XD_BLOCK_BACKGROUND_INPUT) && !Window::hasFocus()) ? false : glfwGetKey(Window::s_window, key) == GLFW_PRESS;
}

END_XD_NAMESPACE