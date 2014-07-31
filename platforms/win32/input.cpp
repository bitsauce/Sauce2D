//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "input.h"

Input::Input() :
	m_position(0.0f)
{
}

Vector2i Input::getCursorPos() const
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

Vector2 Input::getPosition() const
{
	return m_position;
}

xdVirtualKey fromWinKey(uchar vk)
{
	switch(vk)
	{
	case VK_LBUTTON: return X2D_LeftMouse;
	case VK_RBUTTON: return X2D_RightMouse; 
	case VK_MBUTTON: return X2D_MiddleMouse; 
	case VK_RIGHT: return XD_KEY_Right; 
	case VK_DOWN: return XD_KEY_Down; 
	case VK_LEFT: return XD_KEY_Left; 
	case VK_UP: return XD_KEY_Up;
	case VK_SHIFT: return XD_KEY_Shift; 
	case VK_LSHIFT: return XD_KEY_LeftShift; 
	case VK_RSHIFT: return XD_KEY_RightShift; 
	case VK_CONTROL: return XD_KEY_Control; 
	case VK_LCONTROL: return XD_KEY_LeftControl; 
	case VK_RCONTROL: return XD_KEY_RightControl; 
	case VK_MENU: return XD_KEY_Menu; 
	case VK_LMENU: return XD_KEY_LeftMenu; 
	case VK_RMENU: return XD_KEY_RightMenu; 
	case VK_SPACE: return XD_KEY_Space; 
	case VK_NEXT: return XD_KEY_PageUp; 
	case VK_PRIOR: return XD_KEY_PageDown; 
	case VK_END: return XD_KEY_End; 
	case VK_HOME: return XD_KEY_Home; 
	case VK_SNAPSHOT: return XD_KEY_Snapshot; 
	case VK_INSERT: return XD_KEY_Insert; 
	case VK_DELETE: return XD_KEY_Delete; 
	case VK_NUMPAD0: return XD_KEY_Numpad0; 
	case VK_NUMPAD1: return XD_KEY_Numpad1; 
	case VK_NUMPAD2: return XD_KEY_Numpad2; 
	case VK_NUMPAD3: return XD_KEY_Numpad3; 
	case VK_NUMPAD4: return XD_KEY_Numpad4; 
	case VK_NUMPAD5: return XD_KEY_Numpad5; 
	case VK_NUMPAD6: return XD_KEY_Numpad6; 
	case VK_NUMPAD7: return XD_KEY_Numpad7; 
	case VK_NUMPAD8: return XD_KEY_Numpad8; 
	case VK_NUMPAD9: return XD_KEY_Numpad9; 
	case VK_OEM_PLUS: return XD_KEY_Plus; 
	//case XD_KEY_Hyphen: return VK_OEM_MINUS; break;
	//case XD_KEY_Caret: return VK_OEM_CAR; break;
	//case XD_KEY_Apostrophe: return VK_OEM_7; break;
	//case XD_KEY_QuotationMark: return VK_OEM_2; break;
	//case XD_KEY_Backslash: return VK_OEM_5; break;
	//case XD_KEY_Slash: return VK_OEM_2; break;
	//case XD_KEY_Asterisk: return VK_OEM_; break;
	//case XD_KEY_Pipe: return VK_OEM_5; break;
	//case XD_KEY_Colon: return VK_OEM_1; break;
	//case XD_KEY_Semicolon: return VK_OEM_1; break;
	//case XD_KEY_Tilde: return ; break;
	//case XD_KEY_Comma: return VK_OEM_COMMA; break;
	//case XD_KEY_Period: return VK_OEM_PERIOD; break;
	//case XD_KEY_Greater: return VK_OEM; break;
	//case XD_KEY_Less: return ; break;
	//case XD_KEY_Equals: return VK_; break;
	default: return xdVirtualKey(vk);
	}
	return XD_KEY_NULL;
}

