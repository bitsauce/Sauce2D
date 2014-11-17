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

bool Input::getKeyState(const XVirtualKey key) const
{
	return /*Window::hasFocus() &&*/ (GetKeyState(toWinKey(key)) & 0x80) != 0;
}