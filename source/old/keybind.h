#ifndef CORE_KEYBIND_H
#define CORE_KEYBIND_H

#include "x2d/platform.h"

struct ActionKey
{
	string function;
	int key;
	bool pressed;
};

enum MouseButton
{
	NoMouseButton,
	LeftMouseButton,
	MiddleMouseButton,
	RightMouseButton
};

enum KEY_TRIGGERS
{
	KEY_UP = 0,
	KEY_DOWN = 1,
	KEY_RIGHT = 2,
	KEY_LEFT = 3,
	KEY_SPACE = 4,
	KEY_LMB = 5,
	KEY_RMB = 6,
	KEY_NONE
};

class KeyMap
{
public:

	static void Register();
	static void Destroy();
	static void BindTriggerKey(string &key, string &device, KEY_TRIGGERS val);
	static void BindKey(string &key, string &func);
	static void Message();
	static bool GetTrigger(KEY_TRIGGERS val);

private:

	static vector<ActionKey> mKeyMap;
	static map<KEY_TRIGGERS, int> mTriggerMap;
};

//bool asGetKeyState(int vkKey);

#endif // CORE_KEYBIND_H