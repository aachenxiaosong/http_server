#ifndef _SYS_HAL_H_
#define _SYS_HAL_H_

#if defined(__cplusplus)
extern "C" {
#endif

#define PLATFORM_RDA5981   0x5981
#define UNI_HAL_PLATFORM   PLATFORM_RDA5981


#define UNUSED(_var)   (void)(_var)


#define HAL_DBG_LEVEL_MIN                     0    
#define HAL_DBG_LEVEL_ERR                     1    
#define HAL_DBG_LEVEL_WARN                  2    
#define HAL_DBG_LEVEL_NOTICE                3    
#define HAL_DBG_LEVEL_INFO                    4    
#define HAL_DBG_LEVEL_DEBUG                 5    
#define HAL_DBG_LEVEL_MAX                    6    
#define HAL_DBG_LEVEL_DEFAULT             HAL_DBG_LEVEL_ERR

#define HAL_PRINT_ERR(x ...)              uni_hal_debug_print(HAL_DBG_LEVEL_ERR, "<ERR> " x)
#define HAL_PRINT_WARN(x ...)           uni_hal_debug_print(HAL_DBG_LEVEL_WARN, "<WARN> " x)
#define HAL_PRINT_NOTICE(x ...)         uni_hal_debug_print(HAL_DBG_LEVEL_NOTICE, "<NOTICE> " x)
#define HAL_PRINT_INFO(x ...)            uni_hal_debug_print(HAL_DBG_LEVEL_INFO, "<INFO> " x)
#define HAL_PRINT_DEBUG(x ...)         uni_hal_debug_print(HAL_DBG_LEVEL_DEBUG, "<DEBUG> " x)


/* POWER MODE */
#define HAL_POWER_MODE_MIN                  0
#define HAL_POWER_MODE_SLEEP               1
#define HAL_POWER_MODE_IDLE                  2
#define HAL_POWER_MODE_RUN_LOW          3
#define HAL_POWER_MODE_RUN_L1               4
#define HAL_POWER_MODE_RUN_L2               5
#define HAL_POWER_MODE_RUN_L3               6
#define HAL_POWER_MODE_RUN_HIGH         7
#define HAL_POWER_MODE_MAX                  8


/* TASK PRIORITY */
#define OS_PRIORITY_IDLE                          -1     //idle (lowest)
#define OS_PRIORITY_LOW                          -2     //low
#define OS_PRIORITY_NORMAL                      0     //normal
#define OS_PRIORITY_HIGH                          1     //high
#define OS_PRIORITY_REALTIME                    2    //realtime (highest)

/* network */
struct network_connect {
    char *ssid;    //"WiFi-Unisound-2.4G";
    char *pwd;     //"2017unisound";
    char *security;  //"WPA-PSK-CCMP";
};

struct network_wifi_signal {
    int rssi_level;
    int linkspeed;
    int noise;
    int freq;
};

struct network_ping {
    char *host;      //if host is NULL, use the default host name or host ip
    int timeout;      //in second
    int size;           //ping size, from 4 to 65400, defualt 32
    int send_cnt;   //ping send count, default 4
    int recv_cnt;    //ping receive count
    int rtt;              //-1: DISCONNECT,  in ms
};

struct network_status {
    struct network_wifi_signal wifi_signal;
    struct network_ping ping;
};


/* user data partition */
extern int uni_hal_get_user_data_path(char *paths, int paths_max_size, int *count); /* paths_max_size is the max length of paths */
extern int uni_hal_get_user_data_path_free_size(char *path, uint64_t *avail_size);

/* file operation interface */
extern int uni_hal_fopen(const char *pathname, int flags, ... );
extern ssize_t uni_hal_fwrite (int fd, const void *buf, size_t count);
extern ssize_t uni_hal_fread(int fd, void *buf, size_t count);
extern int uni_hal_fclose(int fd);
extern int uni_hal_lseek(int fd,long offset,int whence);
extern int uni_hal_unlink(const char *pathname);



/* power mode interface */
extern int uni_hal_select_cpu_core(int core, int online); /*core: [1,2,3], online: 0 or 1*/
extern int uni_hal_set_power_mode(int power_mode);

/*0 Ã»??Á¬??WIFI?Èµ?  1??Á¬????WIFI?Èµ? */
typedef int (*WIFI_EVENT_CALLBACK)(int event);

/*0 INTERNET???Ü·???  1 INTERNET???Ô·??? */
typedef int (*INTERNET_EVENT_CALLBACK)(int event);

/* network */
extern int uni_hal_get_network_mac(char *mac, int *size);
extern int uni_hal_get_network_ip(char *ip, int *size);
extern int uni_hal_close_network(void);
extern int uni_hal_connect_to_network(struct network_connect *connect);
extern int uni_hal_get_wifi_signal(struct network_wifi_signal *wifi_signal);
extern int uni_hal_network_ping(struct network_ping *ping);
extern int uni_hal_get_network_status(struct network_status *status);
extern int uni_hal_get_available_wifi(char *ssid, char *pwd, char *security);
extern int uni_hal_set_netm_debug_level(int level);
extern int uni_hal_set_netm_monitor_enable(int enable);
extern int uni_hal_set_netm_auto_connect_enable(int enable);
extern int uni_hal_wifievent_register_callback(WIFI_EVENT_CALLBACK callback);
extern int uni_hal_internetevent_register_callback(INTERNET_EVENT_CALLBACK callback);

/* debug interface */
extern int uni_hal_set_debug_name(char *debug_name);
extern int uni_hal_set_debug_level(int level);
extern int uni_hal_set_debug_mode(int mode);
extern int uni_hal_debug_print(int level, const char *fmt, ...);

/* utility */
extern int uni_hal_exec_command(char * in, char * out, int outLen);


/* thread interface */
struct thread_param {
    int stack_size;    //stack_size, 0 for default
    int th_priority;   //0 for default
    char task_name[16];
};


/* thread interface */
//extern int uni_hal_thread_create(void **thread, int stack_size, int th_priority, void (*start_routine)(void));
extern int uni_hal_thread_create(void **thread, struct thread_param *para, void* (*start_routine)(void *), void* arg);
extern int uni_hal_thread_detach(void *thread);
extern int uni_hal_thread_exit(void *thread);
extern int uni_hal_thread_destroy(void *thread);

/* semaphore */
extern int uni_hal_sem_init(void **sem, unsigned int value);
extern int uni_hal_sem_wait(void *sem);
extern int uni_hal_sem_post(void *sem);
extern int uni_hal_sem_destroy(void *sem);

/* mutex */
extern int uni_hal_mutex_init(void **mutex);
extern int uni_hal_mutex_lock(void *mutex);
extern int uni_hal_mutex_unlock(void *mutex);
extern int uni_hal_mutex_destroy(void *mutex);

/* assert */
#define uni_hal_assert(expr) {if(!(expr)) {uni_hal_assert_func(__func__, __LINE__, #expr);}}
extern void uni_hal_assert_func(const char *file, int line, const char *expr_str);

/* sleep */
extern int uni_hal_usleep(unsigned int us);
extern int uni_hal_msleep(unsigned int ms);
extern int uni_hal_sleep(unsigned int s);

/* stdlib */
void *uni_hal_malloc(size_t size);
void uni_hal_free(void *ptr);
void *uni_hal_calloc(size_t n, size_t size);
long int uni_hal_strtol(const char *nptr,char **endptr,int base);
long long int uni_hal_strtoll(const char *nptr, char **endptr, int base); 
float uni_hal_strtof(const char *nptr, char **endptr);
double uni_hal_strtod(const char *nptr, char **endptr);

/* THREAD STACK SIZE */
#define STACK_DEFAULT_SIZE          0
#define STACK_SIZE                  (1024 * 1024 * 8)
#define STACK_SMALL_SIZE            STACK_SIZE * 1
#define STACK_NORMAL_SIZE           STACK_SIZE * 1
#define STACK_MIDDLE_SIZE           STACK_SIZE * 1
#define STACK_LARGE_SIZE            STACK_SIZE * 1


#if defined(__cplusplus)
}  /* extern "C" */
#endif

#endif /* _SYS_HAL_H_ */

