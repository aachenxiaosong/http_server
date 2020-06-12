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

#include "uni_shell.h"
#include "uni_iot.h"
int ShellSystem(char *cmd) {
  int rc = -1;
  if (NULL == cmd) {
    return rc;
  }
  rc = system(cmd);
  if (-1 != rc && WIFEXITED(rc) && 0 == WEXITSTATUS(rc)) {
    rc = 0;
  }

  return rc;
}

int ShellRmDir(char *path) {
  DIR *dirent = NULL;
  struct dirent * ptr = NULL;
  char sub_path[256] = {0};
  dirent = opendir(path);
  if (NULL == dirent) {
    return -1;
  }
  while (NULL != (ptr = readdir(dirent))) {
    if (0 == strcmp(ptr->d_name, ".") ||
        0 == strcmp(ptr->d_name, "..")) {
      continue;
    }
    snprintf(sub_path, sizeof(sub_path), "%s/%s", path, ptr->d_name);
    if (DT_LNK == ptr->d_type ||
        DT_REG == ptr->d_type) {
      remove(sub_path);
    } else if (DT_DIR == ptr->d_type) {
      ShellRmDir(sub_path);
    }
  }
  closedir(dirent);
  return remove(path);
}
