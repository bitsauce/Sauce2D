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

#ifndef CGF_SCANCODES_H
#define CGF_SCANCODES_H

#include <CGF/Config.h>

BEGIN_CGF_NAMESPACE

/**
 *  \brief The CGF keyboard scancode representation.
 *
 *  Values of this type are used to represent keyboard keys, among other places
 *  in the \link CGF_Keysym::scancode key.keysym.scancode \endlink field of the
 *  CGF_Event structure.
 *
 *  The values in this enumeration are based on the USB usage page standard:
 *  http://www.usb.org/developers/devclass_docs/Hut1_12v2.pdf
 */
enum Scancode
{
    CGF_SCANCODE_UNKNOWN = 0,

    /**
     *  \name Usage page 0x07
     *
     *  These values are from usage page 0x07 (USB keyboard page).
     */
    /* @{ */

    CGF_SCANCODE_A = 4,
    CGF_SCANCODE_B = 5,
    CGF_SCANCODE_C = 6,
    CGF_SCANCODE_D = 7,
    CGF_SCANCODE_E = 8,
    CGF_SCANCODE_F = 9,
    CGF_SCANCODE_G = 10,
    CGF_SCANCODE_H = 11,
    CGF_SCANCODE_I = 12,
    CGF_SCANCODE_J = 13,
    CGF_SCANCODE_K = 14,
    CGF_SCANCODE_L = 15,
    CGF_SCANCODE_M = 16,
    CGF_SCANCODE_N = 17,
    CGF_SCANCODE_O = 18,
    CGF_SCANCODE_P = 19,
    CGF_SCANCODE_Q = 20,
    CGF_SCANCODE_R = 21,
    CGF_SCANCODE_S = 22,
    CGF_SCANCODE_T = 23,
    CGF_SCANCODE_U = 24,
    CGF_SCANCODE_V = 25,
    CGF_SCANCODE_W = 26,
    CGF_SCANCODE_X = 27,
    CGF_SCANCODE_Y = 28,
    CGF_SCANCODE_Z = 29,

    CGF_SCANCODE_1 = 30,
    CGF_SCANCODE_2 = 31,
    CGF_SCANCODE_3 = 32,
    CGF_SCANCODE_4 = 33,
    CGF_SCANCODE_5 = 34,
    CGF_SCANCODE_6 = 35,
    CGF_SCANCODE_7 = 36,
    CGF_SCANCODE_8 = 37,
    CGF_SCANCODE_9 = 38,
    CGF_SCANCODE_0 = 39,

    CGF_SCANCODE_RETURN = 40,
    CGF_SCANCODE_ESCAPE = 41,
    CGF_SCANCODE_BACKSPACE = 42,
    CGF_SCANCODE_TAB = 43,
    CGF_SCANCODE_SPACE = 44,

    CGF_SCANCODE_MINUS = 45,
    CGF_SCANCODE_EQUALS = 46,
    CGF_SCANCODE_LEFTBRACKET = 47,
    CGF_SCANCODE_RIGHTBRACKET = 48,
    CGF_SCANCODE_BACKSLASH = 49, /**< Located at the lower left of the return
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
    CGF_SCANCODE_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
                                  *   instead of 49 for the same key, but all
                                  *   OSes I've seen treat the two codes
                                  *   identically. So, as an implementor, unless
                                  *   your keyboard generates both of those
                                  *   codes and your OS treats them differently,
                                  *   you should generate CGF_SCANCODE_BACKSLASH
                                  *   instead of this code. As a user, you
                                  *   should not rely on this code because SDL
                                  *   will never generate it with most (all?)
                                  *   keyboards.
                                  */
    CGF_SCANCODE_SEMICOLON = 51,
    CGF_SCANCODE_APOSTROPHE = 52,
    CGF_SCANCODE_GRAVE = 53, /**< Located in the top left corner (on both ANSI
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
    CGF_SCANCODE_COMMA = 54,
    CGF_SCANCODE_PERIOD = 55,
    CGF_SCANCODE_SLASH = 56,

    CGF_SCANCODE_CAPSLOCK = 57,

    CGF_SCANCODE_F1 = 58,
    CGF_SCANCODE_F2 = 59,
    CGF_SCANCODE_F3 = 60,
    CGF_SCANCODE_F4 = 61,
    CGF_SCANCODE_F5 = 62,
    CGF_SCANCODE_F6 = 63,
    CGF_SCANCODE_F7 = 64,
    CGF_SCANCODE_F8 = 65,
    CGF_SCANCODE_F9 = 66,
    CGF_SCANCODE_F10 = 67,
    CGF_SCANCODE_F11 = 68,
    CGF_SCANCODE_F12 = 69,

    CGF_SCANCODE_PRINTSCREEN = 70,
    CGF_SCANCODE_SCROLLLOCK = 71,
    CGF_SCANCODE_PAUSE = 72,
    CGF_SCANCODE_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
                                   does send code 73, not 117) */
    CGF_SCANCODE_HOME = 74,
    CGF_SCANCODE_PAGEUP = 75,
    CGF_SCANCODE_DELETE = 76,
    CGF_SCANCODE_END = 77,
    CGF_SCANCODE_PAGEDOWN = 78,
    CGF_SCANCODE_RIGHT = 79,
    CGF_SCANCODE_LEFT = 80,
    CGF_SCANCODE_DOWN = 81,
    CGF_SCANCODE_UP = 82,

