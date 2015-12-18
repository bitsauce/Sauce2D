#ifndef X2D_INPUT_H
#define X2D_INPUT_H

#include <x2d/config.h>
#include <x2d/math.h>

#include <x2d/input/inputContext.h>
#include <x2d/input/keyListener.h>
#include <x2d/input/mouseListener.h>

BEGIN_CG_NAMESPACE

/* The unknown key */
#define XD_KEY_UNKNOWN            -1

/* Printable keys */
#define XD_KEY_SPACE              32
#define XD_KEY_APOSTROPHE         39  /* ' */
#define XD_KEY_COMMA              44  /* , */
#define XD_KEY_MINUS              45  /* - */
#define XD_KEY_PERIOD             46  /* . */
#define XD_KEY_SLASH              47  /* / */
#define XD_KEY_0                  48
#define XD_KEY_1                  49
#define XD_KEY_2                  50
#define XD_KEY_3                  51
#define XD_KEY_4                  52
#define XD_KEY_5                  53
#define XD_KEY_6                  54
#define XD_KEY_7                  55
#define XD_KEY_8                  56
#define XD_KEY_9                  57
#define XD_KEY_SEMICOLON          59  /* ; */
#define XD_KEY_EQUAL              61  /* = */
#define XD_KEY_A                  65
#define XD_KEY_B                  66
#define XD_KEY_C                  67
#define XD_KEY_D                  68
#define XD_KEY_E                  69
#define XD_KEY_F                  70
#define XD_KEY_G                  71
#define XD_KEY_H                  72
#define XD_KEY_I                  73
#define XD_KEY_J                  74
#define XD_KEY_K                  75
#define XD_KEY_L                  76
#define XD_KEY_M                  77
#define XD_KEY_N                  78
#define XD_KEY_O                  79
#define XD_KEY_P                  80
#define XD_KEY_Q                  81
#define XD_KEY_R                  82
#define XD_KEY_S                  83
#define XD_KEY_T                  84
#define XD_KEY_U                  85
#define XD_KEY_V                  86
#define XD_KEY_W                  87
#define XD_KEY_X                  88
#define XD_KEY_Y                  89
#define XD_KEY_Z                  90
#define XD_KEY_LEFT_BRACKET       91  /* [ */
#define XD_KEY_BACKSLASH          92  /* \ */
#define XD_KEY_RIGHT_BRACKET      93  /* ] */
#define XD_KEY_GRAVE_ACCENT       96  /* ` */
#define XD_KEY_WORLD_1            161 /* non-US #1 */
#define XD_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define XD_KEY_ESCAPE             256
#define XD_KEY_ENTER              257
#define XD_KEY_TAB                258
#define XD_KEY_BACKSPACE          259
#define XD_KEY_INSERT             260
#define XD_KEY_DELETE             261
#define XD_KEY_RIGHT              262
#define XD_KEY_LEFT               263
#define XD_KEY_DOWN               264
#define XD_KEY_UP                 265
#define XD_KEY_PAGE_UP            266
#define XD_KEY_PAGE_DOWN          267
#define XD_KEY_HOME               268
#define XD_KEY_END                269
#define XD_KEY_CAPS_LOCK          280
#define XD_KEY_SCROLL_LOCK        281
#define XD_KEY_NUM_LOCK           282
#define XD_KEY_PRINT_SCREEN       283
#define XD_KEY_PAUSE              284
#define XD_KEY_F1                 290
#define XD_KEY_F2                 291
#define XD_KEY_F3                 292
#define XD_KEY_F4                 293
#define XD_KEY_F5                 294
#define XD_KEY_F6                 295
#define XD_KEY_F7                 296
#define XD_KEY_F8                 297
#define XD_KEY_F9                 298
#define XD_KEY_F10                299
#define XD_KEY_F11                300
#define XD_KEY_F12                301
#define XD_KEY_F13                302
#define XD_KEY_F14                303
#define XD_KEY_F15                304
#define XD_KEY_F16                305
#define XD_KEY_F17                306
#define XD_KEY_F18                307
#define XD_KEY_F19                308
#define XD_KEY_F20                309
#define XD_KEY_F21                310
#define XD_KEY_F22                311
#define XD_KEY_F23                312
#define XD_KEY_F24                313
#define XD_KEY_F25                314
#define XD_KEY_KP_0               320
#define XD_KEY_KP_1               321
#define XD_KEY_KP_2               322
#define XD_KEY_KP_3               323
#define XD_KEY_KP_4               324
#define XD_KEY_KP_5               325
#define XD_KEY_KP_6               326
#define XD_KEY_KP_7               327
#define XD_KEY_KP_8               328
#define XD_KEY_KP_9               329
#define XD_KEY_KP_DECIMAL         330
#define XD_KEY_KP_DIVIDE          331
#define XD_KEY_KP_MULTIPLY        332
#define XD_KEY_KP_SUBTRACT        333
#define XD_KEY_KP_ADD             334
#define XD_KEY_KP_ENTER           335
#define XD_KEY_KP_EQUAL           336
#define XD_KEY_LEFT_SHIFT         340
#define XD_KEY_LEFT_CONTROL       341
#define XD_KEY_LEFT_ALT           342
#define XD_KEY_LEFT_SUPER         343
#define XD_KEY_RIGHT_SHIFT        344
#define XD_KEY_RIGHT_CONTROL      345
#define XD_KEY_RIGHT_ALT          346
#define XD_KEY_RIGHT_SUPER        347
#define XD_KEY_MENU               348
#define XD_KEY_LAST               XD_KEY_MENU

#define XD_MOD_SHIFT           0x0001
#define XD_MOD_CONTROL         0x0002
#define XD_MOD_ALT             0x0004
#define XD_MOD_SUPER           0x0008

#define XD_MOUSE_BUTTON_1         0
#define XD_MOUSE_BUTTON_2         1
#define XD_MOUSE_BUTTON_3         2
#define XD_MOUSE_BUTTON_4         3
#define XD_MOUSE_BUTTON_5         4
#define XD_MOUSE_BUTTON_6         5
#define XD_MOUSE_BUTTON_7         6
#define XD_MOUSE_BUTTON_8         7
#define XD_MOUSE_BUTTON_LAST      XD_MOUSE_BUTTON_8
#define XD_MOUSE_BUTTON_LEFT      XD_MOUSE_BUTTON_1
#define XD_MOUSE_BUTTON_RIGHT     XD_MOUSE_BUTTON_2
#define XD_MOUSE_BUTTON_MIDDLE    XD_MOUSE_BUTTON_3

/*********************************************************************
**	Input class [static]											**
**********************************************************************/

class XDAPI Input
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

END_CG_NAMESPACE

#endif // X2D_INPUT_H