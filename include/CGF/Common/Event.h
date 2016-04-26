#ifndef CGF_EVENT_H
#define CGF_EVENT_H

#include <CGF/Config.h>

BEGIN_CGF_NAMESPACE

enum EventType
{
	EVENT_START,
	EVENT_END,

	EVENT_TICK,
	EVENT_DRAW,
	EVENT_STEP_BEGIN,
	EVENT_STEP_END,
	EVENT_WINDOW_SIZE_CHANGED,

	EVENT_KEY_DOWN,
	EVENT_KEY_UP,
	EVENT_KEY_REPEAT,

	EVENT_TEXT_INPUT,

	EVENT_MOUSE_UP,
	EVENT_MOUSE_DOWN,
	EVENT_MOUSE_MOVE,
	EVENT_MOUSE_WHEEL,

	EVENT_CUSTOM
};

class CGF_API Event
{
protected:
	Event(const uint type) :
		m_type(type),
		m_userData(0)
	{
	}

public:
	uint getType() const
	{
		return m_type;
	}

	void *getUserData() const
	{
		return m_userData;
	}

	void setUserData(void *data)
	{
		m_userData = data;
	}

private:
	const uint m_type;
	void *m_userData;
};

class CGF_API KeyEvent : public Event
{
public:
	enum Modifier
	{
		NONE = KMOD_NONE,
		NUMLOCK = KMOD_NUM,
		CAPSLOCK = KMOD_CAPS,
		LCONTROL = KMOD_LCTRL,
		RCONTROL = KMOD_RCTRL,
		RSHIFT = KMOD_RSHIFT,
		LSHIFT = KMOD_LSHIFT,
		RALT = KMOD_RALT,
		LALT = KMOD_LALT,
		CTRL = KMOD_CTRL,
		SHIFT = KMOD_SHIFT,
		ALT = KMOD_ALT
	};

	enum KeyEventType
	{
		DOWN = EVENT_KEY_DOWN,
		UP = EVENT_KEY_UP,
		REPEAT = EVENT_KEY_REPEAT
	};

	KeyEvent(const KeyEventType type, InputManager *inputManager, const InputButton inputButton, const Uint16 modifiers) :
		Event(type),
		m_inputManager(inputManager),
		m_inputButton(inputButton),
		m_modifiers(modifiers)
	{
	}

	InputManager *getInputManager() const
	{
		return m_inputManager;
	}

	InputButton getInputButton() const
	{
		return m_inputButton;
	}

	Keycode getKeycode() const;

	Scancode getScancode() const
	{
		return (Scancode) m_inputButton.getCode();
	}

	Uint16 getModifiers() const
	{
		return m_modifiers;
	}

private:
	InputManager * const m_inputManager;
	const InputButton m_inputButton;
	const Uint16 m_modifiers;
};

class CGF_API TextEvent : public Event
{
public:
	TextEvent(const char c) :
		Event(EVENT_TEXT_INPUT),
		m_char(c)
	{
	}

	char getChar() const
	{
		return m_char;
	}

private:
	const char m_char;
};

class CGF_API MouseEvent : public Event
{
public:
	enum MouseEventType
	{
		MOVE = EVENT_MOUSE_MOVE,
		DOWN = EVENT_MOUSE_DOWN,
		UP = EVENT_MOUSE_UP,
		WHEEL = EVENT_MOUSE_WHEEL
	};

	MouseEvent(const MouseEventType type, const Sint32 x, const Sint32 y, const MouseButton button, const Sint32 wheelX, const Sint32 wheelY) :
		Event(type),
		m_x(x),
		m_y(y),
		m_button(button),
		m_wheelX(x),
		m_wheelY(y)
	{
	}

	MouseButton getButton() const
	{
		return m_button;
	}

	Sint32 getX() const
	{
		return m_x;
	}
	
	Sint32 getY() const
	{
		return m_y;
	}

	Vector2<Sint32> getPosition() const
	{
		return Vector2<Sint32>(m_x, m_y);
	}

	Sint32 getWheelX() const
	{
		return m_wheelX;
	}

	Sint32 getWheelY() const
	{
		return m_wheelY;
	}

private:
	const Sint32 m_x, m_y;
	const MouseButton m_button;
	const Sint32 m_wheelX, m_wheelY;
};

class GraphicsContext;
class SpriteBatch;

class CGF_API DrawEvent : public Event
{
public:
	DrawEvent(const float alpha, GraphicsContext *graphicsContext) :
		Event(EVENT_DRAW),
		m_alpha(alpha),
		m_graphicsContext(graphicsContext)
	{
	}

	float getAlpha() const
	{
		return m_alpha;
	}

	GraphicsContext *getGraphicsContext() const
	{
		return m_graphicsContext;
	}

private:
	const float m_alpha;
	GraphicsContext *m_graphicsContext;
};

class CGF_API TickEvent : public Event
{
public:
	TickEvent(const float delta) :
		Event(EVENT_TICK),
		m_delta(delta)
	{
	}

	float getDelta() const
	{
		return m_delta;
	}

private:
	const float m_delta;
};

class Window;

class CGF_API WindowEvent : public Event
{
public:
	enum WindowEventType
	{
		SIZE_CHANGED = EVENT_WINDOW_SIZE_CHANGED
	};

	WindowEvent(const WindowEventType type, Window *window, const Sint32 width, const Sint32 height) :
		Event(type),
		m_window(window),
		m_width(width),
		m_height(height)
	{
	}

	Window *getWindow() const
	{
		return m_window;
	}

	Sint32 getWidth() const
	{
		return m_width;
	}

	Sint32 getHeight() const
	{
		return m_height;
	}

private:
	Window *m_window;
	Sint32 m_width, m_height;
};

class CGF_API GameEvent : public Event
{
public:
	enum GameEventType
	{
		START = EVENT_START,
		END = EVENT_END
	};

	GameEvent(const GameEventType type) :
		Event(type)
	{
	}
};

class CGF_API StepEvent : public Event
{
public:
	enum StepEventType
	{
		BEGIN = EVENT_STEP_BEGIN,
		END = EVENT_STEP_END
	};

	StepEvent(const StepEventType type) :
		Event(type)
	{
	}
};

END_CGF_NAMESPACE

#endif // CGF_EVENT_H