#ifndef SAUCE_INPUT_CONTEXT_H
#define SAUCE_INPUT_CONTEXT_H

#include <Sauce/Config.h>
#include <Sauce/Input/InputManager.h>

BEGIN_SAUCE_NAMESPACE

/*********************************************************************
**	Input context													**
**********************************************************************/

class Game;

class SAUCE_API InputContext
{
	friend class InputManager;
public:
	InputContext(InputManager*);

	/**
	 * \fn	void InputContext::addKeybind(KeybindPtr keybind);
	 *
	 * \brief	Bind a function to an action for this input context.
	 *
	 * \param	keybind	The keybind.
	 *
	 * ### param	name	The name.
	 */

	void addKeybind(const string &name, Keybind* keybind);

	Keybind *getKeybind(const string &name)
	{
		map<string, Keybind*>::iterator itr;
		if((itr = m_keybinds.find(name)) != m_keybinds.end())
		{
			return itr->second;
		}
		return 0;
	}

	/**
	 * \fn	void InputContext::removeKeybind(KeybindPtr keybind);
	 *
	 * \brief	Removes the keybind described by keybind.
	 *
	 * \param	keybind	The keybind.
	 *
	 * ### param	name	The name.
	 */

	void removeKeybind(Keybind *keybind);
	void removeKeybind(const string &name);

	bool getKeyState(string name) const;

private:

	/**
	 * \fn	void InputContext::updateKeybinds(KeyEvent *e);
	 *
	 * \brief	Updates the keybinds with event e.
	 *
	 * \param [in,out]	e	If non-null, the KeyEvent to process.
	 */

	void updateKeybinds(InputEvent *e);

	// Key binds
	map<string, Keybind*> m_keybinds;

	// Input manager
	InputManager *m_inputManager;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_INPUT_CONTEXT_H
