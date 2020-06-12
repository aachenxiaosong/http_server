/**************************************************************************
 * Copyright (C) 2018-2018  Unisound
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **************************************************************************
 *
 * Description : uni_keyboard.c
 * Author      : guozhenkun@unisound.com
 * Date        : 2018.12.10
 *
 **************************************************************************/
#include "uni_keyboard.h"
#include "uni_iot.h"
#include "uni_keyboard_hal.h"
#include "uni_log.h"
#include "uni_timer.h"
#if UNI_KEYBOARD_ENABLE

#define KEYBOARD_TAG      "keyboard"
#define KEY_INTERVAL      (400)
#define KEY_CLICK_SPAN    (1000)
#define KEY_PRESS_KEEP    (600)
#define KEY_LONG_PRESS    (KEY_PRESS_KEEP * 5)
#define KEYBOARD_KEY_DOWN (1)
#define KEYBOARD_KEY_UP   (0)

typedef struct {
  TimerHandle      key_press_timer;
  KeyboardCallback cb;
  uni_bool         is_active;
} Keyboard;

typedef struct {
  uni_bool  is_press_keep;
  long      press_time_ms;
  KBKeyCode key_code;
  uni_s32   pre_key_event;
} KeyEvent;

typedef struct {
  KBKeyCode keys;
  uni_u32   pressed_keys;
  long      trigger_time_ms;
} CompositeKeyEvent;

static Keyboard g_keyboard;
static KeyEvent g_key_event;
static CompositeKeyEvent g_comp_key_event;

static inline KBKeyCode _get_mapping_key(uni_s32 key_code) {
  KBKeyCode code = KEY_INVALID;
  switch (key_code) {
    case 113:
      code = KEY_MUTE;
      break;
    case 105:
      code = KEY_PAUSE;
      break;
    case 8:
      code = KEY_NEXT;
      break;
    case 10:
      code = KEY_PRE;
      break;
    case 114:
      code = KEY_VOLUME_DEC;
      break;
    case 115:
      code = KEY_VOLUME_INC;
      break;
    case 213:
      code = KEY_PHONE;
      break;
    case 4:
      code = KEY_WAKEUP;
      break;
    default:
      code = KEY_INVALID;
      break;
  }
  return code;
}

static void _set_keyboard_inactive() {
  uni_memset(&g_key_event, 0, sizeof(KeyEvent));
  g_keyboard.is_active = FALSE;
}

static void _reset_comp_keys() {
  g_comp_key_event.pressed_keys = 0;
}

static void _send_click_event(KBKeyCode key, KBClickType type) {
  KBEvent kb_event;
  kb_event.key  = key;
  kb_event.type = type;
  if (g_keyboard.cb) {
    g_keyboard.cb(&kb_event);
  }
}

static uni_s32 _key_long_press_process(void *arg) {
  long _now = uni_get_clock_time_ms();
  if (!g_key_event.is_press_keep &&
      KEY_LONG_PRESS <= (_now - g_key_event.press_time_ms)) {
    _send_click_event(g_key_event.key_code, CLICK_LONG_PRESS);
    g_key_event.is_press_keep = TRUE;
    LOGT(KEYBOARD_TAG, "long press detected, %ld", _now);
  } else if (g_key_event.is_press_keep) {
    _send_click_event(g_key_event.key_code, CLICK_PRESS_KEEP);
    LOGT(KEYBOARD_TAG, "long press keep detected");
  }
  return 0;
}

static uni_s32 _comp_key_long_press_process(void *arg) {
  _send_click_event(g_comp_key_event.keys, CLICK_COMP_PRESS);
  return 0;
}

static uni_s32 _check_twice_interval_time() {
  static long last_click_time = 0;
  LOGT(KEYBOARD_TAG, "%ld - %ld= %ld.\n", uni_get_clock_time_ms(), last_click_time, (uni_get_clock_time_ms() - last_click_time));
  if (uni_get_clock_time_ms() - last_click_time < 500) {
    LOGT(KEYBOARD_TAG, "interval time too short, skip key event");
    return -1;
  }
  last_click_time = uni_get_clock_time_ms();
  return 0;
}

