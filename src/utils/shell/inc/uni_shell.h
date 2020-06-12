/**********************************************************************
 * Copyright (C) 2018-2019  Unisound
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
 **********************************************************************
 *
 * Description : uni_shell.h
 * Author      : guozhenkun@unisound.com
 * Date        : 2019.01.02
 *
 **********************************************************************/

#ifndef UTILS_SHELL_INC_UNI_SHELL_H_
#define UTILS_SHELL_INC_UNI_SHELL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "uni_iot.h"
int ShellSystem(char *cmd);

int ShellRmDir(char *path);

#ifdef __cplusplus
}
#endif
#endif  //
