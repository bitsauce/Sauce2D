#include <Sauce/Input.h>

BEGIN_SAUCE_NAMESPACE

Keybind::Keybind() :
	m_inputButton(),
	m_function()
{
}

Keybind::Keybind(InputButton inputButton, function<void(InputEvent*)> func) :
	m_inputButton(inputButton),
	m_function(func)
{
}

/*Keycode Keybind::getKeycode() const
{
	return (Keycode) SDL_GetKeyFromScancode((SDL_Scancode) m_scancode);
}

void Keybind::setKeycode(const Keycode keycode)
{
	m_scancode = (Scancode) SDL_GetScancodeFromKey((SDL_Keycode) keycode);
}*/

END_SAUCE_NAMESPACE