    CGF_SCANCODE_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
                                     */
    CGF_SCANCODE_KP_DIVIDE = 84,
    CGF_SCANCODE_KP_MULTIPLY = 85,
    CGF_SCANCODE_KP_MINUS = 86,
    CGF_SCANCODE_KP_PLUS = 87,
    CGF_SCANCODE_KP_ENTER = 88,
    CGF_SCANCODE_KP_1 = 89,
    CGF_SCANCODE_KP_2 = 90,
    CGF_SCANCODE_KP_3 = 91,
    CGF_SCANCODE_KP_4 = 92,
    CGF_SCANCODE_KP_5 = 93,
    CGF_SCANCODE_KP_6 = 94,
    CGF_SCANCODE_KP_7 = 95,
    CGF_SCANCODE_KP_8 = 96,
    CGF_SCANCODE_KP_9 = 97,
    CGF_SCANCODE_KP_0 = 98,
    CGF_SCANCODE_KP_PERIOD = 99,

    CGF_SCANCODE_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
                                        *   keyboards have over ANSI ones,
                                        *   located between left shift and Y.
                                        *   Produces GRAVE ACCENT and TILDE in a
                                        *   US or UK Mac layout, REVERSE SOLIDUS
                                        *   (backslash) and VERTICAL LINE in a
                                        *   US or UK Windows layout, and
                                        *   LESS-THAN SIGN and GREATER-THAN SIGN
                                        *   in a Swiss German, German, or French
                                        *   layout. */
    CGF_SCANCODE_APPLICATION = 101, /**< windows contextual menu, compose */
    CGF_SCANCODE_POWER = 102, /**< The USB document says this is a status flag,
                               *   not a physical key - but some Mac keyboards
                               *   do have a power key. */
    CGF_SCANCODE_KP_EQUALS = 103,
    CGF_SCANCODE_F13 = 104,
    CGF_SCANCODE_F14 = 105,
    CGF_SCANCODE_F15 = 106,
    CGF_SCANCODE_F16 = 107,
    CGF_SCANCODE_F17 = 108,
    CGF_SCANCODE_F18 = 109,
    CGF_SCANCODE_F19 = 110,
    CGF_SCANCODE_F20 = 111,
    CGF_SCANCODE_F21 = 112,
    CGF_SCANCODE_F22 = 113,
    CGF_SCANCODE_F23 = 114,
    CGF_SCANCODE_F24 = 115,
    CGF_SCANCODE_EXECUTE = 116,
    CGF_SCANCODE_HELP = 117,
    CGF_SCANCODE_MENU = 118,
    CGF_SCANCODE_SELECT = 119,
    CGF_SCANCODE_STOP = 120,
    CGF_SCANCODE_AGAIN = 121,   /**< redo */
    CGF_SCANCODE_UNDO = 122,
    CGF_SCANCODE_CUT = 123,
    CGF_SCANCODE_COPY = 124,
    CGF_SCANCODE_PASTE = 125,
    CGF_SCANCODE_FIND = 126,
    CGF_SCANCODE_MUTE = 127,
    CGF_SCANCODE_VOLUMEUP = 128,
    CGF_SCANCODE_VOLUMEDOWN = 129,
/* not sure whether there's a reason to enable these */
/*     CGF_SCANCODE_LOCKINGCAPSLOCK = 130,  */
/*     CGF_SCANCODE_LOCKINGNUMLOCK = 131, */
/*     CGF_SCANCODE_LOCKINGSCROLLLOCK = 132, */
    CGF_SCANCODE_KP_COMMA = 133,
    CGF_SCANCODE_KP_EQUALSAS400 = 134,

