target := uni_gateway 
all: $(target) 

utils_src := src/utils/util/src \
             src/utils/config/src \
             src/utils/json/src \
             src/utils/data_buf/src \
             src/utils/device_info/src \
             src/utils/encrypt/src \
             src/utils/http/src \
             src/utils/list/src \
             src/utils/log/src \
             src/utils/uart/src \
             src/utils/memcheck/src \
             src/utils/rwlock
utils_inc := src/utils/util/inc \
             src/utils/config/inc \
             src/utils/json/inc \
             src/utils/data_buf/inc \
             src/utils/device_info/inc \
             src/utils/encrypt/inc \
             src/utils/http/inc \
             src/utils/list/inc \
             src/utils/log/inc \
             src/utils/uart/inc \
             src/utils/memcheck/inc \
             src/utils/rwlock \
             src/utils/serialization

sdk_src := src/sdk/http/http_server \
           src/sdk/http/http_client \
           src/sdk/tcp/tcp_server \
           src/sdk/tcp/tcp_client \
           src/sdk/tcp/tcp_conn_mgr \
           src/sdk/udp/udp_server \
           src/sdk/udp/udp_client \
           src/sdk/mqtt/mqtt_client \
           src/sdk/mq

sdk_inc := src/sdk/http/http_server \
           src/sdk/http/http_client \
           src/sdk/tcp/tcp_server \
           src/sdk/tcp/tcp_client \
           src/sdk/tcp/tcp_conn_mgr \
           src/sdk/tcp/tcp_handle \
           src/sdk/udp/udp_server \
           src/sdk/udp/udp_client \
           src/sdk/mqtt/mqtt_client \
           src/sdk/mqtt/mqtt_handle \
           src/sdk/message \
           src/sdk/message/message_type \
           src/sdk/message/message_handler \
           src/sdk/mq

app_src := src/app \
           src/app/left/lift_control \
           src/app/left/lift_control/service \
           src/app/left/lift_control/service/http/packer \
           src/app/left/lift_control/service/http/handler \
           src/app/left/lift_control/service/mq/packer \
           src/app/left/lift_control/service/mq/handler \
           src/app/left/lift_control/message_handlers \
           src/app/left/lift_control/message_handlers/rili \
           src/app/left/lift_control/message_handlers/wlong \
           src/app/left/lift_control/message_handlers/slingsh \
           src/app/left/lift_control/message_handlers/sling \
           src/app/right/lift_control/rili \
           src/app/right/lift_control/wlong \
           src/app/right/lift_control/slingsh \
           src/app/right/lift_control/sling \
           src/app/up/sulink \
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
           src/app/left/lift_control \
           src/app/left/lift_control/service \
           src/app/left/lift_control/service/http/packer \
           src/app/left/lift_control/service/http/handler \
           src/app/left/lift_control/service/mq/packer \
           src/app/left/lift_control/service/mq/handler \
           src/app/left/lift_control/messages \
           src/app/left/lift_control/message_handlers \
           src/app/left/lift_control/message_handlers/rili \
           src/app/left/lift_control/message_handlers/wlong \
           src/app/left/lift_control/message_handlers/slingsh \
           src/app/left/lift_control/message_handlers/sling \
           src/app/right/lift_control/rili \
           src/app/right/lift_control/wlong \
           src/app/right/lift_control/slingsh \
           src/app/right/lift_control/sling \
           src/app/up/sulink \
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
           lib/mqtt/inc \
           lib/poco/inc \
           lib/boost/inc \
           lib/zmq/inc

test_src := src/sdk/tcp/test
test_inc := src/sdk/tcp/test


SRC := $(utils_src) $(sdk_src) $(app_src) $(test_src)
INC := $(utils_inc) $(sdk_inc) $(app_inc) $(lib_inc) $(test_inc)

# functions -----------------------
change_file_location = $(foreach f,$1,$2/$(notdir $(f)))
c_to_o = $(subst .c,.o,$1)
cpp_to_o = $(subst .cpp,.o,$1)
o_to_d = $(subst .o,.d,$1)

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

LIB := -Llib/libevent -Llib/mqtt -Llib/poco -Llib/boost -Llib/zmq -levent-2.2 -lpaho-mqtt3c -lPocoData -lPocoDataSQLite -lPocoFoundation -lPocoNetSSL -lPocoNet -lPocoCrypto -lPocoUtil -lPocoXML -lPocoJSON -lboost_timer -lboost_system -lboost_chrono -lboost_serialization -lzmq -lcurl -lssl -lcrypto -lm -lpthread

ifneq ($(DEP_FILES),)
-include $(DEP_FILES)
endif

CFLAGS := -g -std=c++11
IS_VMWARE=$(shell ifconfig | grep ens33)
ifneq ($(IS_VMWARE),)
CFLAGS += -DIS_VMWARE
endif
$(C_OBJ_FILES):$(BUILD_DIR)/%.o: %.c
	g++ $(CFLAGS) $(INC_FLAGS) $(LIB) -MMD -MP -c -o $@ $<

$(CPP_OBJ_FILES):$(BUILD_DIR)/%.o: %.cpp
	g++ $(CFLAGS) $(INC_FLAGS) $(LIB) -MMD -MP -c -o $@ $<
  
$(target): prepare $(C_OBJ_FILES) $(CPP_OBJ_FILES)
	g++ $(CFLAGS) $(C_OBJ_FILES) $(CPP_OBJ_FILES) $(INC_FLAGS) $(LIB) -o $@

prepare:
	@[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(target) build/*

.PHONY: all clean
