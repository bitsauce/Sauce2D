#include <CGF/Input.h>

BEGIN_CGF_NAMESPACE

Keybind::Keybind() :
	m_keycode(CGF_KEY_UNKNOWN),
	m_function()
{
}

Keybind::Keybind(Keycode keycode, function<void(KeyEvent*)> func) :
	m_keycode(keycode),
	m_function(func)
{
}

END_CGF_NAMESPACE
