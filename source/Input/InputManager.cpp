#include <CGF/Input.h>

BEGIN_CGF_NAMESPACE

Keybind::Keybind() :
	m_scancode(CGF_SCANCODE_UNKNOWN),
	m_function()
{
}

Keybind::Keybind(Keycode keycode, function<void(KeyEvent*)> func) :
	m_function(func)
{
	setKeycode(keycode);
}

Keybind::Keybind(Scancode scancode, function<void(KeyEvent*)> func) :
	m_scancode(scancode),
	m_function(func)
{
}

Keycode Keybind::getKeycode() const
{
	return (Keycode) SDL_GetKeyFromScancode((SDL_Scancode) m_scancode);
}

void Keybind::setKeycode(const Keycode keycode)
{
	m_scancode = (Scancode) SDL_GetScancodeFromKey((SDL_Keycode) keycode);
}

END_CGF_NAMESPACE
