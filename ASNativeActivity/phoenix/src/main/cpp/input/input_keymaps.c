/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2010-2014 - Hans-Kristian Arntzen
 *  Copyright (C) 2011-2017 - Daniel De Matteis
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <compat/strl.h>
#include <retro_assert.h>
#include <retro_miscellaneous.h>

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#ifdef ANDROID
#include <android/keycodes.h>
#include "drivers_keyboard/keyboard_event_android.h"
#endif

#ifdef DJGPP
#include "drivers_keyboard/keyboard_event_dos.h"
#endif

#ifdef __QNX__
#include <sys/keycodes.h>
#endif

#if defined(HAVE_SDL) || defined(HAVE_SDL2)
#include "SDL.h"
#endif

#ifdef __linux__
#include <linux/input.h>
#include <linux/kd.h>
#endif

#include "input_keymaps.h"

const struct input_key_map input_config_key_map[] = {
   { "left", RETROK_LEFT },
   { "right", RETROK_RIGHT },
   { "up", RETROK_UP },
   { "down", RETROK_DOWN },
   { "enter", RETROK_RETURN },
   { "kp_enter", RETROK_KP_ENTER },
   { "tab", RETROK_TAB },
   { "insert", RETROK_INSERT },
   { "del", RETROK_DELETE },
   { "end", RETROK_END },
   { "home", RETROK_HOME },
   { "rshift", RETROK_RSHIFT },
   { "shift", RETROK_LSHIFT },
   { "ctrl", RETROK_LCTRL },
   { "alt", RETROK_LALT },
   { "space", RETROK_SPACE },
   { "escape", RETROK_ESCAPE },
   { "add", RETROK_KP_PLUS },
   { "subtract", RETROK_KP_MINUS },
   { "kp_plus", RETROK_KP_PLUS },
   { "kp_minus", RETROK_KP_MINUS },
   { "f1", RETROK_F1 },
   { "f2", RETROK_F2 },
   { "f3", RETROK_F3 },
   { "f4", RETROK_F4 },
   { "f5", RETROK_F5 },
   { "f6", RETROK_F6 },
   { "f7", RETROK_F7 },
   { "f8", RETROK_F8 },
   { "f9", RETROK_F9 },
   { "f10", RETROK_F10 },
   { "f11", RETROK_F11 },
   { "f12", RETROK_F12 },
   { "num0", RETROK_0 },
   { "num1", RETROK_1 },
   { "num2", RETROK_2 },
   { "num3", RETROK_3 },
   { "num4", RETROK_4 },
   { "num5", RETROK_5 },
   { "num6", RETROK_6 },
   { "num7", RETROK_7 },
   { "num8", RETROK_8 },
   { "num9", RETROK_9 },
   { "pageup", RETROK_PAGEUP },
   { "pagedown", RETROK_PAGEDOWN },
   { "keypad0", RETROK_KP0 },
   { "keypad1", RETROK_KP1 },
   { "keypad2", RETROK_KP2 },
   { "keypad3", RETROK_KP3 },
   { "keypad4", RETROK_KP4 },
   { "keypad5", RETROK_KP5 },
   { "keypad6", RETROK_KP6 },
   { "keypad7", RETROK_KP7 },
   { "keypad8", RETROK_KP8 },
   { "keypad9", RETROK_KP9 },
   { "period", RETROK_PERIOD },
   { "capslock", RETROK_CAPSLOCK },
   { "numlock", RETROK_NUMLOCK },
   { "backspace", RETROK_BACKSPACE },
   { "multiply", RETROK_KP_MULTIPLY },
   { "divide", RETROK_KP_DIVIDE },
   { "print_screen", RETROK_PRINT },
   { "scroll_lock", RETROK_SCROLLOCK },
   { "tilde", RETROK_BACKQUOTE },
   { "backquote", RETROK_BACKQUOTE },
   { "pause", RETROK_PAUSE },

   /* Keys that weren't mappable before */
   { "quote", RETROK_QUOTE },
   { "comma", RETROK_COMMA },
   { "minus", RETROK_MINUS },
   { "slash", RETROK_SLASH },
   { "semicolon", RETROK_SEMICOLON },
   { "equals", RETROK_EQUALS },
   { "leftbracket", RETROK_LEFTBRACKET },
   { "backslash", RETROK_BACKSLASH },
   { "rightbracket", RETROK_RIGHTBRACKET },
   { "kp_period", RETROK_KP_PERIOD },
   { "kp_equals", RETROK_KP_EQUALS },
   { "rctrl", RETROK_RCTRL },
   { "ralt", RETROK_RALT },

   /* Keys not referenced in any keyboard mapping 
    * (except perhaps rarch_key_map_apple_hid) */
   { "caret", RETROK_CARET },
   { "underscore", RETROK_UNDERSCORE },
   { "exclaim", RETROK_EXCLAIM },
   { "quotedbl", RETROK_QUOTEDBL },
   { "hash", RETROK_HASH },
   { "dollar", RETROK_DOLLAR },
   { "ampersand", RETROK_AMPERSAND },
   { "leftparen", RETROK_LEFTPAREN },
   { "rightparen", RETROK_RIGHTPAREN },
   { "asterisk", RETROK_ASTERISK },
   { "plus", RETROK_PLUS },
   { "colon", RETROK_COLON },
   { "less", RETROK_LESS },
   { "greater", RETROK_GREATER },
   { "question", RETROK_QUESTION },
   { "at", RETROK_AT },

   { "f13", RETROK_F13 },
   { "f14", RETROK_F14 },
   { "f15", RETROK_F15 },

   { "rmeta", RETROK_RMETA },
   { "lmeta", RETROK_LMETA },
   { "lsuper", RETROK_LSUPER },
   { "rsuper", RETROK_RSUPER },
   { "mode", RETROK_MODE },
   { "compose", RETROK_COMPOSE },

   { "help", RETROK_HELP },
   { "sysreq", RETROK_SYSREQ },
   { "break", RETROK_BREAK },
   { "menu", RETROK_MENU },
   { "power", RETROK_POWER },
   { "euro", RETROK_EURO },
   { "undo", RETROK_UNDO },
   { "clear", RETROK_CLEAR },

   { "nul", RETROK_UNKNOWN },
   { NULL, RETROK_UNKNOWN },
};