    CGF_SCANCODE_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
                                            footnotes in USB doc */
    CGF_SCANCODE_INTERNATIONAL2 = 136,
    CGF_SCANCODE_INTERNATIONAL3 = 137, /**< Yen */
    CGF_SCANCODE_INTERNATIONAL4 = 138,
    CGF_SCANCODE_INTERNATIONAL5 = 139,
    CGF_SCANCODE_INTERNATIONAL6 = 140,
    CGF_SCANCODE_INTERNATIONAL7 = 141,
    CGF_SCANCODE_INTERNATIONAL8 = 142,
    CGF_SCANCODE_INTERNATIONAL9 = 143,
    CGF_SCANCODE_LANG1 = 144, /**< Hangul/English toggle */
    CGF_SCANCODE_LANG2 = 145, /**< Hanja conversion */
    CGF_SCANCODE_LANG3 = 146, /**< Katakana */
    CGF_SCANCODE_LANG4 = 147, /**< Hiragana */
    CGF_SCANCODE_LANG5 = 148, /**< Zenkaku/Hankaku */
    CGF_SCANCODE_LANG6 = 149, /**< reserved */
    CGF_SCANCODE_LANG7 = 150, /**< reserved */
    CGF_SCANCODE_LANG8 = 151, /**< reserved */
    CGF_SCANCODE_LANG9 = 152, /**< reserved */

    CGF_SCANCODE_ALTERASE = 153, /**< Erase-Eaze */
    CGF_SCANCODE_SYSREQ = 154,
    CGF_SCANCODE_CANCEL = 155,
    CGF_SCANCODE_CLEAR = 156,
    CGF_SCANCODE_PRIOR = 157,
    CGF_SCANCODE_RETURN2 = 158,
    CGF_SCANCODE_SEPARATOR = 159,
    CGF_SCANCODE_OUT = 160,
    CGF_SCANCODE_OPER = 161,
    CGF_SCANCODE_CLEARAGAIN = 162,
    CGF_SCANCODE_CRSEL = 163,
    CGF_SCANCODE_EXSEL = 164,

    CGF_SCANCODE_KP_00 = 176,
    CGF_SCANCODE_KP_000 = 177,
    CGF_SCANCODE_THOUSANDSSEPARATOR = 178,
    CGF_SCANCODE_DECIMALSEPARATOR = 179,
    CGF_SCANCODE_CURRENCYUNIT = 180,
    CGF_SCANCODE_CURRENCYSUBUNIT = 181,
    CGF_SCANCODE_KP_LEFTPAREN = 182,
    CGF_SCANCODE_KP_RIGHTPAREN = 183,
    CGF_SCANCODE_KP_LEFTBRACE = 184,
    CGF_SCANCODE_KP_RIGHTBRACE = 185,
    CGF_SCANCODE_KP_TAB = 186,
    CGF_SCANCODE_KP_BACKSPACE = 187,
    CGF_SCANCODE_KP_A = 188,
    CGF_SCANCODE_KP_B = 189,
    CGF_SCANCODE_KP_C = 190,
    CGF_SCANCODE_KP_D = 191,
    CGF_SCANCODE_KP_E = 192,
    CGF_SCANCODE_KP_F = 193,
    CGF_SCANCODE_KP_XOR = 194,
    CGF_SCANCODE_KP_POWER = 195,
    CGF_SCANCODE_KP_PERCENT = 196,
    CGF_SCANCODE_KP_LESS = 197,
    CGF_SCANCODE_KP_GREATER = 198,
    CGF_SCANCODE_KP_AMPERSAND = 199,
    CGF_SCANCODE_KP_DBLAMPERSAND = 200,
    CGF_SCANCODE_KP_VERTICALBAR = 201,
    CGF_SCANCODE_KP_DBLVERTICALBAR = 202,
    CGF_SCANCODE_KP_COLON = 203,
    CGF_SCANCODE_KP_HASH = 204,
    CGF_SCANCODE_KP_SPACE = 205,
    CGF_SCANCODE_KP_AT = 206,
    CGF_SCANCODE_KP_EXCLAM = 207,
    CGF_SCANCODE_KP_MEMSTORE = 208,
    CGF_SCANCODE_KP_MEMRECALL = 209,
    CGF_SCANCODE_KP_MEMCLEAR = 210,
    CGF_SCANCODE_KP_MEMADD = 211,
    CGF_SCANCODE_KP_MEMSUBTRACT = 212,
    CGF_SCANCODE_KP_MEMMULTIPLY = 213,
    CGF_SCANCODE_KP_MEMDIVIDE = 214,
    CGF_SCANCODE_KP_PLUSMINUS = 215,
    CGF_SCANCODE_KP_CLEAR = 216,
    CGF_SCANCODE_KP_CLEARENTRY = 217,
    CGF_SCANCODE_KP_BINARY = 218,
    CGF_SCANCODE_KP_OCTAL = 219,
    CGF_SCANCODE_KP_DECIMAL = 220,
    CGF_SCANCODE_KP_HEXADECIMAL = 221,

