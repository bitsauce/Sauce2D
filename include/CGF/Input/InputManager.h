#ifndef CGF_INPUT_MANAGER_H
#define CGF_INPUT_MANAGER_H

#include <CGF/Config.h>

#include <CGF/Input/Keycodes.h>
#include <CGF/Input/Scancodes.h>

BEGIN_CGF_NAMESPACE

class KeyEvent;
class InputContext;

/**
* \class	Keyname
*
* \brief	A keyname.
*/
class Keyname
{
public:
	Keyname() :
		m_name(""),
		m_keycode(CGF_KEY_UNKNOWN)
	{
	}

	Keyname(const string name, const Keycode keycode) :
		m_name(name),
		m_keycode(keycode)
	{
	}

	string getName() const
	{
		return m_name;
	}

	void setName(const string name)
	{
		m_name = name;
	}

	Keycode getKeycode() const
	{
		return m_keycode;
	}

	void setKeycode(const Keycode keycode)
	{
		m_keycode = keycode;
	}

	bool isValid() const
	{
		return !m_name.empty() && m_keycode != CGF_KEY_UNKNOWN;
	}

private:
	string m_name;
	Keycode m_keycode;
};

/**
* \class	Keybind
*
* \brief
*/

class Keybind;
typedef shared_ptr<Keybind> KeybindPtr;

class CGF_API Keybind
{
public:
	Keybind();
	Keybind(Keyname keyname, function<void(KeyEvent*)> func = function<void(KeyEvent*)>());
	Keybind(Keycode keycode, function<void(KeyEvent*)> func = function<void(KeyEvent*)>());

	Keyname &getKeyname()
	{
		return m_keyname;
	}

	void setKeyname(Keyname keyname)
	{
		m_keyname = keyname;
	}

	function<void(KeyEvent*)> getFunction() const
	{
		return m_function;
	}

	void setFunction(function<void(KeyEvent*)> func)
	{
		m_function = func;
	}

private:
	Keyname m_keyname;
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

	// Desktop cursor functions
	//void setCursorLimits(const int x, const int y, const int w, const int h);

	// Key state function
	bool getKeyState(const Keycode keycode) const;
	bool getKeyState(const Scancode scancode) const;

	// Window-relative position
	void getPosition(Sint32 *x, Sint32 *y) const;
	Vector2i getPosition() const;
	Sint32 getX() const;
	Sint32 getY() const;

	// Input context
	void setContext(InputContext *inputContext);
	InputContext *getContext();

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

#endif // CGF_INPUT_MANAGER_H