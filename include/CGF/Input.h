#ifndef CGF_INPUT_H
#define CGF_INPUT_H

#include <CGF/Config.h>
#include <CGF/Math.h>

#include <CGF/Input/InputContext.h>
#include <CGF/Input/KeyListener.h>
#include <CGF/Input/MouseListener.h>
#include <CGF/Input/Keycodes.h>
#include <CGF/Input/Scancodes.h>

BEGIN_CGF_NAMESPACE

class KeyEvent;

/**
 * \class	Keybind
 *
 * \brief	
 */

class Keybind;
typedef shared_ptr<Keybind> KeybindPtr;

class CGF_API Keybind
{
	friend class InputManager;
public:
	Keybind(Keycode key, function<void(KeyEvent*)> func);

private:
	const Keycode m_keycode;
	const Scancode m_scancode;
	const function<void(KeyEvent*)> m_function;
};

/**
 * \class	InputManager
 *
 * \brief	
 */

class CGF_API InputManager
{
	friend class Game;
public:
	InputManager(string contextFile);

	// Desktop cursor functions
	//void setCursorLimits(const int x, const int y, const int w, const int h);

	// Key state function
	bool getKeyState(const Keycode keycode) const;
	bool getKeyState(const Scancode scancode) const;

	// Window-relative position
	void getPosition(int &x, int &y) const;
	Sint32 getX() const;
	Sint32 getY() const;

	// Input context
	void setContext(InputContext *inputContext);
	InputContext *getContext(const string &contextName);

	// Clipboard
	string getClipboardString();
	void setClipboardString(const string);

	// Virtual key to string
	Keycode strToKey(string name);

	/**
	 * \fn	KeybindPtr InputManager::addKeybind(KeybindPtr keybind);
	 *
	 * \brief	Adds a keybind.
	 *
	 * \param	keybind	The keybind.
	 *
	 * \return	A KeybindPtr.
	 */

	void addKeybind(KeybindPtr keybind);

	/**
	 * \fn	void InputManager::removeKeybind(KeybindPtr keybind);
	 *
	 * \brief	Removes the keybind described by keybind.
	 *
	 * \param	keybind	The keybind.
	 */

	void removeKeybind(KeybindPtr keybind);

private:
	// Update bindings
	void updateKeybinds(KeyEvent *e);

	// Initialize
	void init(string file);

	// Cursor position
	Sint32 m_x, m_y;

	// Input context
	InputContext *m_context;
	map<string, InputContext*> m_contextMap;

	// String to key map
	map<string, Keycode> m_strToKey;

	// Key binds
	list<KeybindPtr> m_keybinds;
};

END_CGF_NAMESPACE

#endif // CGF_INPUT_H
