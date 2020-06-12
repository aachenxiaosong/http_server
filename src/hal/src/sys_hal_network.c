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
#include <stdarg.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "sys_hal_config_def.h"
#include "uni_sys_hal.h"

int uni_hal_get_network_mac(char *mac, int *size) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}

int uni_hal_get_network_ip(char *ip, int *size) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}

int uni_hal_close_network(void) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}

int uni_hal_connect_to_network(struct network_connect *connect) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}

int uni_hal_get_wifi_signal(struct network_wifi_signal *wifi_signal) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}

int uni_hal_network_ping(struct network_ping *ping) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}

int uni_hal_get_network_status(struct network_status *status) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}

int uni_hal_get_available_wifi(char *ssid, char *pwd, char *security) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}

int uni_hal_set_netm_debug_level(int level) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}

int uni_hal_set_netm_monitor_enable(int enable) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}

int uni_hal_set_netm_auto_connect_enable(int enable) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}

int uni_hal_is_network_connected(void) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}

int uni_hal_wifievent_register_callback(WIFI_EVENT_CALLBACK callback) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}

int uni_hal_internetevent_register_callback(INTERNET_EVENT_CALLBACK callback) {
  HAL_PRINT_ERR("%s, no implementation\n", __func__);
  return -1;
}
