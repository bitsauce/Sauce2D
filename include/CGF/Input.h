#ifndef CGF_INPUT_H
#define CGF_INPUT_H

#include <CGF/config.h>
#include <CGF/math.h>

#include <CGF/input/inputContext.h>
#include <CGF/input/keyListener.h>
#include <CGF/input/mouseListener.h>

BEGIN_CGF_NAMESPACE

/* The unknown key */
#define CGF_KEY_UNKNOWN            -1

/* Printable keys */
#define CGF_KEY_SPACE              32
#define CGF_KEY_APOSTROPHE         39  /* ' */
#define CGF_KEY_COMMA              44  /* , */
#define CGF_KEY_MINUS              45  /* - */
#define CGF_KEY_PERIOD             46  /* . */
#define CGF_KEY_SLASH              47  /* / */
#define CGF_KEY_0                  48
#define CGF_KEY_1                  49
#define CGF_KEY_2                  50
#define CGF_KEY_3                  51
#define CGF_KEY_4                  52
#define CGF_KEY_5                  53
#define CGF_KEY_6                  54
#define CGF_KEY_7                  55
#define CGF_KEY_8                  56
#define CGF_KEY_9                  57
#define CGF_KEY_SEMICOLON          59  /* ; */
#define CGF_KEY_EQUAL              61  /* = */
#define CGF_KEY_A                  65
#define CGF_KEY_B                  66
#define CGF_KEY_C                  67
#define CGF_KEY_D                  68
#define CGF_KEY_E                  69
#define CGF_KEY_F                  70
#define CGF_KEY_G                  71
#define CGF_KEY_H                  72
#define CGF_KEY_I                  73
#define CGF_KEY_J                  74
#define CGF_KEY_K                  75
#define CGF_KEY_L                  76
#define CGF_KEY_M                  77
#define CGF_KEY_N                  78
#define CGF_KEY_O                  79
#define CGF_KEY_P                  80
#define CGF_KEY_Q                  81
#define CGF_KEY_R                  82
#define CGF_KEY_S                  83
#define CGF_KEY_T                  84
#define CGF_KEY_U                  85
#define CGF_KEY_V                  86
#define CGF_KEY_W                  87
#define CGF_KEY_X                  88
#define CGF_KEY_Y                  89
#define CGF_KEY_Z                  90
#define CGF_KEY_LEFT_BRACKET       91  /* [ */
#define CGF_KEY_BACKSLASH          92  /* \ */
#define CGF_KEY_RIGHT_BRACKET      93  /* ] */
#define CGF_KEY_GRAVE_ACCENT       96  /* ` */
#define CGF_KEY_WORLD_1            161 /* non-US #1 */
#define CGF_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define CGF_KEY_ESCAPE             256
#define CGF_KEY_ENTER              257
#define CGF_KEY_TAB                258
#define CGF_KEY_BACKSPACE          259
#define CGF_KEY_INSERT             260
#define CGF_KEY_DELETE             261
#define CGF_KEY_RIGHT              262
#define CGF_KEY_LEFT               263
#define CGF_KEY_DOWN               264
#define CGF_KEY_UP                 265
#define CGF_KEY_PAGE_UP            266
#define CGF_KEY_PAGE_DOWN          267
#define CGF_KEY_HOME               268
#define CGF_KEY_END                269
#define CGF_KEY_CAPS_LOCK          280
#define CGF_KEY_SCROLL_LOCK        281
#define CGF_KEY_NUM_LOCK           282
#define CGF_KEY_PRINT_SCREEN       283
#define CGF_KEY_PAUSE              284
#define CGF_KEY_F1                 290
#define CGF_KEY_F2                 291
#define CGF_KEY_F3                 292
#define CGF_KEY_F4                 293
#define CGF_KEY_F5                 294
#define CGF_KEY_F6                 295
#define CGF_KEY_F7                 296
#define CGF_KEY_F8                 297
#define CGF_KEY_F9                 298
#define CGF_KEY_F10                299
#define CGF_KEY_F11                300
#define CGF_KEY_F12                301
#define CGF_KEY_F13                302
#define CGF_KEY_F14                303
#define CGF_KEY_F15                304
#define CGF_KEY_F16                305
#define CGF_KEY_F17                306
#define CGF_KEY_F18                307
#define CGF_KEY_F19                308
#define CGF_KEY_F20                309
#define CGF_KEY_F21                310
#define CGF_KEY_F22                311
#define CGF_KEY_F23                312
#define CGF_KEY_F24                313
#define CGF_KEY_F25                314
#define CGF_KEY_KP_0               320
#define CGF_KEY_KP_1               321
#define CGF_KEY_KP_2               322
#define CGF_KEY_KP_3               323
#define CGF_KEY_KP_4               324
#define CGF_KEY_KP_5               325
#define CGF_KEY_KP_6               326
#define CGF_KEY_KP_7               327
#define CGF_KEY_KP_8               328
#define CGF_KEY_KP_9               329
#define CGF_KEY_KP_DECIMAL         330
#define CGF_KEY_KP_DIVIDE          331
#define CGF_KEY_KP_MULTIPLY        332
#define CGF_KEY_KP_SUBTRACT        333
#define CGF_KEY_KP_ADD             334
#define CGF_KEY_KP_ENTER           335
#define CGF_KEY_KP_EQUAL           336
#define CGF_KEY_LEFT_SHIFT         340
#define CGF_KEY_LEFT_CONTROL       341
#define CGF_KEY_LEFT_ALT           342
#define CGF_KEY_LEFT_SUPER         343
#define CGF_KEY_RIGHT_SHIFT        344
#define CGF_KEY_RIGHT_CONTROL      345
#define CGF_KEY_RIGHT_ALT          346
#define CGF_KEY_RIGHT_SUPER        347
#define CGF_KEY_MENU               348
#define CGF_KEY_LAST               CGF_KEY_MENU

