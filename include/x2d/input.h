#ifndef X2D_INPUT_H
#define X2D_INPUT_H

#include <x2d/config.h>
#include <x2d/console.h>
#include <x2d/base.h>
#include <x2d/math.h>

enum xdMouseButton
{
	X2D_LeftMouseButton,
	X2D_MiddleMouseButton,
	X2D_RightMouseButton
};

// Virtual keys
enum xdVirtualKey
{
	// Standard ASCII-Mapped keys
	XD_KEY_NULL = 0x00,
	XD_KEY_BACKSPACE = 0x08,
	XD_KEY_TAB = 0x09,
	XD_KEY_ENTER = 0x0D,
	XD_KEY_ESCAPE = 0x1B,
	XD_KEY_SPACE = 0x20,

	XD_KEY_0 = 0x30,
	XD_KEY_1,
	XD_KEY_2,
	XD_KEY_3,
	XD_KEY_4,
	XD_KEY_5,
	XD_KEY_6,
	XD_KEY_7,
	XD_KEY_8,
	XD_KEY_9,

	// Alphabetic keys
	XD_KEY_A = 0x41,
	XD_KEY_B,
	XD_KEY_C,
	XD_KEY_D,
	XD_KEY_E,
	XD_KEY_F,
	XD_KEY_G,
	XD_KEY_H,
	XD_KEY_I,
	XD_KEY_J,
	XD_KEY_K,
	XD_KEY_L,
	XD_KEY_M,
	XD_KEY_N,
	XD_KEY_O,
	XD_KEY_P,
	XD_KEY_Q,
	XD_KEY_R,
	XD_KEY_S,
	XD_KEY_T,
	XD_KEY_U,
	XD_KEY_V,
	XD_KEY_W,
	XD_KEY_X,
	XD_KEY_Y,
	XD_KEY_Z,

	// Mouse keys
	X2D_LeftMouse = 0x80,
	X2D_RightMouse,
	X2D_MiddleMouse,

	// Arrow keys
	XD_KEY_Left,
	XD_KEY_Up,
	XD_KEY_Right,
	XD_KEY_Down,

	// Special keys
	XD_KEY_Shift,
	XD_KEY_LeftShift,
	XD_KEY_RightShift,
	XD_KEY_Control,
	XD_KEY_LeftControl,
	XD_KEY_RightControl,
	XD_KEY_Menu, // Windows button
	XD_KEY_LeftMenu,
	XD_KEY_RightMenu,
	XD_KEY_Space,
	XD_KEY_PageUp,
	XD_KEY_PageDown,
	XD_KEY_End,
	XD_KEY_Home,
	XD_KEY_Snapshot,
	XD_KEY_Insert,
	XD_KEY_Delete,

	// Numpad keys
	XD_KEY_Numpad0,
	XD_KEY_Numpad1,
	XD_KEY_Numpad2,
	XD_KEY_Numpad3,
	XD_KEY_Numpad4,
	XD_KEY_Numpad5,
	XD_KEY_Numpad6,
	XD_KEY_Numpad7,
	XD_KEY_Numpad8,
	XD_KEY_Numpad9,
	XD_KEY_Plus,
	XD_KEY_Hyphen,

	// Other keys
	XD_KEY_Caret,
	XD_KEY_Apostrophe,
	XD_KEY_QuotationMark,
	XD_KEY_Backslash,
	XD_KEY_Slash,
	XD_KEY_Asterisk,
	XD_KEY_Pipe,
	XD_KEY_Colon,
	XD_KEY_Semicolon,
	XD_KEY_Tilde,
	XD_KEY_Comma,
	XD_KEY_Period,
	XD_KEY_Greater,
	XD_KEY_Less,
	XD_KEY_Equals
};

class XDAPI xdInput
{
public:
	AS_DECL_SINGLETON

	virtual ~xdInput();

	// Desktop cursor functions
	virtual void     setCursorPos(const Vector2i &pos)			{ NOT_IMPLEMENTED(setCursorPos) }
	virtual Vector2i getCursorPos() const						{ NOT_IMPLEMENTED_RET(getCursorPos, Vector2i(0)) }
	virtual void     setCursorLimits(const Recti &area)			{ NOT_IMPLEMENTED(setCursorLimits) }

	// Key state function
	virtual bool getKeyState(const xdVirtualKey key) const		{ NOT_IMPLEMENTED_RET(getKeyState, false) }

	// General position
	virtual Vector2 getPosition() const 						{ NOT_IMPLEMENTED_RET(getPosition, Vector2(0.0f)) }

	// Key binding
	void bind(const xdVirtualKey key, asIScriptFunction *func);
	void checkBindings();

	// Keyboard listener
	void addKeyboardListener(asIScriptObject *object);
	void charEvent(uint utf8char);
	void keyPressed(xdVirtualKey key);
	void keyReleased(xdVirtualKey key);
	
	// Overloads
	void setCursorPos(const int x, const int y) { setCursorPos(Vector2i(x, y)); }
	void setCursorLimits(const int x, const int y, const int w, const int h) { setCursorLimits(Recti(x, y, w, h)); }

private:
	struct KeyBind
	{
		bool pressed;
		asIScriptFunction *function;
	};

	map<xdVirtualKey, KeyBind> m_keyBindings;
	vector<asIScriptObject*> m_keyListeners;
};

#endif // X2D_INPUT_H