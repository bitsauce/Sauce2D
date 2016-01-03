#include <CGF/Input.h>

BEGIN_CGF_NAMESPACE

Keybind::Keybind() :
	m_keyname(),
	m_function()
{
}

Keybind::Keybind(Keyname keyname, function<void(KeyEvent*)> func) :
	m_keyname(keyname),
	m_function(func)
{
}

Keybind::Keybind(Keycode keycode, function<void(KeyEvent*)> func) :
	m_keyname("", keycode),
	m_function(func)
{
}

END_CGF_NAMESPACE
