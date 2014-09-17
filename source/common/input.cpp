//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>
#include <x2d/extention.h>

AS_REG_SINGLETON(XInput)

int XInput::Register(asIScriptEngine *scriptEngine)
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
	r = scriptEngine->RegisterObjectMethod("XInput", "void     setCursorPos(const Vector2i &in)", asMETHODPR(XInput, setCursorPos, (const Vector2i&), void), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XInput", "Vector2i getCursorPos() const", asMETHOD(XInput, getCursorPos), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XInput", "void     setCursorLimits(const Recti &in) const", asMETHODPR(XInput, setCursorLimits, (const Recti&), void), asCALL_THISCALL); AS_ASSERT

	// Keyboard
	r = scriptEngine->RegisterObjectMethod("XInput", "bool getKeyState(const VirtualKey key) const", asMETHOD(XInput, getKeyState), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XInput", "void bind(const VirtualKey key, KeybindCallback @callback)", asMETHOD(XInput, bind), asCALL_THISCALL); 
	r = scriptEngine->RegisterObjectMethod("XInput", "void unbind(const VirtualKey key)", asMETHOD(XInput, unbind), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XInput", "void unbindAll()", asMETHOD(XInput, unbindAll), asCALL_THISCALL); AS_ASSERT
	r = scriptEngine->RegisterObjectMethod("XInput", "void addKeyboardListener(KeyboardListener@)", asMETHOD(XInput, addKeyboardListener), asCALL_THISCALL); AS_ASSERT

	// General
	r = scriptEngine->RegisterObjectMethod("XInput", "Vector2 get_position() const", asMETHOD(XInput, getPosition), asCALL_THISCALL); AS_ASSERT

	return r;
}

XInput::~XInput()
{
	for(map<XVirtualKey, KeyBind>::iterator itr = m_keyBindings.begin(); itr != m_keyBindings.end(); ++itr)
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

void XInput::bind(const XVirtualKey key, asIScriptFunction *function)
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

void XInput::unbind(const XVirtualKey key)
{
	bind(key, 0);
}

void XInput::unbindAll()
{
	// Release all function handles
	for(map<XVirtualKey, KeyBind>::iterator itr = m_keyBindings.begin(); itr != m_keyBindings.end(); ++itr) {
		itr->second.function->Release();
	}
	m_keyBindings.clear();
}

void XInput::addKeyboardListener(asIScriptObject *object)
{
	// Add keyboard listener
	if(object) {
		m_keyListeners.push_back(object);
	}
}

void XInput::charEvent(uint utf8char)
{
	for(vector<asIScriptObject*>::iterator itr = m_keyListeners.begin(); itr != m_keyListeners.end(); ++itr)
	{
		asIObjectType *type = (*itr)->GetObjectType();
		asIScriptFunction *func = type->GetMethodByDecl("void charEvent(uint)");

		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		r = ctx->SetObject(*itr); assert(r >= 0);
		r = ctx->SetArgDWord(0, utf8char); assert(r >= 0);
		r = ctx->Execute();
		r = ctx->Release();
	}
}

void XInput::keyPressed(const XVirtualKey key)
{
	for(vector<asIScriptObject*>::iterator itr = m_keyListeners.begin(); itr != m_keyListeners.end(); ++itr)
	{
		asIObjectType *type = (*itr)->GetObjectType();
		asIScriptFunction *func = type->GetMethodByDecl("void keyPressed(VirtualKey)");

		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		r = ctx->SetObject(*itr); assert(r >= 0);
		r = ctx->SetArgDWord(0, key); assert(r >= 0);
		r = ctx->Execute();
		r = ctx->Release();
	}
}

void XInput::keyReleased(const XVirtualKey key)
{
	for(vector<asIScriptObject*>::iterator itr = m_keyListeners.begin(); itr != m_keyListeners.end(); ++itr)
	{
		asIObjectType *type = (*itr)->GetObjectType();
		asIScriptFunction *func = type->GetMethodByDecl("void keyReleased(VirtualKey)");

		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		r = ctx->SetObject(*itr); assert(r >= 0);
		r = ctx->SetArgDWord(0, key); assert(r >= 0);
		r = ctx->Execute();
		r = ctx->Release();
	}
}

void XInput::checkBindings()
{
	// Iterate key bindings
	for(map<XVirtualKey, KeyBind>::iterator itr = m_keyBindings.begin(); itr != m_keyBindings.end(); ++itr)
	{
		KeyBind &key = itr->second;
		if(getKeyState(itr->first))
		{
			if(!key.pressed)
			{
				// Key was pressed, call function
				XFuncCall(key.function).execute();
			}
			key.pressed = true;
		}else{
			key.pressed = false;
		}
	}
}