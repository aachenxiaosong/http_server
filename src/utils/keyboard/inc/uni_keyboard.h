/**************************************************************************
 * Copyright (C) 2017-2017  Unisound
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
 * Description : uni_keyboard.h
 * Author      : guozhenkun@unisound.com
 * Date        : 2018.12.10
 *
 **************************************************************************/
#ifndef UTILS_INC_UNI_KEYBOARD_H_
#define UTILS_INC_UNI_KEYBOARD_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "uni_iot.h"
#define KEY_INVALID    (0)
#define KEY_VOLUME_INC (1 << 0)
#define KEY_VOLUME_DEC (1 << 1)
#define KEY_NEXT       (1 << 2)
#define KEY_PRE        (1 << 3)
#define KEY_PAUSE      (1 << 4)
#define KEY_WAKEUP     (1 << 5)
#define KEY_PHONE      (1 << 6)
#define KEY_MUTE       (1 << 7)
#define KEY_MAX        (1 << 8)
typedef uni_u32 KBKeyCode;

#include "uni_iot.h"
typedef enum {
  CLICK_SINGLE,
  CLICK_LONG_PRESS,
  CLICK_PRESS_KEEP,
  CLICK_PRESS_RELEASE,
  CLICK_COMP_PRESS
} KBClickType;

typedef struct KBEvent {
  KBClickType type;
  KBKeyCode   key;
} KBEvent;

typedef void (* KeyboardCallback)(KBEvent *event);

Result KeyboardInit(void);
void   KeyBoardRegisterCallback(KeyboardCallback callback);
void   KeyBoardRegisterCompKey(KBKeyCode keys, uni_u32 trigger_ms);
void   KeyboardFinal(void);
#ifdef __cplusplus
}
#endif
#endif  //  UTILS_INC_UNI_KEYBOARD_H_
