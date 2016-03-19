#ifndef CGF_INPUT_MANAGER_H
#define CGF_INPUT_MANAGER_H

#include <CGF/Config.h>

#include <CGF/Input/Keycodes.h>
#include <CGF/Input/Scancodes.h>

BEGIN_CGF_NAMESPACE

class CGF_API InputButton
{
public:
	enum Type
	{
		NONE,
		KEYBOARD,
		MOUSE,
		GAMEPAD
	};

	InputButton() :
		code(0),
		type(NONE)
	{
	}

	InputButton(const Keycode keycode) :
		code(SDL_GetScancodeFromKey((SDL_Keycode) keycode)),
		type(KEYBOARD)
	{
	}

	InputButton(const Scancode scancode) :
		code(scancode),
		type(KEYBOARD)
	{
	}

	InputButton(const MouseButton mouseButton) :
		code(mouseButton),
		type(MOUSE)
	{
	}

	InputButton &operator=(const Scancode scancode)
	{
		code = scancode;
		type = KEYBOARD;
		return *this;
	}

	InputButton &operator=(const Keycode keycode)
	{
		code = SDL_GetScancodeFromKey((SDL_Keycode) keycode);
		type = KEYBOARD;
		return *this;
	}

	InputButton &operator=(const MouseButton mouseButton)
	{
		code = mouseButton;
		type = MOUSE;
		return *this;
	}

	bool operator==(const InputButton inputButton)
	{
		return type == inputButton.type && code == inputButton.code;
	}

	bool operator==(const Scancode scancode)
	{
		return type == KEYBOARD && code == scancode;
	}

	bool operator==(const Keycode keycode)
	{
		return type == KEYBOARD && code == SDL_GetScancodeFromKey((SDL_Keycode) keycode);
	}

	bool operator==(const MouseButton mouseButton)
	{
		return type == MOUSE && code == mouseButton;
	}

	Type getType() const
	{
		return type;
	}

	uint getCode() const
	{
		return code;
	}

private:
	uint code;
	Type type;
};

class KeyEvent;
class InputContext;

/**
* \class	Keybind
*
* \brief
*/

class CGF_API Keybind
{
public:
	Keybind();
	Keybind(InputButton keycode, function<void(KeyEvent*)> func = function<void(KeyEvent*)>());

	function<void(KeyEvent*)> getFunction() const
	{
		return m_function;
	}

	void setFunction(function<void(KeyEvent*)> func)
	{
		m_function = func;
	}

	InputButton getInputButton() const
	{
		return m_inputButton;
	}

	void setInputButton(const InputButton inputButton)
	{
		m_inputButton = inputButton;
	}

private:
	InputButton m_inputButton;
	function<void(KeyEvent*)> m_function;
};

/**
* \class	InputManager
*
* \brief
*/

#include <CGF/Math/Vector.h>

class CGF_API InputManager
{
	friend class Game;
public:
	InputManager(string contextFile);
	~InputManager();

	// Desktop cursor functions
	//void setCursorLimits(const int x, const int y, const int w, const int h);

	// Key state function
	bool getKeyState(const InputButton inputButton) const;

	// Window-relative position
	void getPosition(Sint32 *x, Sint32 *y) const;
	Vector2I getPosition() const;
	Sint32 getX() const;
	Sint32 getY() const;

	// Input context
	void setContext(InputContext *inputContext);
	InputContext *getContext() const
	{
		return m_context;
	}
	InputContext *getContextByName(const string &name);

	// Clipboard
	string getClipboardString();
	void setClipboardString(const string);

	// Virtual key to string
	InputButton toInputButton(string name) const;

	/**
	* \fn	KeybindPtr InputManager::addKeybind(KeybindPtr keybind);
	*
	* \brief	Adds a keybind.
	*
	* \param	keybind	The keybind.
	*
	* \return	A KeybindPtr.
	*/

	void addKeybind(Keybind *keybind);

	/**
	* \fn	void InputManager::removeKeybind(KeybindPtr keybind);
	*
	* \brief	Removes the keybind described by keybind.
	*
	* \param	keybind	The keybind.
	*/

	void removeKeybind(Keybind *keybind);

private:
	// Update bindings
	void updateKeybinds(KeyEvent *e);

	// Cursor position
	Sint32 m_x, m_y;

	// Input context
	InputContext *m_context;
	map<string, InputContext*> m_contextMap;

	// String to key map
	map<string, InputButton> m_strToKey;

	// Key binds
	list<Keybind*> m_keybinds;
	list<Keybind*> m_contextKeybinds;
};


END_CGF_NAMESPACE

#endif // CGF_INPUT_MANAGER_H