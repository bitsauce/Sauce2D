#ifndef SAUCE_INPUT_MANAGER_H
#define SAUCE_INPUT_MANAGER_H

#include <Sauce/Config.h>

#include <Sauce/Input/InputButton.h>

BEGIN_SAUCE_NAMESPACE

class InputEvent;
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
	Keybind(InputButton button, function<void(InputEvent*)> func = function<void(InputEvent*)>(), const uint flags = TRIGGER_WHEN_PRESSED);

	enum
	{
		TRIGGER_WHEN_PRESSED = 1 << 0,
		TRIGGER_WHEN_RELEASED = 1 << 1,
		TRIGGER_WHEN_REPEATING = 1 << 2,
		TRIGGER_WHILE_PRESSED = 1 << 3
	};

	function<void(InputEvent*)> getFunction() const
	{
		return m_function;
	}

	void setFunction(function<void(InputEvent*)> func)
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
	function<void(InputEvent*)> m_function;
};

typedef void ControllerDevice;

/**
* \class	InputManager
*
* \brief
*/

#include <Sauce/Math/Vector.h>

#define AXIS_VALUE_TO_FLOAT(x) (float(x) / SHRT_MAX)

class SAUCE_API InputManager
{
	friend class Game;
public:
	InputManager(string contextFile);
	~InputManager();

	// Desktop cursor functions
	//void setCursorLimits(const int x, const int y, const int w, const int h);

	// Key state function
	bool getKeyState(const InputButton inputButton, ControllerDevice *controller = 0) const;

	// Buttons and axis
	bool getButtonState(const InputButton inputButton, ControllerDevice *controller = 0) const;
	float getAxisValue(const ControllerAxis axis, ControllerDevice *controller = 0) const;

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

	void addController(const uint id);
	void removeController(const uint id);
	void setAxisThreshold(const float threshold) { m_triggerThreshold = threshold; }

private:
	// Update bindings
	void updateKeybinds(InputEvent *e);

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

	// Default controller
	ControllerDevice *m_defaultController;

	// Controllers
	map<uint, ControllerDevice*> m_controllers;

	// Controller trigger buttons
	bool m_leftTrigger, m_rightTrigger;
	float m_triggerThreshold;
};


END_SAUCE_NAMESPACE

#endif // SAUCE_INPUT_MANAGER_H
