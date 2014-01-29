//     _                                     ____  ____  
//    / \  _   _ _ __ ___  _ __ __ _  __  __|___ \|  _ \ 
//   / _ \| | | |  __/ _ \|  __/ _  | \ \/ /  __) | | | |
//  / ___ \ |_| | | | (_) | | | (_| |  >  <  / __/| |_| |
// /_/   \_\__ _|_|  \___/|_|  \__ _| /_/\_\|_____|____/ 
//		MixedGraphics (C)

#ifdef WIN32

	// Mouse keys
	/*AS_ENUM_VALUE("VirtualKey", "VK_LBUTTON", VK_LBUTTON); // Left mouse button
	AS_ENUM_VALUE("VirtualKey", "VK_RBUTTON", VK_RBUTTON); // Right mouse button
	AS_ENUM_VALUE("VirtualKey", "VK_MBUTTON", VK_MBUTTON); // Any mouse button

	// Special keys
	AS_ENUM_VALUE("VirtualKey", "VK_BACK", VK_BACK); // Backspace
	AS_ENUM_VALUE("VirtualKey", "VK_CANCEL", VK_CANCEL); // Break
	AS_ENUM_VALUE("VirtualKey", "VK_ESCAPE", VK_ESCAPE);
	AS_ENUM_VALUE("VirtualKey", "VK_TAB", VK_TAB);
	AS_ENUM_VALUE("VirtualKey", "VK_RETURN", VK_RETURN); // Enter
	AS_ENUM_VALUE("VirtualKey", "VK_SHIFT", VK_SHIFT);
	AS_ENUM_VALUE("VirtualKey", "VK_LSHIFT", VK_LSHIFT);
	AS_ENUM_VALUE("VirtualKey", "VK_RSHIFT", VK_RSHIFT);
	AS_ENUM_VALUE("VirtualKey", "VK_CONTROL", VK_CONTROL);
	AS_ENUM_VALUE("VirtualKey", "VK_LCONTROL", VK_LCONTROL);
	AS_ENUM_VALUE("VirtualKey", "VK_RCONTROL", VK_RCONTROL);
	AS_ENUM_VALUE("VirtualKey", "VK_MENU", VK_MENU); // Windows
	AS_ENUM_VALUE("VirtualKey", "VK_SPACE", VK_SPACE);
	AS_ENUM_VALUE("VirtualKey", "VK_PRIOR", VK_PRIOR); // Page up
	AS_ENUM_VALUE("VirtualKey", "VK_NEXT", VK_NEXT); // Page down
	AS_ENUM_VALUE("VirtualKey", "VK_END", VK_END);
	AS_ENUM_VALUE("VirtualKey", "VK_HOME", VK_HOME);
	AS_ENUM_VALUE("VirtualKey", "VK_LEFT", VK_LEFT);
	AS_ENUM_VALUE("VirtualKey", "VK_UP", VK_UP);
	AS_ENUM_VALUE("VirtualKey", "VK_RIGHT", VK_RIGHT);
	AS_ENUM_VALUE("VirtualKey", "VK_DOWN", VK_DOWN);
	AS_ENUM_VALUE("VirtualKey", "VK_SNAPSHOT", VK_SNAPSHOT); // Printscreen
	AS_ENUM_VALUE("VirtualKey", "VK_INSERT", VK_INSERT);
	AS_ENUM_VALUE("VirtualKey", "VK_DELETE", VK_DELETE);

	// Numpad keys
	AS_ENUM_VALUE("VirtualKey", "VK_NUMPAD0", VK_NUMPAD0);
	AS_ENUM_VALUE("VirtualKey", "VK_NUMPAD1", VK_NUMPAD1);
	AS_ENUM_VALUE("VirtualKey", "VK_NUMPAD2", VK_NUMPAD2);
	AS_ENUM_VALUE("VirtualKey", "VK_NUMPAD3", VK_NUMPAD3);
	AS_ENUM_VALUE("VirtualKey", "VK_NUMPAD4", VK_NUMPAD4);
	AS_ENUM_VALUE("VirtualKey", "VK_NUMPAD5", VK_NUMPAD5);
	AS_ENUM_VALUE("VirtualKey", "VK_NUMPAD6", VK_NUMPAD6);
	AS_ENUM_VALUE("VirtualKey", "VK_NUMPAD7", VK_NUMPAD7);
	AS_ENUM_VALUE("VirtualKey", "VK_NUMPAD8", VK_NUMPAD8);
	AS_ENUM_VALUE("VirtualKey", "VK_NUMPAD9", VK_NUMPAD9);
	AS_ENUM_VALUE("VirtualKey", "VK_ADD", VK_ADD); // Numpad add
	AS_ENUM_VALUE("VirtualKey", "VK_SUBTRACT", VK_SUBTRACT); // Numpad subtract
	AS_ENUM_VALUE("VirtualKey", "VK_DECIMAL", VK_DECIMAL); // Numpad dot
	AS_ENUM_VALUE("VirtualKey", "VK_DIVIDE", VK_DIVIDE); // Numpad divide
	AS_ENUM_VALUE("VirtualKey", "VK_MULTIPLY", VK_MULTIPLY); // Numpad multiply
	
	// OEM keys - Keyboard layout dependent keys
	//
	// Note: The following key commenting is for
	//		 Norwegian keyboard layout
	AS_ENUM_VALUE("VirtualKey", "VK_OEM_1", VK_OEM_1); // ¨ and ^
	AS_ENUM_VALUE("VirtualKey", "VK_OEM_2", VK_OEM_2); // ' and *
	AS_ENUM_VALUE("VirtualKey", "VK_OEM_3", VK_OEM_3); // ø and Ø
	AS_ENUM_VALUE("VirtualKey", "VK_OEM_4", VK_OEM_4); // \ and `
	AS_ENUM_VALUE("VirtualKey", "VK_OEM_5", VK_OEM_5); // | and §
	AS_ENUM_VALUE("VirtualKey", "VK_OEM_6", VK_OEM_6); // å and Å
	AS_ENUM_VALUE("VirtualKey", "VK_OEM_7", VK_OEM_7); // æ and Æ
	AS_ENUM_VALUE("VirtualKey", "VK_OEM_8", VK_OEM_8); // no oem 8
	AS_ENUM_VALUE("VirtualKey", "VK_OEM_102", VK_OEM_102); // < and >
	AS_ENUM_VALUE("VirtualKey", "VK_OEM_PLUS", VK_OEM_PLUS); // +
	AS_ENUM_VALUE("VirtualKey", "VK_OEM_COMMA", VK_OEM_COMMA); // ,
	AS_ENUM_VALUE("VirtualKey", "VK_OEM_MINUS", VK_OEM_MINUS); // -
	AS_ENUM_VALUE("VirtualKey", "VK_OEM_PERIOD", VK_OEM_PERIOD); // .

	// Numerical keys - Using ASCII
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_0", '0');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_1", '1');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_2", '2');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_3", '3');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_4", '4');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_5", '5');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_6", '6');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_7", '7');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_8", '8');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_9", '9');
	
	// Alphabetical keys - Using ASCII
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_A", 'A');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_B", 'B');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_C", 'C');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_D", 'D');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_E", 'E');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_F", 'F');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_G", 'G');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_H", 'H');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_I", 'I');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_J", 'J');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_K", 'K');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_L", 'L');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_M", 'M');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_N", 'N');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_O", 'O');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_P", 'P');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_Q", 'Q');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_R", 'R');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_S", 'S');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_T", 'T');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_U", 'U');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_V", 'V');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_W", 'W');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_X", 'X');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_Y", 'Y');
	AS_ENUM_VALUE("VirtualKey", "VK_KEY_Z", 'Z');*/

