//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>

// TODO: Refactor. This class is a mess
map<XVirtualKey, XInput::KeyBind> XInput::s_keyBindings;
vector<xd::KeyboardListener*> XInput::s_keyListeners;
	
map<XMouseButton, bool> XInput::s_mousePressed;
vector<xd::MouseListener*> XInput::s_mouseListeners;

Vector2 XInput::s_position;

XVirtualKey fromWinKey(uchar vk)
{
	switch(vk)
	{
	case VK_LBUTTON: return XD_LMB;
	case VK_RBUTTON: return XD_RMB; 
	case VK_MBUTTON: return XD_WHEEL; 
	case VK_RIGHT: return XD_KEY_RIGHT; 
	case VK_DOWN: return XD_KEY_DOWN;
	case VK_LEFT: return XD_KEY_LEFT; 
	case VK_UP: return XD_KEY_UP;
	case VK_SHIFT: return XD_KEY_SHIFT; 
	case VK_LSHIFT: return XD_KEY_LSHIFT; 
	case VK_RSHIFT: return XD_KEY_RSHIFT; 
	case VK_CONTROL: return XD_KEY_CONTROL; 
	case VK_LCONTROL: return XD_KEY_LCONTROL; 
	case VK_RCONTROL: return XD_KEY_RCONTROL; 
	case VK_MENU: return XD_KEY_MENU; 
	case VK_LMENU: return XD_KEY_LMENU; 
	case VK_RMENU: return XD_KEY_RMENU; 
	case VK_SPACE: return XD_KEY_SPACE; 
	case VK_NEXT: return XD_KEY_PAGEUP; 
	case VK_PRIOR: return XD_KEY_PAGEDOWN; 
	case VK_END: return XD_KEY_END; 
	case VK_HOME: return XD_KEY_HOME; 
	case VK_SNAPSHOT: return XD_KEY_SNAPSHOT; 
	case VK_INSERT: return XD_KEY_INSERT;
	case VK_DELETE: return XD_KEY_DELETE; 
	case VK_NUMPAD0: return XD_KEY_NUMPAD0; 
	case VK_NUMPAD1: return XD_KEY_NUMPAD1; 
	case VK_NUMPAD2: return XD_KEY_NUMPAD2; 
	case VK_NUMPAD3: return XD_KEY_NUMPAD3; 
	case VK_NUMPAD4: return XD_KEY_NUMPAD4; 
	case VK_NUMPAD5: return XD_KEY_NUMPAD5; 
	case VK_NUMPAD6: return XD_KEY_NUMPAD6; 
	case VK_NUMPAD7: return XD_KEY_NUMPAD7; 
	case VK_NUMPAD8: return XD_KEY_NUMPAD8; 
	case VK_NUMPAD9: return XD_KEY_NUMPAD9; 
	case VK_OEM_PLUS: return XD_KEY_PLUS; 
	//case XD_KEY_MINUS: return VK_OEM_MINUS; break;
	//case XD_KEY_CARET: return VK_OEM_CAR; break;
	//case XD_KEY_APOSTROPHE: return VK_OEM_7; break;
	//case XD_KEY_QUOTATION_MARK: return VK_OEM_2; break;
	//case XD_KEY_BACKSLASH: return VK_OEM_5; break;
	//case XD_KEY_SLASH: return VK_OEM_2; break;
	//case XD_KEY_ASTERISK: return VK_OEM_; break;
	//case XD_KEY_PIPE: return VK_OEM_5; break;
	//case XD_KEY_COLON: return VK_OEM_1; break;
	//case XD_KEY_SEMICOLON: return VK_OEM_1; break;
	//case XD_KEY_TILDE: return ; break;
	//case XD_KEY_COMMA: return VK_OEM_COMMA; break;
	//case XD_KEY_PERIOD: return VK_OEM_PERIOD; break;
	//case XD_KEY_GREATER: return VK_OEM; break;
	//case XD_KEY_LESS: return ; break;
	//case XD_KEY_EQUALS: return VK_; break;
	default: return XVirtualKey(vk);
	}
}

