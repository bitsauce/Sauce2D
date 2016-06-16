/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2014 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**
 *  \file ScanCodes.h
 *
 *  Defines keyboard scancodes.
 */

#ifndef SAUCE_SCANCODES_H
#define SAUCE_SCANCODES_H

#include <Sauce/Config.h>

BEGIN_SAUCE_NAMESPACE

/**
 *  \brief The CGF keyboard scancode representation.
 *
 *  Values of this type are used to represent keyboard keys, among other places
 *  in the \link SAUCE_Keysym::scancode key.keysym.scancode \endlink field of the
 *  SAUCE_Event structure.
 *
 *  The values in this enumeration are based on the USB usage page standard:
 *  http://www.usb.org/developers/devclass_docs/Hut1_12v2.pdf
 */
enum Scancode
{
    SAUCE_SCANCODE_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    SAUCE_SCANCODE_A = 4,
    SAUCE_SCANCODE_B = 5,
    SAUCE_SCANCODE_C = 6,
    SAUCE_SCANCODE_D = 7,
    SAUCE_SCANCODE_E = 8,
    SAUCE_SCANCODE_F = 9,
    SAUCE_SCANCODE_G = 10,
    SAUCE_SCANCODE_H = 11,
    SAUCE_SCANCODE_I = 12,
    SAUCE_SCANCODE_J = 13,
    SAUCE_SCANCODE_K = 14,
    SAUCE_SCANCODE_L = 15,
    SAUCE_SCANCODE_M = 16,
    SAUCE_SCANCODE_N = 17,
    SAUCE_SCANCODE_O = 18,
    SAUCE_SCANCODE_P = 19,
    SAUCE_SCANCODE_Q = 20,
    SAUCE_SCANCODE_R = 21,
    SAUCE_SCANCODE_S = 22,
    SAUCE_SCANCODE_T = 23,
    SAUCE_SCANCODE_U = 24,
    SAUCE_SCANCODE_V = 25,
    SAUCE_SCANCODE_W = 26,
    SAUCE_SCANCODE_X = 27,
    SAUCE_SCANCODE_Y = 28,
    SAUCE_SCANCODE_Z = 29,

    SAUCE_SCANCODE_1 = 30,
    SAUCE_SCANCODE_2 = 31,
    SAUCE_SCANCODE_3 = 32,
    SAUCE_SCANCODE_4 = 33,
    SAUCE_SCANCODE_5 = 34,
    SAUCE_SCANCODE_6 = 35,
    SAUCE_SCANCODE_7 = 36,
    SAUCE_SCANCODE_8 = 37,
    SAUCE_SCANCODE_9 = 38,
    SAUCE_SCANCODE_0 = 39,

    SAUCE_SCANCODE_RETURN = 40,
    SAUCE_SCANCODE_ESCAPE = 41,
    SAUCE_SCANCODE_BACKSPACE = 42,
    SAUCE_SCANCODE_TAB = 43,
    SAUCE_SCANCODE_SPACE = 44,

