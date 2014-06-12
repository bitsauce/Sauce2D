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
	// Mouse keys
	X2D_LeftMouse,
	X2D_RightMouse,
	X2D_MiddleMouse,

	// Numeric keys
	X2D_Key0,
	X2D_Key1,
	X2D_Key2,
	X2D_Key3,
	X2D_Key4,
	X2D_Key5,
	X2D_Key6,
	X2D_Key7,
	X2D_Key8,
	X2D_Key9,

	// Alphabetic keys
	X2D_KeyA,
	X2D_KeyB,
	X2D_KeyC,
	X2D_KeyD,
	X2D_KeyE,
	X2D_KeyF,
	X2D_KeyG,
	X2D_KeyH,
	X2D_KeyI,
	X2D_KeyJ,
	X2D_KeyK,
	X2D_KeyL,
	X2D_KeyM,
	X2D_KeyN,
	X2D_KeyO,
	X2D_KeyP,
	X2D_KeyQ,
	X2D_KeyR,
	X2D_KeyS,
	X2D_KeyT,
	X2D_KeyU,
	X2D_KeyV,
	X2D_KeyW,
	X2D_KeyX,
	X2D_KeyY,
	X2D_KeyZ,

	// Arrow keys
	X2D_KeyLeft,
	X2D_KeyUp,
	X2D_KeyRight,
	X2D_KeyDown,

	// Special keys
	X2D_KeyBackspace,
	X2D_KeyEscape,
	X2D_KeyTab,
	X2D_KeyEnter,
	X2D_KeyShift,
	X2D_KeyLeftShift,
	X2D_KeyRightShift,
	X2D_KeyControl,
	X2D_KeyLeftControl,
	X2D_KeyRightControl,
	X2D_KeyMenu, // Windows button
	X2D_KeyLeftMenu,
	X2D_KeyRightMenu,
	X2D_KeySpace,
	X2D_KeyPageUp,
	X2D_KeyPageDown,
	X2D_KeyEnd,
	X2D_KeyHome,
	X2D_KeySnapshot,
	X2D_KeyInsert,
	X2D_KeyDelete,

	// Numpad keys
	X2D_KeyNumpad0,
	X2D_KeyNumpad1,
	X2D_KeyNumpad2,
	X2D_KeyNumpad3,
	X2D_KeyNumpad4,
	X2D_KeyNumpad5,
	X2D_KeyNumpad6,
	X2D_KeyNumpad7,
	X2D_KeyNumpad8,
	X2D_KeyNumpad9,
	X2D_KeyPluss,
	X2D_KeyHyphen,

	// Other keys
	X2D_KeyCaret,
	X2D_KeyApostrophe,
	X2D_KeyQuotationMark,
	X2D_KeyBackslash,
	X2D_KeySlash,
	X2D_KeyAsterisk,
	X2D_KeyPipe,
	X2D_KeyColon,
	X2D_KeySemicolon,
	X2D_KeyTilde,
	X2D_KeyComma,
	X2D_KeyPeriod,
	X2D_KeyGreater,
	X2D_KeyLess,
	X2D_KeyEquals
};

class XDAPI xdInput
{
public:
	AS_DECL_SINGLETON

	// Desktop cursor functions
	virtual void     setCursorPos(const Vector2i &pos)			{ NOT_IMPLEMENTED(setCursorPos) }
	virtual Vector2i getCursorPos() const						{ NOT_IMPLEMENTED_RET(getCursorPos, Vector2i(0)) }
	virtual void     setCursorLimits(const Recti &area)			{ NOT_IMPLEMENTED(setCursorLimits) }

	// Key state function
	virtual bool getKeyState(const xdVirtualKey key) const		{ NOT_IMPLEMENTED_RET(getKeyState, false) }

	// General position
	virtual Vector2 getPosition() const 						{ NOT_IMPLEMENTED_RET(getPosition, Vector2(0.0f)) }

	// Key binding
	void bind(const xdVirtualKey key, asIScriptFunction *func) {}
	
	// Overloads
	void setCursorPos(const int x, const int y) { setCursorPos(Vector2i(x, y)); }
	void setCursorLimits(const int x, const int y, const int w, const int h) { setCursorLimits(Recti(x, y, w, h)); }
};

#endif // X2D_INPUT_H