#define CGF_MOD_SHIFT           0x0001
#define CGF_MOD_CONTROL         0x0002
#define CGF_MOD_ALT             0x0004
#define CGF_MOD_SUPER           0x0008

#define CGF_MOUSE_BUTTON_1         0
#define CGF_MOUSE_BUTTON_2         1
#define CGF_MOUSE_BUTTON_3         2
#define CGF_MOUSE_BUTTON_4         3
#define CGF_MOUSE_BUTTON_5         4
#define CGF_MOUSE_BUTTON_6         5
#define CGF_MOUSE_BUTTON_7         6
#define CGF_MOUSE_BUTTON_8         7
#define CGF_MOUSE_BUTTON_LAST      CGF_MOUSE_BUTTON_8
#define CGF_MOUSE_BUTTON_LEFT      CGF_MOUSE_BUTTON_1
#define CGF_MOUSE_BUTTON_RIGHT     CGF_MOUSE_BUTTON_2
#define CGF_MOUSE_BUTTON_MIDDLE    CGF_MOUSE_BUTTON_3

/*********************************************************************
**	Input class [static]											**
**********************************************************************/

class CGF_API Input
{
	friend class Window;
	friend class Game;
	friend class KeyListener;
	friend class MouseListener;
public:
	// Desktop cursor functions
	static void     setCursorPos(const Vector2i &pos);
	static void     setCursorPos(const int x, const int y) { setCursorPos(Vector2i(x, y)); }
	static Vector2i getCursorPos();
	static void     setCursorLimits(const Recti &area);
	static void     setCursorLimits(const int x, const int y, const int w, const int h) { setCursorLimits(Recti(x, y, w, h)); }

	// Key state function
	static int getKeyState(const SDL_Scancode scancode);

	// General position
	static Vector2 getPosition();

	// Input context
	static void setContext(InputContext *inputContext);
	static InputContext *getContext(const string &contextName);

	// Clipboard
	static string getClipboardString();
	static void setClipboardString(const string);

	// Virtual key to string
	static VirtualKey strToKey(string name);

private:
	// Update bindings
	static void updateBindings();

	// Initialize
	static void init(string file);

	// Cursor position
	static Vector2 s_position;

	// Input context
	static InputContext *s_context;
	static map<string, InputContext*> s_contextMap;

	// String to key map
	static map<string, VirtualKey> s_strToKey;

	// Mouse buttons
	static map<VirtualKey, int> s_mouseButtonState;

	static Game *s_game;
};

END_CGF_NAMESPACE

#endif // CGF_INPUT_H