uchar toWinKey(XVirtualKey key)
{
	uchar vk = 0;
	switch(key)
	{
	case XD_LMB: vk = VK_LBUTTON; break;
	case XD_RMB: vk = VK_RBUTTON; break;
	case XD_WHEEL: vk = VK_MBUTTON; break;
	case XD_KEY_RIGHT: vk = VK_RIGHT; break;
	case XD_KEY_DOWN: vk = VK_DOWN; break;
	case XD_KEY_LEFT: vk = VK_LEFT; break;
	case XD_KEY_UP: vk = VK_UP; break;
	case XD_KEY_SHIFT: vk = VK_SHIFT; break;
	case XD_KEY_LSHIFT: vk = VK_LSHIFT; break;
	case XD_KEY_RSHIFT: vk = VK_RSHIFT; break;
	case XD_KEY_CONTROL: vk = VK_CONTROL; break;
	case XD_KEY_LCONTROL: vk = VK_LCONTROL; break;
	case XD_KEY_RCONTROL: vk = VK_RCONTROL; break;
	case XD_KEY_MENU: vk = VK_MENU; break;
	case XD_KEY_LMENU: vk = VK_LMENU; break;
	case XD_KEY_RMENU: vk = VK_RMENU; break;
	case XD_KEY_SPACE: vk = VK_SPACE; break;
	case XD_KEY_PAGEUP: vk = VK_NEXT; break;
	case XD_KEY_PAGEDOWN: vk = VK_PRIOR; break;
	case XD_KEY_END: vk = VK_END; break;
	case XD_KEY_HOME: vk = VK_HOME; break;
	case XD_KEY_SNAPSHOT: vk = VK_SNAPSHOT; break;
	case XD_KEY_INSERT: vk = VK_INSERT; break;
	case XD_KEY_DELETE: vk = VK_DELETE; break;

	case XD_KEY_F1:
	case XD_KEY_F2:
	case XD_KEY_F3:
	case XD_KEY_F4:
	case XD_KEY_F5:
	case XD_KEY_F6:
	case XD_KEY_F7:
	case XD_KEY_F8:
	case XD_KEY_F9:
	case XD_KEY_F10:
	case XD_KEY_F11:
	case XD_KEY_F12: vk = VK_F1 + (key - XD_KEY_F1); break;

	case XD_KEY_NUMPAD0: vk = VK_NUMPAD0; break;
	case XD_KEY_NUMPAD1: vk = VK_NUMPAD1; break;
	case XD_KEY_NUMPAD2: vk = VK_NUMPAD2; break;
	case XD_KEY_NUMPAD3: vk = VK_NUMPAD3; break;
	case XD_KEY_NUMPAD4: vk = VK_NUMPAD4; break;
	case XD_KEY_NUMPAD5: vk = VK_NUMPAD5; break;
	case XD_KEY_NUMPAD6: vk = VK_NUMPAD6; break;
	case XD_KEY_NUMPAD7: vk = VK_NUMPAD7; break;
	case XD_KEY_NUMPAD8: vk = VK_NUMPAD8; break;
	case XD_KEY_NUMPAD9: vk = VK_NUMPAD9; break;
	case XD_KEY_PLUS: vk = VK_OEM_PLUS; break;
	case XD_KEY_MINUS: vk = VK_OEM_MINUS; break;
	//case XD_KEY_CARET: vk = VK_OEM_CAR; break;
	case XD_KEY_APOSTROPHE: vk = VK_OEM_7; break;
	case XD_KEY_QUOTATION_MARK: vk = VK_OEM_2; break;
	case XD_KEY_BACKSLASH: vk = VK_OEM_5; break;
	case XD_KEY_SLASH: vk = VK_OEM_2; break;
	//case XD_KEY_ASTERISK: vk = VK_OEM_; break;
	case XD_KEY_PIPE: vk = VK_OEM_5; break;
	case XD_KEY_COLON: vk = VK_OEM_1; break;
	case XD_KEY_SEMICOLON: vk = VK_OEM_1; break;
	//case XD_KEY_TILDE: vk = ; break;
	case XD_KEY_COMMA: vk = VK_OEM_COMMA; break;
	case XD_KEY_PERIOD: vk = VK_OEM_PERIOD; break;
	//case XD_KEY_GREATER: vk = VK_OEM; break;
	//case XD_KEY_LESS: vk = ; break;
	//case XD_KEY_EQUALS: vk = VK_; break;
	default: return uchar(key);
	}
	return vk;
}

