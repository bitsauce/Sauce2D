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

InputButton::InputButton(const ControllerButton controllerButton) :
	code(controllerButton),
	type(CONTROLLER_BUTTON)
{
}

InputButton::InputButton(const ControllerAxis axis) :
	code(axis),
	type(CONTROLLER_AXIS)
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

InputButton &InputButton::operator=(const ControllerButton controllerButton)
{
	code = controllerButton;
	type = CONTROLLER_BUTTON;
	return *this;
}

InputButton &InputButton::operator=(const ControllerAxis axis)
{
	code = axis;
	type = CONTROLLER_AXIS;
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

bool InputButton::operator==(const ControllerButton controllerButton)
{
	return type == CONTROLLER_BUTTON && code == controllerButton;
}

bool InputButton::operator==(const ControllerAxis axis)
{
	return type == CONTROLLER_AXIS && code == axis;
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