target := http_server
all: $(target) 

src_file := src/main.c \
						src/http_server/http_server.cpp \
						src/http_server/http_request_handler.cpp \
						src/service/rili/rili_call_lift.cpp \
						src/service/call_lift.c \
						src/service/exter_visit.c \
						src/service/inter_visit.c \
						src/service/get_home.c \
						src/service/lift_status.c \
						src/service/response_msg.c \
						src/uart/src/serial.c \
            			src/uart/src/protocol.c \
						src/utils/cJSON/src/cJSON.c \
						src/utils/list/src/list_head.c \
						src/utils/log/src/log.c

INC := -Iinclude \
			-Isrc/http_server \
			-Isrc/service/rili \
			 -Isrc/service \
       		 -Isrc/uart/inc \
			 -Isrc/utils/cJSON/inc \
			 -Isrc/utils/list/inc \
			 -Isrc/utils/log/inc

LIB := -Llib -levent-2.2 -lm -lpthread

$(target):
	g++ $(src_file) $(INC) $(LIB) -o $@

clean:
	rm -rf $(target)

.PHONY: all clean