uchar toWinKey(xdVirtualKey key)
{
	uchar vk = 0;
	switch(key)
	{
	case X2D_LeftMouse:	vk = VK_LBUTTON; break;
	case X2D_RightMouse: vk = VK_RBUTTON; break;
	case X2D_MiddleMouse: vk = VK_MBUTTON; break;
	case XD_KEY_Right: vk = VK_RIGHT; break;
	case XD_KEY_Down: vk = VK_DOWN; break;
	case XD_KEY_Left: vk = VK_LEFT; break;
	case XD_KEY_Up: vk = VK_UP; break;
	case XD_KEY_Shift: vk = VK_SHIFT; break;
	case XD_KEY_LeftShift: vk = VK_LSHIFT; break;
	case XD_KEY_RightShift: vk = VK_RSHIFT; break;
	case XD_KEY_Control: vk = VK_CONTROL; break;
	case XD_KEY_LeftControl: vk = VK_LCONTROL; break;
	case XD_KEY_RightControl: vk = VK_RCONTROL; break;
	case XD_KEY_Menu: vk = VK_MENU; break;
	case XD_KEY_LeftMenu: vk = VK_LMENU; break;
	case XD_KEY_RightMenu: vk = VK_RMENU; break;
	case XD_KEY_Space: vk = VK_SPACE; break;
	case XD_KEY_PageUp: vk = VK_NEXT; break;
	case XD_KEY_PageDown: vk = VK_PRIOR; break;
	case XD_KEY_End: vk = VK_END; break;
	case XD_KEY_Home: vk = VK_HOME; break;
	case XD_KEY_Snapshot: vk = VK_SNAPSHOT; break;
	case XD_KEY_Insert: vk = VK_INSERT; break;
	case XD_KEY_Delete: vk = VK_DELETE; break;
	case XD_KEY_Numpad0: vk = VK_NUMPAD0; break;
	case XD_KEY_Numpad1: vk = VK_NUMPAD1; break;
	case XD_KEY_Numpad2: vk = VK_NUMPAD2; break;
	case XD_KEY_Numpad3: vk = VK_NUMPAD3; break;
	case XD_KEY_Numpad4: vk = VK_NUMPAD4; break;
	case XD_KEY_Numpad5: vk = VK_NUMPAD5; break;
	case XD_KEY_Numpad6: vk = VK_NUMPAD6; break;
	case XD_KEY_Numpad7: vk = VK_NUMPAD7; break;
	case XD_KEY_Numpad8: vk = VK_NUMPAD8; break;
	case XD_KEY_Numpad9: vk = VK_NUMPAD9; break;
	case XD_KEY_Plus: vk = VK_OEM_PLUS; break;
	case XD_KEY_Hyphen: vk = VK_OEM_MINUS; break;
	//case XD_KEY_Caret: vk = VK_OEM_CAR; break;
	case XD_KEY_Apostrophe: vk = VK_OEM_7; break;
	case XD_KEY_QuotationMark: vk = VK_OEM_2; break;
	case XD_KEY_Backslash: vk = VK_OEM_5; break;
	case XD_KEY_Slash: vk = VK_OEM_2; break;
	//case XD_KEY_Asterisk: vk = VK_OEM_; break;
	case XD_KEY_Pipe: vk = VK_OEM_5; break;
	case XD_KEY_Colon: vk = VK_OEM_1; break;
	case XD_KEY_Semicolon: vk = VK_OEM_1; break;
	//case XD_KEY_Tilde: vk = ; break;
	case XD_KEY_Comma: vk = VK_OEM_COMMA; break;
	case XD_KEY_Period: vk = VK_OEM_PERIOD; break;
	//case XD_KEY_Greater: vk = VK_OEM; break;
	//case XD_KEY_Less: vk = ; break;
	//case XD_KEY_Equals: vk = VK_; break;
	default: return key;
	}
	return vk;
}

bool Input::getKeyState(const xdVirtualKey key) const
{
	return /*Window::hasFocus() &&*/ (GetKeyState(toWinKey(key)) & 0x80) != 0;
}