#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <stdarg.h>
#include "sys_hal_config_def.h"
#include "uni_sys_hal.h"

int uni_hal_get_user_data_path(char *paths, int paths_max_size, int *count) {
  HAL_PRINT_ERR("%s, no implementation.\n", __func__);

  return -1;
}

int uni_hal_get_user_data_path_free_size(char *path, uint64_t *avail_size) {
  HAL_PRINT_ERR("%s, no implementation.\n", __func__);

  return -1;
}
