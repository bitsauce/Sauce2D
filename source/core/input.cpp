//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/input.h>

#include <x2d/scriptengine.h>
#include <x2d/scripts/funccall.h>

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
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_BACKSPACE", XD_KEY_BACKSPACE); AS_ASSERT // Backspace
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_ESCAPE", XD_KEY_ESCAPE); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_TAB", XD_KEY_TAB); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_ENTER", XD_KEY_ENTER); AS_ASSERT // Enter
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_SHIFT", XD_KEY_Shift); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_LEFT_SHIFT", XD_KEY_LeftShift); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_RIGHT_SHIFT", XD_KEY_RightShift); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_CONTROL", XD_KEY_Control); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_LEFT_CONTROL", XD_KEY_LeftControl); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_RIGHT_CONTROL", XD_KEY_RightControl); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_MENU", XD_KEY_Menu); AS_ASSERT // Windows
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_LEFT_MENU", XD_KEY_LeftMenu); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_RIGHT_MENU", XD_KEY_RightMenu); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_SPACE", XD_KEY_Space); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_PAGE_UP", XD_KEY_PageUp); AS_ASSERT // Page up
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_PAGE_DOWN", XD_KEY_PageDown); AS_ASSERT // Page down
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_END", XD_KEY_End); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_HOME", XD_KEY_Home); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_LEFT", XD_KEY_Left); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_UP", XD_KEY_Up); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_RIGHT", XD_KEY_Right); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_DOWN", XD_KEY_Down); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_SNAPSHOT", XD_KEY_Snapshot); AS_ASSERT // Printscreen
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_INSERT", XD_KEY_Insert); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_DELETE", XD_KEY_Delete); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_0", XD_KEY_Numpad0); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_1", XD_KEY_Numpad1); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_2", XD_KEY_Numpad2); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_3", XD_KEY_Numpad3); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_4", XD_KEY_Numpad4); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_5", XD_KEY_Numpad5); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_6", XD_KEY_Numpad6); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_7", XD_KEY_Numpad7); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_8", XD_KEY_Numpad8); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_NUMPAD_9", XD_KEY_Numpad9); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_PLUS", XD_KEY_Plus); AS_ASSERT // Numpad add
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_HYPHEN", XD_KEY_Hyphen); AS_ASSERT // Numpad subtract
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_PERIOD", XD_KEY_Period); AS_ASSERT // Numpad dot
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_SLASH", XD_KEY_Slash); AS_ASSERT // Numpad divide
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_ASTERISK", XD_KEY_Asterisk); AS_ASSERT // Numpad multiply
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_CARET", XD_KEY_Caret); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_APOSTROPHE", XD_KEY_Apostrophe); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_QUESTION_MARK", XD_KEY_QuotationMark); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_BACKSLASH", XD_KEY_Backslash); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_PIPE", XD_KEY_Pipe); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_COLON", XD_KEY_Colon); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_SEMICOLON", XD_KEY_Semicolon); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_TILDE", XD_KEY_Tilde); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_COMMA", XD_KEY_Comma); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_GREATER", XD_KEY_Greater); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_LESS", XD_KEY_Less); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_EQUALS", XD_KEY_Equals); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_0", XD_KEY_0); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_1", XD_KEY_1); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_2", XD_KEY_2); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_3", XD_KEY_3); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_4", XD_KEY_4); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_5", XD_KEY_5); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_6", XD_KEY_6); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_7", XD_KEY_7); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_8", XD_KEY_8); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_9", XD_KEY_9); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_A", XD_KEY_A); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_B", XD_KEY_B); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_C", XD_KEY_C); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_D", XD_KEY_D); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_E", XD_KEY_E); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_F", XD_KEY_F); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_G", XD_KEY_G); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_H", XD_KEY_H); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_I", XD_KEY_I); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_J", XD_KEY_J); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_K", XD_KEY_K); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_L", XD_KEY_L); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_M", XD_KEY_M); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_N", XD_KEY_N); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_O", XD_KEY_O); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_P", XD_KEY_P); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_Q", XD_KEY_Q); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_R", XD_KEY_R); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_S", XD_KEY_S); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_T", XD_KEY_T); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_U", XD_KEY_U); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_V", XD_KEY_V); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_W", XD_KEY_W); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_X", XD_KEY_X); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_Y", XD_KEY_Y); AS_ASSERT
	r = scriptEngine->RegisterEnumValue("VirtualKey", "KEY_Z", XD_KEY_Z); AS_ASSERT

	// Callback funcdef
	r = scriptEngine->RegisterFuncdef("void KeybindCallback()"); AS_ASSERT

	// Keyboard listener
	r = scriptEngine->RegisterInterface("KeyboardListener"); AS_ASSERT
	r = scriptEngine->RegisterInterfaceMethod("KeyboardListener", "void charEvent(uint)"); AS_ASSERT
	r = scriptEngine->RegisterInterfaceMethod("KeyboardListener", "void keyPressed(VirtualKey key)"); AS_ASSERT
	r = scriptEngine->RegisterInterfaceMethod("KeyboardListener", "void keyReleased(VirtualKey key)"); AS_ASSERT

	// Desktop cursor
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "void     setCursorPos(const Vector2i &in)", asMETHODPR(xdInput, setCursorPos, (const Vector2i&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "Vector2i getCursorPos() const", asMETHOD(xdInput, getCursorPos), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "void     setCursorLimits(const Recti &in) const", asMETHODPR(xdInput, setCursorLimits, (const Recti&), void), asCALL_THISCALL); AS_ASSERT

	// Keyboard
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "bool getKeyState(const VirtualKey key) const", asMETHOD(xdInput, getKeyState), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "void bind(const VirtualKey key, KeybindCallback @callback)", asMETHOD(xdInput, bind), asCALL_THISCALL); 
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "void unbind(const VirtualKey key)", asMETHOD(xdInput, unbind), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "void unbindAll()", asMETHOD(xdInput, unbindAll), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "void addKeyboardListener(KeyboardListener@)", asMETHOD(xdInput, addKeyboardListener), asCALL_THISCALL); AS_ASSERT

	// General
	r = scriptEngine->RegisterObjectMethod("ScriptInput", "Vector2 get_position() const", asMETHOD(xdInput, getPosition), asCALL_THISCALL); AS_ASSERT

	return r;
}

