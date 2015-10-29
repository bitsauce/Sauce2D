#ifndef X2D_KEY_LISTENER_H
#define X2D_KEY_LISTENER_H

#include <x2d/config.h>

BEGIN_XD_NAMESPACE

class XDAPI KeyEvent
{
	friend class Window;
public:
	enum Modifier
	{
		SHIFT = GLFW_MOD_SHIFT,
		CONTROL = GLFW_MOD_CONTROL,
		ALT = GLFW_MOD_ALT,
		SUPER = GLFW_MOD_SUPER
	};

	enum Action
	{
		PRESS = GLFW_PRESS,
		RELEASE = GLFW_RELEASE,
		REPEAT = GLFW_REPEAT
	};

	VirtualKey getKey() const
	{
		return m_key;
	}

	bool isKey(const VirtualKey key) const
	{
		return m_key == key;
	}

	Action getAction() const
	{
		return m_action;
	}

	bool isAction(const Action action) const
	{
		return m_action == action;
	}

	bool isModifier(Modifier modifier) const
	{
		return (m_modifiers & modifier) != 0;
	}

	int getScanCode() const
	{
		return m_scanCode;
	}

private:
	KeyEvent(const VirtualKey key, const Action action, const int modifiers, const int scanCode) :
		m_key(key),
		m_action(action),
		m_modifiers(modifiers),
		m_scanCode(scanCode)
	{
	}

	const VirtualKey m_key;
	const Action m_action;
	const int m_modifiers;
	const int m_scanCode;
};

class XDAPI KeyListener
{
	friend class Window;
public:
	KeyListener();
	virtual ~KeyListener();

	virtual void keyEvent(const KeyEvent & event) { }
	virtual void charEvent(const uint c) { }

private:

	static void callCharEvent(const uint c);
	static void callKeyEvent(const KeyEvent & event);

	static list<KeyListener*> s_keyListeners;
};

END_XD_NAMESPACE

#endif // X2D_KEY_LISTENER_H