/*XInput::init()
{
	s_mousePressed[XD_MOUSE_LEFT] = s_mousePressed[XD_MOUSE_RIGHT] = s_mousePressed[XD_MOUSE_MIDDLE] = true;
}

XInput::~XInput()
{
	for(map<XVirtualKey, KeyBind>::iterator itr = m_keyBindings.begin(); itr != m_keyBindings.end(); ++itr)
	{
		// Release all function handles
		itr->second.function->Release();
	}
	
	for(vector<asIScriptObject*>::iterator itr = m_keyListeners.begin(); itr != m_keyListeners.end(); ++itr)
	{
		// Release keyboard listeners
		(*itr)->Release();
	}
	
	for(vector<asIScriptObject*>::iterator itr = m_clickListeners.begin(); itr != m_clickListeners.end(); ++itr)
	{
		// Release mouse listeners
		(*itr)->Release();
	}
}*/

void XInput::bind(const XVirtualKey key, function<void()> function)
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

void XInput::unbind(const XVirtualKey key)
{
	bind(key, function<void()>());
}

void XInput::resetBindings()
{
	s_keyBindings.clear();
}

void XInput::addKeyboardListener(xd::KeyboardListener *object)
{
	// Add keyboard listener
	if(object)
	{
		s_keyListeners.push_back(object);
	}
}

void XInput::charEvent(const wchar_t c)
{
	for(vector<xd::KeyboardListener*>::iterator itr = s_keyListeners.begin(); itr != s_keyListeners.end(); ++itr)
	{
		(*itr)->charEvent(c);
	}
}

void XInput::keyPressed(const XVirtualKey key)
{
	for(vector<xd::KeyboardListener*>::iterator itr = s_keyListeners.begin(); itr != s_keyListeners.end(); ++itr)
	{
		(*itr)->keyPressEvent(key);
	}
}

void XInput::keyReleased(const XVirtualKey key)
{
	for(vector<xd::KeyboardListener*>::iterator itr = s_keyListeners.begin(); itr != s_keyListeners.end(); ++itr)
	{
		(*itr)->keyReleaseEvent(key);
	}
}

void XInput::addMouseListener(xd::MouseListener *object)
{
	// Add keyboard listener
	if(object) {
		s_mouseListeners.push_back(object);
	}
}

void XInput::mouseScroll(const int dt)
{
	for(vector<xd::MouseListener*>::iterator itr = s_mouseListeners.begin(); itr != s_mouseListeners.end(); ++itr)
	{
		(*itr)->mouseWheelEvent(dt);
	}
}

void XInput::checkBindings()
{
	// Iterate key bindings
	for(map<XVirtualKey, KeyBind>::iterator itr = s_keyBindings.begin(); itr != s_keyBindings.end(); ++itr)
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

Vector2i XInput::getCursorPos()
{
	POINT p;
	GetCursorPos(&p);
	return Vector2i(p.x, p.y);
}

void XInput::setCursorPos(const Vector2i &pos)
{
	SetCursorPos(pos.x, pos.y);
}

void XInput::setCursorLimits(const Recti &area)
{
	RECT rect;
	rect.top    = (long)(area.getY());
	rect.bottom	= (long)(area.getY()+area.getHeight());
	rect.left   = (long)(area.getX());
	rect.right  = (long)(area.getX()+area.getHeight());
	ClipCursor(&rect);
}

Vector2 XInput::getPosition()
{
	return s_position;
}

bool XInput::getKeyState(const XVirtualKey key)
{
	return /*Window::hasFocus() &&*/ (GetKeyState(toWinKey(key)) & 0x80) != 0;
}