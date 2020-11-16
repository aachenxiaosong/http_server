target := http_server
all: $(target) 

hal_src := src/hal/src
hal_inc := src/hal/inc

utils_src := src/utils/bitmap/src \
             src/utils/json/src \
             src/utils/data_buf/src \
             src/utils/device/src \
             src/utils/encrypt/src \
             src/utils/event/src \
             src/utils/event_list/src \
             src/utils/fsm/src \
             src/utils/hash/src \
             src/utils/http/src \
             src/utils/interruptable_sleep/src \
             src/utils/list/src \
             src/utils/log/src \
             src/utils/uuid/src \
             src/utils/uart/src \
             src/utils/md5sum/src \
             src/utils/memcheck/src \
             src/utils/system_resource/src \
             src/utils/timer/src \
             src/utils/time_tick/src \
             src/utils/rwlock
utils_inc := src/utils/uni_util/inc \
             src/utils/bitmap/inc \
             src/utils/json/inc \
             src/utils/data_buf/inc \
             src/utils/device/inc \
             src/utils/encrypt/inc \
             src/utils/event/inc \
             src/utils/event_list/inc \
             src/utils/hash/inc \
             src/utils/http/inc \
             src/utils/fsm/inc \
             src/utils/interruptable_sleep/inc \
             src/utils/list/inc \
             src/utils/log/inc \
             src/utils/uuid/inc \
             src/utils/uart/inc \
             src/utils/md5sum/inc \
             src/utils/memcheck/inc \
             src/utils/system_resource/inc \
             src/utils/timer/inc \
             src/utils/time_tick/inc \
             src/utils/rwlock

sdk_src := src/sdk/http/http_server \
           src/sdk/http/http_client \
           src/sdk/tcp/tcp_server \
           src/sdk/tcp/tcp_client \
           src/sdk/tcp/tcp_conn_mgr \
           src/sdk/mqtt/mqtt_client

sdk_inc := src/sdk/http/http_server \
           src/sdk/http/http_client \
           src/sdk/tcp/tcp_server \
           src/sdk/tcp/tcp_client \
           src/sdk/tcp/tcp_conn_mgr \
           src/sdk/tcp/tcp_handle \
           src/sdk/mqtt/mqtt_client \
           src/sdk/mqtt/mqtt_handle \
           src/sdk/message \
           src/sdk/message/message_type \
           src/sdk/message/message_handler \
           src/sdk/message/message_dispatcher

app_src := src/app \
           src/app/left/lift_control/base/http \
           src/app/left/lift_control/rili/http \
           src/app/left/lift_control/wlong/http \
           src/app/right/common/auth \
           src/app/right/common/hb \
           src/app/right/common \
           src/app/right/lift_control/rili \
           src/app/right/lift_control/wlong \
           src/app/up/sulink/data \
           src/app/up/sulink/http \
           src/app/up/sulink/mqtt \
           src/app/up/sulink/mqtt/packer \
           src/app/up/sulink/mqtt/message_handlers \
           src/app/down/child_device/dechang \
           src/app/down/child_device/dechang/tcp \
           src/app/down/child_device/dechang/tcp/packer \
           src/app/down/child_device/dechang/tcp/message_handlers

app_inc := src/app \
           src/app/config \
           src/app/left/lift_control/base/http \
           src/app/left/lift_control/rili/http \
           src/app/left/lift_control/wlong/http \
           src/app/right/common/auth \
           src/app/right/common/hb \
           src/app/right/common \
           src/app/right/lift_control/rili \
           src/app/right/lift_control/wlong \
           src/app/up/sulink/data \
           src/app/up/sulink/http \
           src/app/up/sulink/mqtt \
           src/app/up/sulink/mqtt/messages \
           src/app/up/sulink/mqtt/packer \
           src/app/up/sulink/mqtt/message_handlers \
           src/app/down/child_device/dechang \
           src/app/down/child_device/dechang/tcp \
           src/app/down/child_device/dechang/tcp/messages \
           src/app/down/child_device/dechang/tcp/packer \
           src/app/down/child_device/dechang/tcp/message_handlers

lib_inc := lib/libevent/inc \
           lib/mqtt/inc

test_src := src/sdk/tcp/test
test_inc := src/sdk/tcp/test


SRC := $(hal_src) $(utils_src) $(sdk_inc) $(app_src) $(test_src)
INC := $(hal_inc) $(utils_inc) $(sdk_inc) $(app_inc) $(lib_inc) $(test_inc)

# functions -----------------------
change_file_location = $(foreach f,$1,$2/$(notdir $(f)))
c_to_o = $(subst .c,.o,$1)
cpp_to_o = $(subst .cpp,.o,$1)
o_to_d = $(subst .o,.o.d,$1)

# compile parameters -------------
vpath %.c $(SRC)
vpath %.cpp $(SRC)

BUILD_DIR := build
INC_FLAGS := $(foreach inc, $(INC),-I$(inc))
C_FILES := $(foreach dir, $(SRC), $(wildcard $(dir)/*.c))
CPP_FILES := $(foreach dir, $(SRC), $(wildcard $(dir)/*.cpp))
C_OBJ_FILES := $(call c_to_o,$(C_FILES))
C_OBJ_FILES := $(call change_file_location, $(C_OBJ_FILES), $(BUILD_DIR))
CPP_OBJ_FILES := $(call cpp_to_o,$(CPP_FILES))
CPP_OBJ_FILES := $(call change_file_location, $(CPP_OBJ_FILES), $(BUILD_DIR))
DEP_FILES := $(call o_to_d,$(C_OBJ_FILES) $(CPP_OBJ_FILES))

LIB := -Llib/libevent -Llib/mqtt -Llib/poco -levent-2.2 -lpaho-mqtt3c -lPocoData -lPocoDataSQLite -lPocoFoundation -lcurl -lm -lpthread

ifneq ($(DEP_FILES),)
-include $(DEP_FILES)
endif

CFLAGS := -g -std=c++11
IS_UBUNTU=$(shell uname -a | grep ubuntu)
ifneq ($(IS_UBUNTU),)
CFLAGS += -DIS_UBUNTU
endif
$(C_OBJ_FILES):$(BUILD_DIR)/%.o: %.c
	g++ $(CFLAGS) $(INC_FLAGS) $(LIB) -Wp,-MD,$@.d -c -o $@ $<

$(CPP_OBJ_FILES):$(BUILD_DIR)/%.o: %.cpp
	g++ $(CFLAGS) $(INC_FLAGS) $(LIB) -Wp,-MD,$@.d -c -o $@ $<
  
$(target): prepare $(C_OBJ_FILES) $(CPP_OBJ_FILES)
	g++ $(CFLAGS) $(C_OBJ_FILES) $(CPP_OBJ_FILES) $(INC_FLAGS) $(LIB) -o $@

prepare:
	@[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(target) build/*

.PHONY: all clean
