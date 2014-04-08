#ifndef X2D_APP_H
#define X2D_APP_H

#include "platform.h"
#include "math.h"

enum X2DMouseButton
{
	X2D_LeftMouseButton,
	X2D_MiddleMouseButton,
	X2D_RightMouseButton
};

// Virtual keys
enum X2DVirtualKey
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

/*********************************************************************
**	Abstract Application Manager									**
**********************************************************************/
class X2DAPI X2DApp
{
public:

	// Printing
	virtual void print(const string &msg, const X2DMessageType type) { NOT_IMPLEMENTED(print) }

	// Application functions
	virtual void processEvents()									= 0;
	virtual void exception(X2DRetCode error, const char* message)	= 0;
	virtual string getPlatformString() const						= 0;
	
	// Time functions
	virtual ulong getProgramTime()									{ NOT_IMPLEMENTED_RET(getProgramTime, 0) }
	virtual ulong getTickCount()									{ NOT_IMPLEMENTED_RET(getTickCount, 0) }
	virtual void  startHrTimer()									{ NOT_IMPLEMENTED(startHrTimer) }
	virtual float getHrTime()										{ NOT_IMPLEMENTED_RET(getHrTime, 0.0f) }

	// Virtual window functions
	virtual void enableFullscreen()									{ NOT_IMPLEMENTED(enableFullscreen) }
	virtual void disableFullscreen()								{ NOT_IMPLEMENTED(disableFullscreen) }
	virtual Array* resolutionList()									{ NOT_IMPLEMENTED_ARR(resolutionList, "Vector2i") }

	// Window flags
	virtual void enableResize()										{ NOT_IMPLEMENTED(enableResize) }
	virtual void disableResize()									{ NOT_IMPLEMENTED(disableResize) }
	virtual bool hasFocus()											{ NOT_IMPLEMENTED_RET(hasFocus, true) }

	// Window actions
	virtual void move(const int x, const int y)						{ NOT_IMPLEMENTED(move) }
	virtual void pos(int &x, int &y, bool borders)					{ NOT_IMPLEMENTED(pos) }
	virtual void resize(const int w, const int h)					{ NOT_IMPLEMENTED(resize) }
	virtual void size(int &w, int &h)								{ NOT_IMPLEMENTED(size) }
	virtual void minimize()											{ NOT_IMPLEMENTED(minimize) }
	virtual void maximize()											{ NOT_IMPLEMENTED(maximize) }
	virtual void restore()											{ NOT_IMPLEMENTED(restore) }

	// System windows
	virtual string showSaveDialog(const string &file, const string &ext, const string &title, const string &folder) { NOT_IMPLEMENTED_RET(showSaveDialog, "") }
	virtual string showOpenDialog(const string &file, const string &ext, const string &title, const string &folder) { NOT_IMPLEMENTED_RET(showOpenDialog, "") }

	// Cursor function
	virtual void setCursorPos(const int x, const int y)									{ NOT_IMPLEMENTED(setCursorPos) }
	virtual void cursorPos(int &x, int &y) const										{ NOT_IMPLEMENTED(cursorPos) }
	virtual void setCursorLimits(const int x, const int y, const int w, const int h)	{ NOT_IMPLEMENTED(setCursorLimits) }

	// Key function
	virtual bool getKeyState(const X2DVirtualKey key)				{ NOT_IMPLEMENTED_RET(getKeyState, false) }

	// Vsync
	virtual void enableVsync()										{ NOT_IMPLEMENTED(enableVsync) }
	virtual void disableVsync()										{ NOT_IMPLEMENTED(disableVsync) }
};

#endif // X2D_APP_H