#ifdef __linux__
const struct rarch_key_map rarch_key_map_linux[] = {
   { KEY_BACKSPACE, RETROK_BACKSPACE },
   { KEY_TAB, RETROK_TAB },
   { KEY_CLEAR, RETROK_CLEAR },
   { KEY_ENTER, RETROK_RETURN },
   { KEY_PAUSE, RETROK_PAUSE },
   { KEY_ESC, RETROK_ESCAPE },
   { KEY_SPACE, RETROK_SPACE },
   /* { ?, RETROK_EXCLAIM }, */
   /* { ?, RETROK_QUOTEDBL }, */
   /* { ?, RETROK_HASH }, */
#ifndef ANDROID
   { KEY_DOLLAR, RETROK_DOLLAR },
#endif
   /* { ?, RETROK_AMPERSAND }, */
   { KEY_APOSTROPHE, RETROK_QUOTE },
   { KEY_KPLEFTPAREN, RETROK_LEFTPAREN },
   { KEY_KPRIGHTPAREN, RETROK_RIGHTPAREN },
   { KEY_KPASTERISK, RETROK_ASTERISK },
   { KEY_KPPLUS, RETROK_PLUS },
   { KEY_COMMA, RETROK_COMMA },
   { KEY_MINUS, RETROK_MINUS },
   { KEY_DOT, RETROK_PERIOD },
   { KEY_SLASH, RETROK_SLASH },
   { KEY_0, RETROK_0 },
   { KEY_1, RETROK_1 },
   { KEY_2, RETROK_2 },
   { KEY_3, RETROK_3 },
   { KEY_4, RETROK_4 },
   { KEY_5, RETROK_5 },
   { KEY_6, RETROK_6 },
   { KEY_7, RETROK_7 },
   { KEY_8, RETROK_8 },
   { KEY_9, RETROK_9 },
   /* { KEY_COLON, RETROK_COLON }, */
   { KEY_SEMICOLON, RETROK_SEMICOLON },
   /* { KEY_LESS, RETROK_LESS }, */
   { KEY_EQUAL, RETROK_EQUALS },
   /* { KEY_GREATER, RETROK_GREATER }, */
   { KEY_QUESTION, RETROK_QUESTION },
   /* { KEY_AT, RETROK_AT }, */
   { KEY_LEFTBRACE, RETROK_LEFTBRACKET },
   { KEY_BACKSLASH, RETROK_BACKSLASH },
   { KEY_RIGHTBRACE, RETROK_RIGHTBRACKET },
   /* { ?, RETROK_CARET }, */
   /* { ?, RETROK_UNDERSCORE }, */
   { KEY_GRAVE, RETROK_BACKQUOTE },
   { KEY_A, RETROK_a },
   { KEY_B, RETROK_b },
   { KEY_C, RETROK_c },
   { KEY_D, RETROK_d },
   { KEY_E, RETROK_e },
   { KEY_F, RETROK_f },
   { KEY_G, RETROK_g },
   { KEY_H, RETROK_h },
   { KEY_I, RETROK_i },
   { KEY_J, RETROK_j },
   { KEY_K, RETROK_k },
   { KEY_L, RETROK_l },
   { KEY_M, RETROK_m },
   { KEY_N, RETROK_n },
   { KEY_O, RETROK_o },
   { KEY_P, RETROK_p },
   { KEY_Q, RETROK_q },
   { KEY_R, RETROK_r },
   { KEY_S, RETROK_s },
   { KEY_T, RETROK_t },
   { KEY_U, RETROK_u },
   { KEY_V, RETROK_v },
   { KEY_W, RETROK_w },
   { KEY_X, RETROK_x },
   { KEY_Y, RETROK_y },
   { KEY_Z, RETROK_z },
   { KEY_DELETE, RETROK_DELETE },
   { KEY_KP0, RETROK_KP0 },
   { KEY_KP1, RETROK_KP1 },
   { KEY_KP2, RETROK_KP2 },
   { KEY_KP3, RETROK_KP3 },
   { KEY_KP4, RETROK_KP4 },
   { KEY_KP5, RETROK_KP5 },
   { KEY_KP6, RETROK_KP6 },
   { KEY_KP7, RETROK_KP7 },
   { KEY_KP8, RETROK_KP8 },
   { KEY_KP9, RETROK_KP9 },
   { KEY_KPDOT, RETROK_KP_PERIOD },
   { KEY_KPSLASH, RETROK_KP_DIVIDE },
   { KEY_KPASTERISK, RETROK_KP_MULTIPLY },
   { KEY_KPMINUS, RETROK_KP_MINUS },
   { KEY_KPPLUS, RETROK_KP_PLUS },
   { KEY_KPENTER, RETROK_KP_ENTER },
   { KEY_KPEQUAL, RETROK_KP_EQUALS },
   { KEY_UP, RETROK_UP },
   { KEY_DOWN, RETROK_DOWN },
   { KEY_RIGHT, RETROK_RIGHT },
   { KEY_LEFT, RETROK_LEFT },
   { KEY_INSERT, RETROK_INSERT },
   { KEY_HOME, RETROK_HOME },
   { KEY_END, RETROK_END },
   { KEY_PAGEUP, RETROK_PAGEUP },
   { KEY_PAGEDOWN, RETROK_PAGEDOWN },
   { KEY_F1, RETROK_F1 },
   { KEY_F2, RETROK_F2 },
   { KEY_F3, RETROK_F3 },
   { KEY_F4, RETROK_F4 },
   { KEY_F5, RETROK_F5 },
   { KEY_F6, RETROK_F6 },
   { KEY_F7, RETROK_F7 },
   { KEY_F8, RETROK_F8 },
   { KEY_F9, RETROK_F9 },
   { KEY_F10, RETROK_F10 },
   { KEY_F11, RETROK_F11 },
   { KEY_F12, RETROK_F12 },
   { KEY_F13, RETROK_F13 },
   { KEY_F14, RETROK_F14 },
   { KEY_F15, RETROK_F15 },
   { KEY_NUMLOCK, RETROK_NUMLOCK },
   { KEY_CAPSLOCK, RETROK_CAPSLOCK },
   { KEY_SCROLLLOCK, RETROK_SCROLLOCK },
   { KEY_RIGHTSHIFT, RETROK_RSHIFT },
   { KEY_LEFTSHIFT, RETROK_LSHIFT },
   { KEY_RIGHTCTRL, RETROK_RCTRL },
   { KEY_LEFTCTRL, RETROK_LCTRL },
   { KEY_RIGHTALT, RETROK_RALT },
   { KEY_LEFTALT, RETROK_LALT },
   /* { ?, RETROK_RMETA }, */
   /* { ?, RETROK_LMETA }, */
   { KEY_LEFTMETA, RETROK_LSUPER },
   { KEY_RIGHTMETA, RETROK_RSUPER },
   { KEY_MODE, RETROK_MODE },
   { KEY_COMPOSE, RETROK_COMPOSE },
   { KEY_HELP, RETROK_HELP },
   { KEY_PRINT, RETROK_PRINT },
   { KEY_SYSRQ, RETROK_SYSREQ },
   { KEY_BREAK, RETROK_BREAK },
   { KEY_MENU, RETROK_MENU },
   { KEY_POWER, RETROK_POWER },
#ifndef ANDROID
   { KEY_EURO, RETROK_EURO },
#endif
   { KEY_UNDO, RETROK_UNDO },
   { 0, RETROK_UNKNOWN },
};
#endif