#include "keybind.h"
#include "app.h"

#include "core/io.h"
#include "angelscript/scripthelper.h"

vector<ActionKey> KeyMap::mKeyMap;
map<KEY_TRIGGERS, int> KeyMap::mTriggerMap;

void KeyMap::Register()
{
	int r;
	AS_ENUM_REGISTER("KeyTrigger");
	AS_ENUM_VALUE("KeyTrigger", "KEY_UP", KEY_UP);
	AS_ENUM_VALUE("KeyTrigger", "KEY_DOWN", KEY_DOWN);
	AS_ENUM_VALUE("KeyTrigger", "KEY_RIGHT", KEY_RIGHT);
	AS_ENUM_VALUE("KeyTrigger", "KEY_LEFT", KEY_LEFT);
	AS_ENUM_VALUE("KeyTrigger", "KEY_SPACE", KEY_SPACE);
	AS_ENUM_VALUE("KeyTrigger", "KEY_LMB", KEY_LMB);
	AS_ENUM_VALUE("KeyTrigger", "KEY_RMB", KEY_RMB);
	AS_GLOBAL_FUNCTION("void bindKey(string &in, string &in)", KeyMap::BindKey);
	AS_GLOBAL_FUNCTION("void bindTriggerKey(string &in, string &in, KeyTrigger value)", KeyMap::BindTriggerKey);
	AS_GLOBAL_FUNCTION("bool getTrigger(KeyTrigger value)", KeyMap::GetTrigger);
}