    SAUCE_SCANCODE_MINUS = 45,
    SAUCE_SCANCODE_EQUALS = 46,
    SAUCE_SCANCODE_LEFTBRACKET = 47,
    SAUCE_SCANCODE_RIGHTBRACKET = 48,
    SAUCE_SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
                                  *   key on ISO keyboards and at the right end
                                  *   of the QWERTY row on ANSI keyboards.
                                  *   Produces REVERSE SOLIDUS (backslash) and
                                  *   VERTICAL LINE in a US layout, REVERSE
                                  *   SOLIDUS and VERTICAL LINE in a UK Mac
                                  *   layout, NUMBER SIGN and TILDE in a UK
                                  *   Windows layout, DOLLAR SIGN and POUND SIGN
                                  *   in a Swiss German layout, NUMBER SIGN and
                                  *   APOSTROPHE in a German layout, GRAVE
                                  *   ACCENT and POUND SIGN in a French Mac
                                  *   layout, and ASTERISK and MICRO SIGN in a
                                  *   French Windows layout.
                                  */
    SAUCE_SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate SAUCE_SCANCODE_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    SAUCE_SCANCODE_SEMICOLON = 51,
    SAUCE_SCANCODE_APOSTROPHE = 52,
    SAUCE_SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
                              *   and ISO keyboards). Produces GRAVE ACCENT and
                              *   TILDE in a US Windows layout and in US and UK
                              *   Mac layouts on ANSI keyboards, GRAVE ACCENT
                              *   and NOT SIGN in a UK Windows layout, SECTION
                              *   SIGN and PLUS-MINUS SIGN in US and UK Mac
                              *   layouts on ISO keyboards, SECTION SIGN and
                              *   DEGREE SIGN in a Swiss German layout (Mac:
                              *   only on ISO keyboards), CIRCUMFLEX ACCENT and
                              *   DEGREE SIGN in a German layout (Mac: only on
                              *   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
                              *   French Windows layout, COMMERCIAL AT and
                              *   NUMBER SIGN in a French Mac layout on ISO
                              *   keyboards, and LESS-THAN SIGN and GREATER-THAN
                              *   SIGN in a Swiss German, German, or French Mac
                              *   layout on ANSI keyboards.
                              */
    SAUCE_SCANCODE_COMMA = 54,
    SAUCE_SCANCODE_PERIOD = 55,
    SAUCE_SCANCODE_SLASH = 56,

    SAUCE_SCANCODE_CAPSLOCK = 57,

    SAUCE_SCANCODE_F1 = 58,
    SAUCE_SCANCODE_F2 = 59,
    SAUCE_SCANCODE_F3 = 60,
    SAUCE_SCANCODE_F4 = 61,
    SAUCE_SCANCODE_F5 = 62,
    SAUCE_SCANCODE_F6 = 63,
    SAUCE_SCANCODE_F7 = 64,
    SAUCE_SCANCODE_F8 = 65,
    SAUCE_SCANCODE_F9 = 66,
    SAUCE_SCANCODE_F10 = 67,
    SAUCE_SCANCODE_F11 = 68,
    SAUCE_SCANCODE_F12 = 69,

    SAUCE_SCANCODE_PRINTSCREEN = 70,
    SAUCE_SCANCODE_SCROLLLOCK = 71,
    SAUCE_SCANCODE_PAUSE = 72,
    SAUCE_SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    SAUCE_SCANCODE_HOME = 74,
    SAUCE_SCANCODE_PAGEUP = 75,
    SAUCE_SCANCODE_DELETE = 76,
    SAUCE_SCANCODE_END = 77,
    SAUCE_SCANCODE_PAGEDOWN = 78,
    SAUCE_SCANCODE_RIGHT = 79,
    SAUCE_SCANCODE_LEFT = 80,
    SAUCE_SCANCODE_DOWN = 81,
    SAUCE_SCANCODE_UP = 82,

    SAUCE_SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    SAUCE_SCANCODE_KP_DIVIDE = 84,
    SAUCE_SCANCODE_KP_MULTIPLY = 85,
    SAUCE_SCANCODE_KP_MINUS = 86,
    SAUCE_SCANCODE_KP_PLUS = 87,
    SAUCE_SCANCODE_KP_ENTER = 88,
    SAUCE_SCANCODE_KP_1 = 89,
    SAUCE_SCANCODE_KP_2 = 90,
    SAUCE_SCANCODE_KP_3 = 91,
    SAUCE_SCANCODE_KP_4 = 92,
    SAUCE_SCANCODE_KP_5 = 93,
    SAUCE_SCANCODE_KP_6 = 94,
    SAUCE_SCANCODE_KP_7 = 95,
    SAUCE_SCANCODE_KP_8 = 96,
    SAUCE_SCANCODE_KP_9 = 97,
    SAUCE_SCANCODE_KP_0 = 98,
    SAUCE_SCANCODE_KP_PERIOD = 99,

    SAUCE_SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    SAUCE_SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
    SAUCE_SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    SAUCE_SCANCODE_KP_EQUALS = 103,
    SAUCE_SCANCODE_F13 = 104,
    SAUCE_SCANCODE_F14 = 105,
    SAUCE_SCANCODE_F15 = 106,
    SAUCE_SCANCODE_F16 = 107,
    SAUCE_SCANCODE_F17 = 108,
    SAUCE_SCANCODE_F18 = 109,
    SAUCE_SCANCODE_F19 = 110,
    SAUCE_SCANCODE_F20 = 111,
    SAUCE_SCANCODE_F21 = 112,
    SAUCE_SCANCODE_F22 = 113,
    SAUCE_SCANCODE_F23 = 114,
    SAUCE_SCANCODE_F24 = 115,
    SAUCE_SCANCODE_EXECUTE = 116,
    SAUCE_SCANCODE_HELP = 117,
    SAUCE_SCANCODE_MENU = 118,
    SAUCE_SCANCODE_SELECT = 119,
    SAUCE_SCANCODE_STOP = 120,
    SAUCE_SCANCODE_AGAIN = 121,   /**< redo */
    SAUCE_SCANCODE_UNDO = 122,
    SAUCE_SCANCODE_CUT = 123,
    SAUCE_SCANCODE_COPY = 124,
    SAUCE_SCANCODE_PASTE = 125,
    SAUCE_SCANCODE_FIND = 126,
    SAUCE_SCANCODE_MUTE = 127,
    SAUCE_SCANCODE_VOLUMEUP = 128,
    SAUCE_SCANCODE_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     SAUCE_SCANCODE_LOCKINGCAPSLOCK = 130,  */
/*     SAUCE_SCANCODE_LOCKINGNUMLOCK = 131, */
/*     SAUCE_SCANCODE_LOCKINGSCROLLLOCK = 132, */
    SAUCE_SCANCODE_KP_COMMA = 133,
    SAUCE_SCANCODE_KP_EQUALSAS400 = 134,

    SAUCE_SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    SAUCE_SCANCODE_INTERNATIONAL2 = 136,
    SAUCE_SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
    SAUCE_SCANCODE_INTERNATIONAL4 = 138,
    SAUCE_SCANCODE_INTERNATIONAL5 = 139,
    SAUCE_SCANCODE_INTERNATIONAL6 = 140,
    SAUCE_SCANCODE_INTERNATIONAL7 = 141,
    SAUCE_SCANCODE_INTERNATIONAL8 = 142,
    SAUCE_SCANCODE_INTERNATIONAL9 = 143,
    SAUCE_SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
    SAUCE_SCANCODE_LANG2 = 145, /**< Hanja conversion */
    SAUCE_SCANCODE_LANG3 = 146, /**< Katakana */
    SAUCE_SCANCODE_LANG4 = 147, /**< Hiragana */
    SAUCE_SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
    SAUCE_SCANCODE_LANG6 = 149, /**< reserved */
    SAUCE_SCANCODE_LANG7 = 150, /**< reserved */
    SAUCE_SCANCODE_LANG8 = 151, /**< reserved */
    SAUCE_SCANCODE_LANG9 = 152, /**< reserved */

    SAUCE_SCANCODE_ALTERASE = 153, /**< Erase-Eaze */
    SAUCE_SCANCODE_SYSREQ = 154,
    SAUCE_SCANCODE_CANCEL = 155,
    SAUCE_SCANCODE_CLEAR = 156,
    SAUCE_SCANCODE_PRIOR = 157,
    SAUCE_SCANCODE_RETURN2 = 158,
    SAUCE_SCANCODE_SEPARATOR = 159,
    SAUCE_SCANCODE_OUT = 160,
    SAUCE_SCANCODE_OPER = 161,
    SAUCE_SCANCODE_CLEARAGAIN = 162,
    SAUCE_SCANCODE_CRSEL = 163,
    SAUCE_SCANCODE_EXSEL = 164,

    SAUCE_SCANCODE_KP_00 = 176,
    SAUCE_SCANCODE_KP_000 = 177,
    SAUCE_SCANCODE_THOUSANDSSEPARATOR = 178,
    SAUCE_SCANCODE_DECIMALSEPARATOR = 179,
    SAUCE_SCANCODE_CURRENCYUNIT = 180,
    SAUCE_SCANCODE_CURRENCYSUBUNIT = 181,
    SAUCE_SCANCODE_KP_LEFTPAREN = 182,
    SAUCE_SCANCODE_KP_RIGHTPAREN = 183,
    SAUCE_SCANCODE_KP_LEFTBRACE = 184,
    SAUCE_SCANCODE_KP_RIGHTBRACE = 185,
    SAUCE_SCANCODE_KP_TAB = 186,
    SAUCE_SCANCODE_KP_BACKSPACE = 187,
    SAUCE_SCANCODE_KP_A = 188,
    SAUCE_SCANCODE_KP_B = 189,
    SAUCE_SCANCODE_KP_C = 190,
    SAUCE_SCANCODE_KP_D = 191,
    SAUCE_SCANCODE_KP_E = 192,
    SAUCE_SCANCODE_KP_F = 193,
    SAUCE_SCANCODE_KP_XOR = 194,
    SAUCE_SCANCODE_KP_POWER = 195,
    SAUCE_SCANCODE_KP_PERCENT = 196,
    SAUCE_SCANCODE_KP_LESS = 197,
    SAUCE_SCANCODE_KP_GREATER = 198,
    SAUCE_SCANCODE_KP_AMPERSAND = 199,
    SAUCE_SCANCODE_KP_DBLAMPERSAND = 200,
    SAUCE_SCANCODE_KP_VERTICALBAR = 201,
    SAUCE_SCANCODE_KP_DBLVERTICALBAR = 202,
    SAUCE_SCANCODE_KP_COLON = 203,
    SAUCE_SCANCODE_KP_HASH = 204,
    SAUCE_SCANCODE_KP_SPACE = 205,
    SAUCE_SCANCODE_KP_AT = 206,
    SAUCE_SCANCODE_KP_EXCLAM = 207,
    SAUCE_SCANCODE_KP_MEMSTORE = 208,
    SAUCE_SCANCODE_KP_MEMRECALL = 209,
    SAUCE_SCANCODE_KP_MEMCLEAR = 210,
    SAUCE_SCANCODE_KP_MEMADD = 211,
    SAUCE_SCANCODE_KP_MEMSUBTRACT = 212,
    SAUCE_SCANCODE_KP_MEMMULTIPLY = 213,
    SAUCE_SCANCODE_KP_MEMDIVIDE = 214,
    SAUCE_SCANCODE_KP_PLUSMINUS = 215,
    SAUCE_SCANCODE_KP_CLEAR = 216,
    SAUCE_SCANCODE_KP_CLEARENTRY = 217,
    SAUCE_SCANCODE_KP_BINARY = 218,
    SAUCE_SCANCODE_KP_OCTAL = 219,
    SAUCE_SCANCODE_KP_DECIMAL = 220,
    SAUCE_SCANCODE_KP_HEXADECIMAL = 221,

    SAUCE_SCANCODE_LCTRL = 224,
    SAUCE_SCANCODE_LSHIFT = 225,
    SAUCE_SCANCODE_LALT = 226, /**< alt, option */
    SAUCE_SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
    SAUCE_SCANCODE_RCTRL = 228,
    SAUCE_SCANCODE_RSHIFT = 229,
    SAUCE_SCANCODE_RALT = 230, /**< alt gr, option */
    SAUCE_SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

    SAUCE_SCANCODE_MODE = 257,    /**< I'm not sure if this is really not covered
                                 *   by any of the above, but since there's a
                                 *   special KMOD_MODE for it I'm adding it here
                                 */

    /* @} *//* Usage page 0x07 */

    /**
     *  \name Usage page 0x0C
     *
     *  These values are mapped from usage page 0x0C (USB consumer page).
     */
    /* @{ */

    SAUCE_SCANCODE_AUDIONEXT = 258,
    SAUCE_SCANCODE_AUDIOPREV = 259,
    SAUCE_SCANCODE_AUDIOSTOP = 260,
    SAUCE_SCANCODE_AUDIOPLAY = 261,
    SAUCE_SCANCODE_AUDIOMUTE = 262,
    SAUCE_SCANCODE_MEDIASELECT = 263,
    SAUCE_SCANCODE_WWW = 264,
    SAUCE_SCANCODE_MAIL = 265,
    SAUCE_SCANCODE_CALCULATOR = 266,
    SAUCE_SCANCODE_COMPUTER = 267,
    SAUCE_SCANCODE_AC_SEARCH = 268,
    SAUCE_SCANCODE_AC_HOME = 269,
    SAUCE_SCANCODE_AC_BACK = 270,
    SAUCE_SCANCODE_AC_FORWARD = 271,
    SAUCE_SCANCODE_AC_STOP = 272,
    SAUCE_SCANCODE_AC_REFRESH = 273,
    SAUCE_SCANCODE_AC_BOOKMARKS = 274,

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    SAUCE_SCANCODE_BRIGHTNESSDOWN = 275,
    SAUCE_SCANCODE_BRIGHTNESSUP = 276,
    SAUCE_SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    SAUCE_SCANCODE_KBDILLUMTOGGLE = 278,
    SAUCE_SCANCODE_KBDILLUMDOWN = 279,
    SAUCE_SCANCODE_KBDILLUMUP = 280,
    SAUCE_SCANCODE_EJECT = 281,
    SAUCE_SCANCODE_SLEEP = 282,

    SAUCE_SCANCODE_APP1 = 283,
    SAUCE_SCANCODE_APP2 = 284,

    /* @} *//* Walther keys */

    /* Add any other keys here. */

    SAUCE_NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
};

END_SAUCE_NAMESPACE

#endif /* SAUCE_SCANCODES_H */
