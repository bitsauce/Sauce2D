//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/input.h>

AS_REG_SINGLETON(xdInput, "ScriptInput")

int xdInput::Register(asIScriptEngine *scriptEngine)
{
	int r;

	// Mouse buttons
	r = scriptEngine->RegisterEnum("MouseButton"); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("MouseButton", "MOUSE_LEFT", X2D_LeftMouseButton); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("MouseButton", "MOUSE_RIGHT", X2D_RightMouseButton); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("MouseButton", "MOUSE_MIDDLE", X2D_MiddleMouseButton); AS_ASSERT
	
	// Virtual keys
	r = scriptEngine->RegisterEnum("VirtualKey"); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_LMB", X2D_LeftMouse); AS_ASSERT // Left mouse button
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_RMB", X2D_RightMouse); AS_ASSERT // Right mouse button
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_MMB", X2D_MiddleMouse); AS_ASSERT // Any mouse button
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_BACKSPACE", X2D_KeyBackspace); AS_ASSERT // Backspace
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_ESCAPE", X2D_KeyEscape); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_TAB", X2D_KeyTab); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_ENTER", X2D_KeyEnter); AS_ASSERT // Enter
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_SHIFT", X2D_KeyShift); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_LEFT_SHIFT", X2D_KeyLeftShift); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_RIGHT_SHIFT", X2D_KeyRightShift); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_CONTROL", X2D_KeyControl); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_LEFT_CONTROL", X2D_KeyLeftControl); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_RIGHT_CONTROL", X2D_KeyRightControl); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_MENU", X2D_KeyMenu); AS_ASSERT // Windows
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_LEFT_MENU", X2D_KeyLeftMenu); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_RIGHT_MENU", X2D_KeyRightMenu); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_SPACE", X2D_KeySpace); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_PAGE_UP", X2D_KeyPageUp); AS_ASSERT // Page up
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_PAGE_DOWN", X2D_KeyPageDown); AS_ASSERT // Page down
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_END", X2D_KeyEnd); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_HOME", X2D_KeyHome); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_LEFT", X2D_KeyLeft); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_UP", X2D_KeyUp); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_RIGHT", X2D_KeyRight); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_DOWN", X2D_KeyDown); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_SNAPSHOT", X2D_KeySnapshot); AS_ASSERT // Printscreen
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_INSERT", X2D_KeyInsert); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_DELETE", X2D_KeyDelete); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_0", X2D_KeyNumpad0); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_1", X2D_KeyNumpad1); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_2", X2D_KeyNumpad2); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_3", X2D_KeyNumpad3); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_4", X2D_KeyNumpad4); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_5", X2D_KeyNumpad5); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_6", X2D_KeyNumpad6); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_7", X2D_KeyNumpad7); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_8", X2D_KeyNumpad8); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_9", X2D_KeyNumpad9); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_PULSS", X2D_KeyPluss); AS_ASSERT // Numpad add
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_HYPHEN", X2D_KeyHyphen); AS_ASSERT // Numpad subtract
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_PERIOD", X2D_KeyPeriod); AS_ASSERT // Numpad dot
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_SLASH", X2D_KeySlash); AS_ASSERT // Numpad divide
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_ASTERISK", X2D_KeyAsterisk); AS_ASSERT // Numpad multiply
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_CARET", X2D_KeyCaret); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_APOSTROPHE", X2D_KeyApostrophe); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_QUESTION_MARK", X2D_KeyQuotationMark); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_BACKSLASH", X2D_KeyBackslash); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_PIPE", X2D_KeyPipe); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_COLON", X2D_KeyColon); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_SEMICOLON", X2D_KeySemicolon); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_TILDE", X2D_KeyTilde); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_COMMA", X2D_KeyComma); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_GREATER", X2D_KeyGreater); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_LESS", X2D_KeyLess); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_EQUALS", X2D_KeyEquals); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_0", X2D_Key0); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_1", X2D_Key1); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_2", X2D_Key2); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_3", X2D_Key3); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_4", X2D_Key4); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_5", X2D_Key5); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_6", X2D_Key6); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_7", X2D_Key7); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_8", X2D_Key8); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_9", X2D_Key9); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_A", X2D_KeyA); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_B", X2D_KeyB); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_C", X2D_KeyC); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_D", X2D_KeyD); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_E", X2D_KeyE); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_F", X2D_KeyF); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_G", X2D_KeyG); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_H", X2D_KeyH); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_I", X2D_KeyI); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_J", X2D_KeyJ); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_K", X2D_KeyK); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_L", X2D_KeyL); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_M", X2D_KeyM); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_N", X2D_KeyN); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_O", X2D_KeyO); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_P", X2D_KeyP); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_Q", X2D_KeyQ); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_R", X2D_KeyR); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_S", X2D_KeyS); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_T", X2D_KeyT); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_U", X2D_KeyU); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_V", X2D_KeyV); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_W", X2D_KeyW); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_X", X2D_KeyX); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_Y", X2D_KeyY); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_Z", X2D_KeyZ); AS_ASSERT

	// Callback funcdef
	r = scriptEngine->RegisterFuncdef("void Callback()"); AS_ASSERT

	// Desktop cursor
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "void     setCursorPos(const Vector2i &in)", asMETHODPR(xdInput, setCursorPos, (const Vector2i&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "Vector2i getCursorPos() const", asMETHOD(xdInput, getCursorPos), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "void     setCursorLimits(const Recti &in) const", asMETHODPR(xdInput, setCursorLimits, (const Recti&), void), asCALL_THISCALL); AS_ASSERT

	// Keyboard
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "bool getKeyState(const VirtualKey key) const", asMETHOD(xdInput, getKeyState), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "void bind(const VirtualKey key, Callback @callback)", asMETHOD(xdInput, bind), asCALL_THISCALL); AS_ASSERT

	// General
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "Vector2 get_position() const", asMETHOD(xdInput, getPosition), asCALL_THISCALL); AS_ASSERT

	return r;
}