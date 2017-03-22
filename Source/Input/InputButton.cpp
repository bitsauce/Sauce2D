#include <Sauce/Input/InputButton.h>

BEGIN_SAUCE_NAMESPACE

InputButton::InputButton() :
	code(0),
	type(NONE)
{
}

InputButton::InputButton(const Keycode keycode) :
	code(SDL_GetScancodeFromKey((SDL_Keycode) keycode)),
	type(KEYBOARD)
{
}

InputButton::InputButton(const Scancode scancode) :
	code(scancode),
	type(KEYBOARD)
{
}

InputButton::InputButton(const MouseButton mouseButton) :
	code(mouseButton),
	type(MOUSE)
{
}

InputButton &InputButton::operator=(const Scancode scancode)
{
	code = scancode;
	type = KEYBOARD;
	return *this;
}

InputButton &InputButton::operator=(const Keycode keycode)
{
	code = SDL_GetScancodeFromKey((SDL_Keycode) keycode);
	type = KEYBOARD;
	return *this;
}

InputButton &InputButton::operator=(const MouseButton mouseButton)
{
	code = mouseButton;
	type = MOUSE;
	return *this;
}

bool InputButton::operator==(const InputButton inputButton)
{
	return type == inputButton.type && code == inputButton.code;
}

bool InputButton::operator==(const Scancode scancode)
{
	return type == KEYBOARD && code == scancode;
}

bool InputButton::operator==(const Keycode keycode)
{
	return type == KEYBOARD && code == SDL_GetScancodeFromKey((SDL_Keycode) keycode);
}

bool InputButton::operator==(const MouseButton mouseButton)
{
	return type == MOUSE && code == mouseButton;
}

InputButton::Type InputButton::getType() const
{
	return type;
}

uint InputButton::getCode() const
{
	return code;
}

END_SAUCE_NAMESPACE