static inline uni_s32 _key_click_process(long now) {
  if (g_key_event.is_press_keep) {
    LOGT(KEYBOARD_TAG, "long click release detected");
    _send_click_event(g_key_event.key_code, CLICK_PRESS_RELEASE);
  } else {
    if ((now - g_key_event.press_time_ms) < KEY_CLICK_SPAN) {
      if (g_key_event.key_code == KEY_VOLUME_INC ||
          g_key_event.key_code == KEY_VOLUME_DEC) {
        if (_check_twice_interval_time() == 0) {
          _send_click_event(g_key_event.key_code, CLICK_SINGLE);
        }
      } else {
        _send_click_event(g_key_event.key_code, CLICK_SINGLE);
      }
      LOGT(KEYBOARD_TAG, "signal click detected");
    }
  }
  return 0;
}

static void _key_process(uni_s32 event, KBKeyCode key, long cur_time_ms) {
  TimerStop(g_keyboard.key_press_timer);
  g_key_event.pre_key_event = event;
  if (KEYBOARD_KEY_DOWN == event) {
    g_keyboard.is_active       = TRUE;
    g_key_event.key_code       = key;
    g_key_event.press_time_ms  = cur_time_ms;
    if (key & g_comp_key_event.keys) {
      g_comp_key_event.pressed_keys |= key;
    }
    if (KEY_INVALID != g_comp_key_event.keys &&
        g_comp_key_event.keys == g_comp_key_event.pressed_keys) {
      g_keyboard.key_press_timer =
          TimerStart(g_comp_key_event.trigger_time_ms, TIMER_TYPE_ONESHOT,
                     _comp_key_long_press_process, NULL);
    } else {
      g_keyboard.key_press_timer = TimerStart(
          KEY_PRESS_KEEP, TIMER_TYPE_PERIODICAL, _key_long_press_process, NULL);
    }
    return;
  }
  if ((key & g_comp_key_event.keys)) {
    _reset_comp_keys();
  }
  _key_click_process(cur_time_ms);
  _set_keyboard_inactive();
}

static uni_bool _do_filter(uni_s32 event, uni_s32 key) {
  if (!g_keyboard.is_active) {
    LOGW(KEYBOARD_TAG, "invalid key event: %d, keyevent: %d", key);
    if (KEYBOARD_KEY_DOWN != event) {
      return FALSE;
    }
    return TRUE;
  }
  if (key & g_comp_key_event.keys) {
    return TRUE;
  }
  if (key != g_key_event.key_code || g_key_event.pre_key_event == event) {
    LOGT(KEYBOARD_TAG, "previous key is %d, event %d, ignore this %d",
         g_key_event.key_code, g_key_event.pre_key_event, key);
    return FALSE;
  }

  return TRUE;
}

static uni_s32 _keyboard_cb(uni_s32 event, uni_s32 key_code) {
  long      cur_time_ms = uni_get_clock_time_ms();
  KBKeyCode key         = _get_mapping_key(key_code);
  LOGT(KEYBOARD_TAG, "key: %d, keyevent: %d, keytime: %ld", key, event,
       cur_time_ms);
  if (KEY_INVALID == key) {
    LOGW(KEYBOARD_TAG, "invalid key: %d, keyevent: %d, keytime: %ld", key,
         event, cur_time_ms);
    return -1;
  }
  if (!_do_filter(event, key)) {
    return -1;
  }
  _key_process(event, key, cur_time_ms);
  return 0;
}

void KeyBoardRegisterCompKey(KBKeyCode keys, uni_u32 trigger_ms) {
  g_comp_key_event.keys = keys;
  g_comp_key_event.trigger_time_ms = trigger_ms;
  LOGW(KEYBOARD_TAG, "register comp keys", keys);
  return;
}

Result KeyboardInit(void) {
  uni_memset(&g_keyboard, 0, sizeof(g_keyboard));
  _set_keyboard_inactive();
  uni_memset(&g_comp_key_event, 0, sizeof(g_comp_key_event));
  if (0 != uni_hal_keyboard_open()) {
    return E_FAILED;
  }
  uni_hal_keyboard_register_callback(_keyboard_cb);
  return E_OK;
}

void KeyBoardRegisterCallback(KeyboardCallback callback) {
  g_keyboard.cb = callback;
}

void KeyboardFinal(void) {
  TimerStop(g_keyboard.key_press_timer);
  uni_hal_keyboard_close();
  return;
}
#else
Result KeyboardInit(void) { return E_OK; }
void KeyBoardRegisterCallback(KeyboardCallback callback) {}
void KeyboardFinal(void) { return; }
void KeyBoardRegisterCompKey(KBKeyCode keys, uni_u32 trigger_ms) { return; }
#endif