xdInput::~xdInput()
{
	for(map<xdVirtualKey, KeyBind>::iterator itr = m_keyBindings.begin(); itr != m_keyBindings.end(); ++itr)
	{
		// Release all function handles
		itr->second.function->Release();
	}
	
	for(vector<asIScriptObject*>::iterator itr = m_keyListeners.begin(); itr != m_keyListeners.end(); ++itr)
	{
		// Release keyboard listeners
		(*itr)->Release();
	}
	
}

void xdInput::bind(const xdVirtualKey key, asIScriptFunction *function)
{
	// If key is already bound, release its function handle
	if(m_keyBindings.find(key) != m_keyBindings.end())
	{
		// Relese function ref
		m_keyBindings[key].function->Release();

		// If there is no function
		if(!function)
		{
			// Remove key binding
			m_keyBindings.erase(key);
		}
	}

	if(function)
	{
		// Bind function to key
		m_keyBindings[key].function = function;
		m_keyBindings[key].pressed = false;
	}
}

void xdInput::unbind(const xdVirtualKey key)
{
	bind(key, 0);
}

void xdInput::unbindAll()
{
	// Release all function handles
	for(map<xdVirtualKey, KeyBind>::iterator itr = m_keyBindings.begin(); itr != m_keyBindings.end(); ++itr) {
		itr->second.function->Release();
	}
	m_keyBindings.clear();
}

void xdInput::addKeyboardListener(asIScriptObject *object)
{
	// Add keyboard listener
	if(object) {
		m_keyListeners.push_back(object);
	}
}

void xdInput::charEvent(uint utf8char)
{
	for(vector<asIScriptObject*>::iterator itr = m_keyListeners.begin(); itr != m_keyListeners.end(); ++itr)
	{
		asIObjectType *type = (*itr)->GetObjectType();
		asIScriptFunction *func = type->GetMethodByDecl("void charEvent(uint)");

		asIScriptContext *ctx = g_engine->getScriptEngine()->createContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		r = ctx->SetObject(*itr); assert(r >= 0);
		r = ctx->SetArgDWord(0, utf8char); assert(r >= 0);
		r = ctx->Execute(); assert(r >= 0);
		r = ctx->Release(); assert(r >= 0);
	}
}

void xdInput::keyPressed(xdVirtualKey key)
{
	for(vector<asIScriptObject*>::iterator itr = m_keyListeners.begin(); itr != m_keyListeners.end(); ++itr)
	{
		asIObjectType *type = (*itr)->GetObjectType();
		asIScriptFunction *func = type->GetMethodByDecl("void keyPressed(VirtualKey)");

		asIScriptContext *ctx = g_engine->getScriptEngine()->createContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		r = ctx->SetObject(*itr); assert(r >= 0);
		r = ctx->SetArgDWord(0, key); assert(r >= 0);
		r = ctx->Execute(); assert(r >= 0);
		r = ctx->Release(); assert(r >= 0);
	}
}

void xdInput::keyReleased(xdVirtualKey key)
{
	for(vector<asIScriptObject*>::iterator itr = m_keyListeners.begin(); itr != m_keyListeners.end(); ++itr)
	{
		asIObjectType *type = (*itr)->GetObjectType();
		asIScriptFunction *func = type->GetMethodByDecl("void keyReleased(VirtualKey)");

		asIScriptContext *ctx = g_engine->getScriptEngine()->createContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		r = ctx->SetObject(*itr); assert(r >= 0);
		r = ctx->SetArgDWord(0, key); assert(r >= 0);
		r = ctx->Execute(); assert(r >= 0);
		r = ctx->Release(); assert(r >= 0);
	}
}

void xdInput::checkBindings()
{
	// Iterate key bindings
	for(map<xdVirtualKey, KeyBind>::iterator itr = m_keyBindings.begin(); itr != m_keyBindings.end(); ++itr)
	{
		KeyBind &key = itr->second;
		if(getKeyState(itr->first))
		{
			if(!key.pressed)
			{
				// Key was pressed, call function
				FunctionCall *funcCall = CreateFuncCall();
				funcCall->Prepare(key.function);
				funcCall->Execute();
				delete funcCall;
			}
			key.pressed = true;
		}else{
			key.pressed = false;
		}
	}
}