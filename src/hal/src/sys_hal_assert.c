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
#include <sys/select.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <stdarg.h>
#include <assert.h>

#include "uni_sys_hal.h"

void uni_hal_assert_func(const char *file, int line, const char *expr_str) {
  HAL_PRINT_ERR("[%s:%d] ASSERT: (%s)\n", file, line, expr_str);
  // abort();
  assert(1==0);
}
