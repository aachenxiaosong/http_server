#ifndef _SYS_HAL_CONFIG_DEF_H_
#define _SYS_HAL_CONFIG_DEF_H_

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(PLATFORM_RK3229_NDK)

#define USER_DATA_PATH "/data"

#define NETM_STATUS_RUNNING "running"
#define NETM_STATUS_STOP "stop"
#define NETM_PATH "/data/misc/netm"
#define NETM_STATUS_FILE "/data/misc/netm/netm_status"
#define UNIX_DOMAIN "/data/misc/netm/UNIX.domain"

#define UNI_WIFI_LIST_FILE "/data/misc/netm/wifi_list.data"

#elif defined(PLATFORM_KAR_C600)

#endif

#define HAL_DBG_MODE_PRINTF (1 << 0)
#define HAL_DBG_MODE_ALOGD (1 << 1)
#define HAL_DBG_MODE_ALL (HAL_DBG_MODE_ALOGD | HAL_DBG_MODE_PRINTF)
#define HAL_DBG_MODE_DEFAULT HAL_DBG_MODE_PRINTF

/* POWER MODE */
#define HAL_POWER_MODE_MIN 0
#define HAL_POWER_MODE_SLEEP 1
#define HAL_POWER_MODE_IDLE 2
#define HAL_POWER_MODE_RUN_LOW 3
#define HAL_POWER_MODE_RUN_L1 4
#define HAL_POWER_MODE_RUN_L2 5
#define HAL_POWER_MODE_RUN_L3 6
#define HAL_POWER_MODE_RUN_HIGH 7
#define HAL_POWER_MODE_MAX 8

#if defined(PLATFORM_RK3229_NDK)
#define HAL_CPU_CORE_0 0
#define HAL_CPU_CORE_1 1
#define HAL_CPU_CORE_2 2
#define HAL_CPU_CORE_3 3
#elif defined(PLATFORM_KAR_C600)
#define HAL_CPU_CORE_0 0
#endif

#if defined(__cplusplus)
} /* extern "C" */
#endif

#endif /* _SYS_HAL_CONFIG_DEF_H_ */
