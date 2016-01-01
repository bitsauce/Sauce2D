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

	EVENT_MOUSE_UP,
	EVENT_MOUSE_DOWN,
	EVENT_MOUSE_MOVE,
	EVENT_MOUSE_WHEEL
};

class CGF_API Event
{
protected:
	Event(const EventType type) :
		m_type(type)
	{

	}

public:
	EventType getType() const
	{
		return m_type;
	}

private:
	const EventType m_type;
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

	KeyEvent(const KeyEventType type, const Keycode keycode, const Scancode scanCode, const Uint16 modifiers) :
		Event((EventType) type),
		m_keycode(keycode),
		m_modifiers(modifiers),
		m_scancode(scanCode)
	{
	}

	Keycode getKeycode() const
	{
		return m_keycode;
	}

	Scancode getScancode() const
	{
		return m_scancode;
	}

	Uint16 getModifiers() const
	{
		return m_modifiers;
	}

private:
	const Keycode m_keycode;
	const Scancode m_scancode;
	const Uint16 m_modifiers;
};

enum MouseButton
{
	MOUSE_BUTTON_NONE = 0,
	MOUSE_BUTTON_LEFT = SDL_BUTTON_LEFT,
	MOUSE_BUTTON_MIDDLE = SDL_BUTTON_MIDDLE,
	MOUSE_BUTTON_RIGHT = SDL_BUTTON_RIGHT,
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
		Event((EventType) type),
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

class CGF_API WindowEvent : public Event
{
public:
	enum WindowEventType
	{
		SIZE_CHANGED = EVENT_WINDOW_SIZE_CHANGED
	};

	WindowEvent(const WindowEventType type, const Sint32 width, const Sint32 height) :
		Event((EventType) type),
		m_width(width),
		m_height(height)
	{
	}

private:
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
		Event((EventType) type)
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
		Event((EventType) type)
	{
	}
};

END_CGF_NAMESPACE

#endif // CGF_EVENT_H