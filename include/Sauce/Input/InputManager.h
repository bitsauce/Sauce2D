#ifndef SAUCE_INPUT_MANAGER_H
#define SAUCE_INPUT_MANAGER_H

#include <Sauce/Config.h>

#include <Sauce/Input/InputButton.h>

BEGIN_SAUCE_NAMESPACE

class KeyEvent;
class InputContext;

/**
* \class	Keybind
*
* \brief
*/

class SAUCE_API Keybind
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

#include <Sauce/Math/Vector.h>

class SAUCE_API InputManager
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


END_SAUCE_NAMESPACE

#endif // SAUCE_INPUT_MANAGER_H