#ifdef ANDROID
const struct rarch_key_map rarch_key_map_android[] = {
   { AKEYCODE_DEL, RETROK_BACKSPACE },
   { AKEYCODE_TAB, RETROK_TAB },
   { AKEYCODE_CLEAR, RETROK_CLEAR },
   { AKEYCODE_ENTER, RETROK_RETURN },
   { AKEYCODE_BREAK, RETROK_PAUSE },
   { AKEYCODE_ESCAPE, RETROK_ESCAPE },
   { AKEYCODE_SPACE, RETROK_SPACE },
   { AKEYCODE_APOSTROPHE, RETROK_QUOTE },
   { AKEYCODE_NUMPAD_LEFT_PAREN, RETROK_LEFTPAREN },
   { AKEYCODE_NUMPAD_RIGHT_PAREN, RETROK_RIGHTPAREN },
   { AKEYCODE_NUMPAD_MULTIPLY, RETROK_ASTERISK },
   { AKEYCODE_NUMPAD_ADD, RETROK_PLUS },
   { AKEYCODE_COMMA, RETROK_COMMA },
   { AKEYCODE_MINUS, RETROK_MINUS },
   { AKEYCODE_PERIOD, RETROK_PERIOD },
   { AKEYCODE_SLASH, RETROK_SLASH },
   { AKEYCODE_0, RETROK_0 },
   { AKEYCODE_1, RETROK_1 },
   { AKEYCODE_2, RETROK_2 },
   { AKEYCODE_3, RETROK_3 },
   { AKEYCODE_4, RETROK_4 },
   { AKEYCODE_5, RETROK_5 },
   { AKEYCODE_6, RETROK_6 },
   { AKEYCODE_7, RETROK_7 },
   { AKEYCODE_8, RETROK_8 },
   { AKEYCODE_9, RETROK_9 },
   { AKEYCODE_SEMICOLON, RETROK_SEMICOLON },
   { AKEYCODE_EQUALS, RETROK_EQUALS },
   { AKEYCODE_LEFT_BRACKET, RETROK_LEFTBRACKET },
   { AKEYCODE_BACKSLASH, RETROK_BACKSLASH },
   { AKEYCODE_RIGHT_BRACKET, RETROK_RIGHTBRACKET },
   { AKEYCODE_GRAVE, RETROK_BACKQUOTE },
   { AKEYCODE_A, RETROK_a },
   { AKEYCODE_B, RETROK_b },
   { AKEYCODE_C, RETROK_c },
   { AKEYCODE_D, RETROK_d },
   { AKEYCODE_E, RETROK_e },
   { AKEYCODE_F, RETROK_f },
   { AKEYCODE_G, RETROK_g },
   { AKEYCODE_H, RETROK_h },
   { AKEYCODE_I, RETROK_i },
   { AKEYCODE_J, RETROK_j },
   { AKEYCODE_K, RETROK_k },
   { AKEYCODE_L, RETROK_l },
   { AKEYCODE_M, RETROK_m },
   { AKEYCODE_N, RETROK_n },
   { AKEYCODE_O, RETROK_o },
   { AKEYCODE_P, RETROK_p },
   { AKEYCODE_Q, RETROK_q },
   { AKEYCODE_R, RETROK_r },
   { AKEYCODE_S, RETROK_s },
   { AKEYCODE_T, RETROK_t },
   { AKEYCODE_U, RETROK_u },
   { AKEYCODE_V, RETROK_v },
   { AKEYCODE_W, RETROK_w },
   { AKEYCODE_X, RETROK_x },
   { AKEYCODE_Y, RETROK_y },
   { AKEYCODE_Z, RETROK_z },
   { AKEYCODE_DEL, RETROK_DELETE },
   { AKEYCODE_NUMPAD_0, RETROK_KP0 },
   { AKEYCODE_NUMPAD_1, RETROK_KP1 },
   { AKEYCODE_NUMPAD_2, RETROK_KP2 },
   { AKEYCODE_NUMPAD_3, RETROK_KP3 },
   { AKEYCODE_NUMPAD_4, RETROK_KP4 },
   { AKEYCODE_NUMPAD_5, RETROK_KP5 },
   { AKEYCODE_NUMPAD_6, RETROK_KP6 },
   { AKEYCODE_NUMPAD_7, RETROK_KP7 },
   { AKEYCODE_NUMPAD_8, RETROK_KP8 },
   { AKEYCODE_NUMPAD_9, RETROK_KP9 },
   { AKEYCODE_NUMPAD_DOT, RETROK_KP_PERIOD },
   { AKEYCODE_NUMPAD_DIVIDE, RETROK_KP_DIVIDE },
   { AKEYCODE_NUMPAD_MULTIPLY, RETROK_KP_MULTIPLY },
   { AKEYCODE_NUMPAD_SUBTRACT, RETROK_KP_MINUS },
   { AKEYCODE_NUMPAD_ADD, RETROK_KP_PLUS },
   { AKEYCODE_NUMPAD_ENTER, RETROK_KP_ENTER },
   { AKEYCODE_NUMPAD_EQUALS, RETROK_KP_EQUALS },
   { AKEYCODE_DPAD_UP, RETROK_UP },
   { AKEYCODE_DPAD_DOWN, RETROK_DOWN },
   { AKEYCODE_DPAD_RIGHT, RETROK_RIGHT },
   { AKEYCODE_DPAD_LEFT, RETROK_LEFT },
   { AKEYCODE_INSERT, RETROK_INSERT },
   { AKEYCODE_MOVE_HOME, RETROK_HOME },
   { AKEYCODE_MOVE_END, RETROK_END },
   { AKEYCODE_PAGE_UP, RETROK_PAGEUP },
   { AKEYCODE_PAGE_DOWN, RETROK_PAGEDOWN },
   { AKEYCODE_F1, RETROK_F1 },
   { AKEYCODE_F2, RETROK_F2 },
   { AKEYCODE_F3, RETROK_F3 },
   { AKEYCODE_F4, RETROK_F4 },
   { AKEYCODE_F5, RETROK_F5 },
   { AKEYCODE_F6, RETROK_F6 },
   { AKEYCODE_F7, RETROK_F7 },
   { AKEYCODE_F8, RETROK_F8 },
   { AKEYCODE_F9, RETROK_F9 },
   { AKEYCODE_F10, RETROK_F10 },
   { AKEYCODE_F11, RETROK_F11 },
   { AKEYCODE_F12, RETROK_F12 },
   { AKEYCODE_NUM_LOCK, RETROK_NUMLOCK },
   { AKEYCODE_CAPS_LOCK, RETROK_CAPSLOCK },
   { AKEYCODE_SCROLL_LOCK, RETROK_SCROLLOCK },
   { AKEYCODE_SHIFT_LEFT, RETROK_RSHIFT },
   { AKEYCODE_SHIFT_RIGHT, RETROK_LSHIFT },
   { AKEYCODE_CTRL_RIGHT, RETROK_RCTRL },
   { AKEYCODE_CTRL_LEFT, RETROK_LCTRL },
   { AKEYCODE_ALT_RIGHT, RETROK_RALT },
   { AKEYCODE_ALT_LEFT, RETROK_LALT },
   { 0, RETROK_UNKNOWN },
};
#endif