void KeyMap::Destroy()
{
	mKeyMap.empty();
}

void KeyMap::BindTriggerKey(string &key, string &device, KEY_TRIGGERS val)
{
	// Convert string to vk key
	int vkKey = -1;

	if(device == "keyboard")
	{
		if(key == "a")
			vkKey = 0x41;
		else if(key == "d")
			vkKey = 0x44;
		else if(key == "space")
			vkKey = VK_SPACE;

	}else if(device == "mouse"){
		if(key == "left")
			vkKey = VK_LBUTTON;
		else if(key == "right")
			vkKey = VK_RBUTTON;
	}

	mTriggerMap[val] = vkKey;
}

void KeyMap::BindKey(string &key, string &func)
{
	// Convert string to vk key
	int vkKey = -1;

	// Function keys
	if(key == "F1")
		vkKey = VK_F1;
	else if(key == "F2")
		vkKey = VK_F2;
	else if(key == "F3")
		vkKey = VK_F3;
	else if(key == "F4")
		vkKey = VK_F4;
	else if(key == "F5")
		vkKey = VK_F5;
	else if(key == "F6")
		vkKey = VK_F6;
	else if(key == "F7")
		vkKey = VK_F7;
	else if(key == "F8")
		vkKey = VK_F8;
	else if(key == "F9")
		vkKey = VK_F9;
	else if(key == "F10")
		vkKey = VK_F10;
	else if(key == "F11")
		vkKey = VK_F11;
	else if(key == "F12")
		vkKey = VK_F12;
	// OEM
	else if(key == "|")
		vkKey = VK_OEM_5;

	// Sanity check
	if(vkKey == -1)
	{
		print(string("Unable to map '" + key + "' as a key").c_str());
		return;
	}

	// Create the ActionKey
	ActionKey actionKey;
	actionKey.function = func;
	actionKey.key = vkKey;
	actionKey.pressed = false;
	mKeyMap.push_back(actionKey);
}

bool KeyMap::GetTrigger(KEY_TRIGGERS val)
{
	if(GetKeyState(mTriggerMap[val]) & 0x80)
		return true;
	return false;
}

void KeyMap::Message()
{
	// Iterate all keys
	for(vector<ActionKey>::iterator key = mKeyMap.begin(); key != mKeyMap.end(); ++key)
	{
		// If a key is pressed
		if(GetKeyState((*key).key) & 0x80)
		{
			// If the key is pressed from before, return
			if((*key).pressed == true)
				return;

			// If not, call the function and set it as pressed
			(*key).pressed = true;
			ExecuteString(scriptEngine, (*key).function.c_str(), scriptEngine->GetModule("GameModule"));
		}else{
			// Key is not pressed
			(*key).pressed = false;
		}
	}
}

bool asGetKeyState(int vkKey)
{
	if(gameEngine->app->hasFocus() && GetKeyState(vkKey) & 0x80)
		return true;
	return false;
}

#endif