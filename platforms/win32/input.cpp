
/*void Window::cursorPos(int &x, int &y) const
{
	POINT p;
	GetCursorPos(&p);
	x = p.x;
	y = p.y;
}

void Window::setCursorPos(const int x, const int y)
{
	SetCursorPos(x, y);
}

void Window::setCursorLimits(const int x, const int y, const int w, const int h)
{
	RECT rect;
	rect.top    = (long)(y);
	rect.bottom	= (long)(y+h);
	rect.left   = (long)(x);
	rect.right  = (long)(x+w);
	ClipCursor(&rect);
}

bool Window::getKeyState(const xdVirtualKey key)
{
	int vk = 0;
	switch(key)
	{
	case X2D_LeftMouse:	vk = VK_LBUTTON; break;
	case X2D_RightMouse: vk = VK_RBUTTON; break;
	case X2D_MiddleMouse: vk = VK_MBUTTON; break;
	case X2D_Key0: vk = '0'; break;
	case X2D_Key1: vk = '1'; break;
	case X2D_Key2: vk = '2'; break;
	case X2D_Key3: vk = '3'; break;
	case X2D_Key4: vk = '4'; break;
	case X2D_Key5: vk = '5'; break;
	case X2D_Key6: vk = '6'; break;
	case X2D_Key7: vk = '7'; break;
	case X2D_Key8: vk = '8'; break;
	case X2D_Key9: vk = '9'; break;
	case X2D_KeyA: vk = 'A'; break;
	case X2D_KeyB: vk = 'B'; break;
	case X2D_KeyC: vk = 'C'; break;
	case X2D_KeyD: vk = 'D'; break;
	case X2D_KeyE: vk = 'E'; break;
	case X2D_KeyF: vk = 'F'; break;
	case X2D_KeyG: vk = 'G'; break;
	case X2D_KeyH: vk = 'H'; break;
	case X2D_KeyI: vk = 'I'; break;
	case X2D_KeyJ: vk = 'J'; break;
	case X2D_KeyK: vk = 'K'; break;
	case X2D_KeyL: vk = 'L'; break;
	case X2D_KeyM: vk = 'M'; break;
	case X2D_KeyN: vk = 'N'; break;
	case X2D_KeyO: vk = 'O'; break;
	case X2D_KeyP: vk = 'P'; break;
	case X2D_KeyQ: vk = 'Q'; break;
	case X2D_KeyR: vk = 'R'; break;
	case X2D_KeyS: vk = 'S'; break;
	case X2D_KeyT: vk = 'T'; break;
	case X2D_KeyU: vk = 'U'; break;
	case X2D_KeyV: vk = 'V'; break;
	case X2D_KeyW: vk = 'W'; break;
	case X2D_KeyX: vk = 'X'; break;
	case X2D_KeyY: vk = 'Y'; break;
	case X2D_KeyZ: vk = 'X'; break;
	case X2D_KeyRight: vk = VK_RIGHT; break;
	case X2D_KeyDown: vk = VK_DOWN; break;
	case X2D_KeyLeft: vk = VK_LEFT; break;
	case X2D_KeyUp: vk = VK_UP; break;
	case X2D_KeyBackspace: vk = VK_BACK; break;
	case X2D_KeyEscape: vk = VK_ESCAPE; break;
	case X2D_KeyTab: vk = VK_TAB; break;
	case X2D_KeyEnter: vk = VK_RETURN; break;
	case X2D_KeyShift: vk = VK_SHIFT; break;
	case X2D_KeyLeftShift: vk = VK_LSHIFT; break;
	case X2D_KeyRightShift: vk = VK_RSHIFT; break;
	case X2D_KeyControl: vk = VK_CONTROL; break;
	case X2D_KeyLeftControl: vk = VK_LCONTROL; break;
	case X2D_KeyRightControl: vk = VK_RCONTROL; break;
	case X2D_KeyMenu: vk = VK_MENU; break;
	case X2D_KeyLeftMenu: vk = VK_LMENU; break;
	case X2D_KeyRightMenu: vk = VK_RMENU; break;
	case X2D_KeySpace: vk = VK_SPACE; break;
	case X2D_KeyPageUp: vk = VK_NEXT; break;
	case X2D_KeyPageDown: vk = VK_PRIOR; break;
	case X2D_KeyEnd: vk = VK_END; break;
	case X2D_KeyHome: vk = VK_HOME; break;
	case X2D_KeySnapshot: vk = VK_SNAPSHOT; break;
	case X2D_KeyInsert: vk = VK_INSERT; break;
	case X2D_KeyDelete: vk = VK_DELETE; break;
	case X2D_KeyNumpad0: vk = VK_NUMPAD0; break;
	case X2D_KeyNumpad1: vk = VK_NUMPAD1; break;
	case X2D_KeyNumpad2: vk = VK_NUMPAD2; break;
	case X2D_KeyNumpad3: vk = VK_NUMPAD3; break;
	case X2D_KeyNumpad4: vk = VK_NUMPAD4; break;
	case X2D_KeyNumpad5: vk = VK_NUMPAD5; break;
	case X2D_KeyNumpad6: vk = VK_NUMPAD6; break;
	case X2D_KeyNumpad7: vk = VK_NUMPAD7; break;
	case X2D_KeyNumpad8: vk = VK_NUMPAD8; break;
	case X2D_KeyNumpad9: vk = VK_NUMPAD9; break;
	/*case X2D_KeyPluss: vk = ; break;
	case X2D_KeyHyphen: vk = ; break;
	case X2D_KeyCaret: vk = ; break;
	case X2D_KeyApostrophe: vk = ; break;
	case X2D_KeyQuotationMark: vk = ; break;
	case X2D_KeyBackslash: vk = ; break;
	case X2D_KeySlash: vk = ; break;
	case X2D_KeyAsterisk: vk = ; break;
	case X2D_KeyPipe: vk = ; break;
	case X2D_KeyColon: vk = ; break;
	case X2D_KeySemicolon: vk = ; break;
	case X2D_KeyTilde: vk = ; break;
	case X2D_KeyComma: vk = ; break;
	case X2D_KeyPeriod: vk = ; break;
	case X2D_KeyGreater: vk = ; break;
	case X2D_KeyLess: vk = ; break;
	case X2D_KeyEquals: vk = VK_; break;*/
	/*}
	return Window::hasFocus() && (GetKeyState(vk) & 0x80) != 0;
}*/