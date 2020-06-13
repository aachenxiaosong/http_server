target := http_server
all: $(target) 

hal_src_file := src/hal/src/sys_hal_assert.c \
            src/hal/src/sys_hal_debug.c \
            src/hal/src/sys_hal_file_if.c \
            src/hal/src/sys_hal_mutex.c \
            src/hal/src/sys_hal_network.c \
            src/hal/src/sys_hal_power_mode.c \
            src/hal/src/sys_hal_pthread.c \
            src/hal/src/sys_hal_sem.c \
            src/hal/src/sys_hal_sleep.c \
            src/hal/src/sys_hal_stdlib.c \
            src/hal/src/sys_hal_user_partition.c
hal_inc := -Isrc/hal/inc

utils_src_file := src/utils/bitmap/src/uni_bitmap.c \
            src/utils/cJSON/src/uni_json.c \
            src/utils/cJSON/src/cJSON.c \
            src/utils/data_buf/src/uni_databuf.c \
			src/utils/device/src/uni_device.c \
			src/utils/encrypt/src/uni_sha.c \
            src/utils/event/src/uni_event.c \
            src/utils/event_list/src/uni_event_list.c \
            src/utils/fsm/src/uni_fsm.c \
            src/utils/hash/src/bkdr_hash.c \
            src/utils/http/src/uni_http.c \
            src/utils/interruptable_sleep/src/uni_interruptable.c \
            src/utils/list/src/list_head.c \
            src/utils/log/src/uni_log.c \
            src/utils/uart/src/serial.c \
            src/utils/md5sum/src/uni_md5sum.c \
            src/utils/memcheck/src/uni_memcheck.c \
			src/utils/timer/src/uni_timer.c

utils_inc := -Isrc/utils/bitmap/inc \
                           -Isrc/utils/cJSON/inc \
                           -Isrc/utils/data_buf/inc \
						   -Isrc/utils/device/inc \
						   -Isrc/utils/encrypt/inc \
                           -Isrc/utils/event/inc \
                           -Isrc/utils/event_list/inc \
                           -Isrc/utils/hash/inc \
                           -Isrc/utils/http/inc \
                           -Isrc/utils/fsm/inc \
                           -Isrc/utils/interruptable_sleep/inc \
                           -Isrc/utils/list/inc \
                           -Isrc/utils/log/inc \
                           -Isrc/utils/uart/inc \
                           -Isrc/utils/md5sum/inc \
                           -Isrc/utils/memcheck/inc \
						   -Isrc/utils/timer/inc

src_file := $(hal_src_file) $(utils_src_file) src/Main.cpp \
						src/http_server/src/HttpServer.cpp \
						src/http_server/src/IHttpRequestHandler.cpp \
						src/http_client/auth/src/uni_auth_http.c \
						src/http_client/resource_prefetch/src/uni_resource_prefetch.c \
						src/http_service/IHttpService.cpp \
						src/http_service/rili/RiliHttpService.cpp \
						src/http_service/rili/RiliCallLiftHttpHandler.cpp \
						src/http_service/rili/RiliBookLiftHttpHandler.cpp \
						src/http_service/rili/RiliTakeLiftHttpHandler.cpp \
						src/http_service/wlong/WlongHttpService.cpp \
						src/http_service/wlong/WlongCallLiftHttpHandler.cpp \
						src/http_service/wlong/WlongBookLiftHttpHandler.cpp \
						src/http_service/wlong/WlongTakeLiftHttpHandler.cpp \
						src/mc/src/MQTTClient.c \
						src/mc/src/MQTTLinux.c \
						src/mc/src/uni_mc_param.c \
						src/mc/src/uni_mc_service.c \
						src/mc/src/uni_msg_center.c \
						src/mc/src/uni_cloud_utils.c
					

INC := $(hal_inc) $(utils_inc) \
			-Isrc \
			-Isrc/http_server/inc \
			-Isrc/http_client/auth/inc \
			-Isrc/http_client/resource_prefetch/inc \
			-Isrc/http_service \
			-Isrc/http_service/rili \
			-Isrc/http_service/wlong \
			-Isrc/mc/inc \
			-Isrc/mc/inc/mqtt

LIB := -Llib -levent-2.2 -lpaho-embed-mqtt3c -lcurl -lm -lpthread

$(target):
	g++ -g $(src_file) $(INC) $(LIB) -o $@

clean:
	rm -rf $(target)

.PHONY: all clean