enum retro_key rarch_keysym_lut[RETROK_LAST];

/**
 * input_keymaps_init_keyboard_lut:
 * @map                   : Keyboard map.
 *
 * Initializes and sets the keyboard layout to a keyboard map (@map).
 **/
void input_keymaps_init_keyboard_lut(const struct rarch_key_map *map)
{
   memset(rarch_keysym_lut, 0, sizeof(rarch_keysym_lut));

   for (; map->rk != RETROK_UNKNOWN; map++)
      rarch_keysym_lut[map->rk] = (enum retro_key)map->sym;
}

/**
 * input_keymaps_translate_keysym_to_rk:
 * @sym                   : Key symbol.
 *
 * Translates a key symbol from the keyboard layout table
 * to an associated retro key identifier.
 *
 * Returns: Retro key identifier.
 **/
enum retro_key input_keymaps_translate_keysym_to_rk(unsigned sym)
{
   unsigned i;

   for (i = 0; i < ARRAY_SIZE(rarch_keysym_lut); i++)
   {
      if (rarch_keysym_lut[i] != sym)
         continue;

      return (enum retro_key)i;
   }

   return RETROK_UNKNOWN;
}

/**
 * input_keymaps_translate_rk_to_str:
 * @key                   : Retro key identifier.
 * @buf                   : Buffer.
 * @size                  : Size of @buf.
 *
 * Translates a retro key identifier to a human-readable 
 * identifier string.
 **/
void input_keymaps_translate_rk_to_str(enum retro_key key, char *buf, size_t size)
{
   unsigned i;

   retro_assert(size >= 2);
   *buf = '\0';

   if (key >= RETROK_a && key <= RETROK_z)
   {
      buf[0] = (key - RETROK_a) + 'a';
      buf[1] = '\0';
      return;
   }

   for (i = 0; input_config_key_map[i].str; i++)
   {
      if (input_config_key_map[i].key != key)
         continue;

      strlcpy(buf, input_config_key_map[i].str, size);
      break;
   }
}