    CGF_SCANCODE_LCTRL = 224,
    CGF_SCANCODE_LSHIFT = 225,
    CGF_SCANCODE_LALT = 226, /**< alt, option */
    CGF_SCANCODE_LGUI = 227, /**< windows, command (apple), meta */
    CGF_SCANCODE_RCTRL = 228,
    CGF_SCANCODE_RSHIFT = 229,
    CGF_SCANCODE_RALT = 230, /**< alt gr, option */
    CGF_SCANCODE_RGUI = 231, /**< windows, command (apple), meta */

    CGF_SCANCODE_MODE = 257,    /**< I'm not sure if this is really not covered
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

    CGF_SCANCODE_AUDIONEXT = 258,
    CGF_SCANCODE_AUDIOPREV = 259,
    CGF_SCANCODE_AUDIOSTOP = 260,
    CGF_SCANCODE_AUDIOPLAY = 261,
    CGF_SCANCODE_AUDIOMUTE = 262,
    CGF_SCANCODE_MEDIASELECT = 263,
    CGF_SCANCODE_WWW = 264,
    CGF_SCANCODE_MAIL = 265,
    CGF_SCANCODE_CALCULATOR = 266,
    CGF_SCANCODE_COMPUTER = 267,
    CGF_SCANCODE_AC_SEARCH = 268,
    CGF_SCANCODE_AC_HOME = 269,
    CGF_SCANCODE_AC_BACK = 270,
    CGF_SCANCODE_AC_FORWARD = 271,
    CGF_SCANCODE_AC_STOP = 272,
    CGF_SCANCODE_AC_REFRESH = 273,
    CGF_SCANCODE_AC_BOOKMARKS = 274,

    /* @} *//* Usage page 0x0C */

    /**
     *  \name Walther keys
     *
     *  These are values that Christian Walther added (for mac keyboard?).
     */
    /* @{ */

    CGF_SCANCODE_BRIGHTNESSDOWN = 275,
    CGF_SCANCODE_BRIGHTNESSUP = 276,
    CGF_SCANCODE_DISPLAYSWITCH = 277, /**< display mirroring/dual display
                                           switch, video mode switch */
    CGF_SCANCODE_KBDILLUMTOGGLE = 278,
    CGF_SCANCODE_KBDILLUMDOWN = 279,
    CGF_SCANCODE_KBDILLUMUP = 280,
    CGF_SCANCODE_EJECT = 281,
    CGF_SCANCODE_SLEEP = 282,

    CGF_SCANCODE_APP1 = 283,
    CGF_SCANCODE_APP2 = 284,

    /* @} *//* Walther keys */

    /* Add any other keys here. */

    CGF_NUM_SCANCODES = 512 /**< not a key, just marks the number of scancodes
                                 for array bounds */
};

END_CGF_NAMESPACE

#endif /* CGF_SCANCODES_H */