//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/engine.h>

XInput::XInput()
{
	m_mousePressed[XD_MOUSE_LEFT] = m_mousePressed[XD_MOUSE_RIGHT] = m_mousePressed[XD_MOUSE_MIDDLE] = true;
}

XInput::~XInput()
{
	/*for(map<XVirtualKey, KeyBind>::iterator itr = m_keyBindings.begin(); itr != m_keyBindings.end(); ++itr)
	{
		// Release all function handles
		itr->second.function->Release();
	}
	
	for(vector<asIScriptObject*>::iterator itr = m_keyListeners.begin(); itr != m_keyListeners.end(); ++itr)
	{
		// Release keyboard listeners
		(*itr)->Release();
	}
	
	for(vector<asIScriptObject*>::iterator itr = m_clickListeners.begin(); itr != m_clickListeners.end(); ++itr)
	{
		// Release mouse listeners
		(*itr)->Release();
	}*/
}

/*void XInput::bind(const XVirtualKey key, asIScriptFunction *function)
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
}*/

void XInput::unbind(const XVirtualKey key)
{
	bind(key, 0);
}

void XInput::unbindAll()
{
	// Release all function handles
	for(map<XVirtualKey, KeyBind>::iterator itr = m_keyBindings.begin(); itr != m_keyBindings.end(); ++itr) {
		//itr->second.function->Release();
	}
	m_keyBindings.clear();
}

/*void XInput::addKeyboardListener(asIScriptObject *object)
{
	// Add keyboard listener
	if(object) {
		m_keyListeners.push_back(object);
	}
}*/

void XInput::charEvent(uint utf8char)
{
	/*for(vector<asIScriptObject*>::iterator itr = m_keyListeners.begin(); itr != m_keyListeners.end(); ++itr)
	{
		asIObjectType *type = (*itr)->GetObjectType();
		asIScriptFunction *func = type->GetMethodByDecl("void charEvent(uint)");

		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		r = ctx->SetObject(*itr); assert(r >= 0);
		r = ctx->SetArgDWord(0, utf8char); assert(r >= 0);
		r = ctx->Execute();
		r = ctx->Release();
	}*/
}

void XInput::keyPressed(const XVirtualKey key)
{
	/*for(vector<asIScriptObject*>::iterator itr = m_keyListeners.begin(); itr != m_keyListeners.end(); ++itr)
	{
		asIObjectType *type = (*itr)->GetObjectType();
		asIScriptFunction *func = type->GetMethodByDecl("void keyPressed(VirtualKey)");

		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		r = ctx->SetObject(*itr); assert(r >= 0);
		r = ctx->SetArgDWord(0, key); assert(r >= 0);
		r = ctx->Execute();
		r = ctx->Release();
	}*/
}

void XInput::keyReleased(const XVirtualKey key)
{
	/*for(vector<asIScriptObject*>::iterator itr = m_keyListeners.begin(); itr != m_keyListeners.end(); ++itr)
	{
		asIObjectType *type = (*itr)->GetObjectType();
		asIScriptFunction *func = type->GetMethodByDecl("void keyReleased(VirtualKey)");

		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		r = ctx->SetObject(*itr); assert(r >= 0);
		r = ctx->SetArgDWord(0, key); assert(r >= 0);
		r = ctx->Execute();
		r = ctx->Release();
	}*/
}

/*void XInput::addMouseListener(asIScriptObject *object)
{
	// Add keyboard listener
	if(object) {
		m_clickListeners.push_back(object);
	}
}*/

void XInput::mouseClick(const XMouseButton btn)
{
	/*for(vector<asIScriptObject*>::iterator itr = m_clickListeners.begin(); itr != m_clickListeners.end(); ++itr)
	{
		asIObjectType *type = (*itr)->GetObjectType();
		asIScriptFunction *func = type->GetMethodByDecl("void mouseClick(MouseButton)");

		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		r = ctx->SetObject(*itr); assert(r >= 0);
		r = ctx->SetArgDWord(0, btn); assert(r >= 0);
		r = ctx->Execute();
		r = ctx->Release();
	}*/
}

void XInput::mouseScroll(const int dt)
{
	/*for(vector<asIScriptObject*>::iterator itr = m_clickListeners.begin(); itr != m_clickListeners.end(); ++itr)
	{
		asIObjectType *type = (*itr)->GetObjectType();
		asIScriptFunction *func = type->GetMethodByDecl("void mouseScroll(int)");

		asIScriptContext *ctx = XScriptEngine::CreateContext();
		int r = ctx->Prepare(func); assert(r >= 0);
		r = ctx->SetObject(*itr); assert(r >= 0);
		r = ctx->SetArgDWord(0, dt); assert(r >= 0);
		r = ctx->Execute();
		r = ctx->Release();
	}*/
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
				//XFuncCall(key.function).execute();
			}
			key.pressed = true;
		}
		else
		{
			key.pressed = false;
		}
	}

	// LMB
	if(!getKeyState(XD_LMB) && m_mousePressed[XD_MOUSE_LEFT])
	{
		mouseClick(XD_MOUSE_LEFT);
	}
	m_mousePressed[XD_MOUSE_LEFT] = getKeyState(XD_LMB);
	
	// RMB
	if(!getKeyState(XD_RMB) && m_mousePressed[XD_MOUSE_RIGHT])
	{
		mouseClick(XD_MOUSE_RIGHT);
	}
	m_mousePressed[XD_MOUSE_RIGHT] = getKeyState(XD_RMB);
	
	// WHEEL
	if(!getKeyState(XD_WHEEL) && m_mousePressed[XD_MOUSE_MIDDLE])
	{
		mouseClick(XD_MOUSE_MIDDLE);
	}
	m_mousePressed[XD_MOUSE_MIDDLE] = getKeyState(XD_WHEEL);
}