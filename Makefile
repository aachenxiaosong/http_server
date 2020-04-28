target := http_server
all: $(target) 

src_file := http_server.c \
						service/call_lift.c \
						service/exter_visit.c \
						service/inter_visit.c \
						service/get_home.c \
						service/lift_status.c \
						service/response_msg.c \
            uart/src/serial.c \
            uart/src/protocol.c \
						utils/cJSON/src/cJSON.c \
						utils/list/src/list_head.c \
						utils/log/src/log.c

INC := -Iinclude \
			 -Iservice \
       -Iuart/inc \
			 -Iutils/cJSON/inc \
			 -Iutils/list/inc \
			 -Iutils/log/inc

LIB := -Llib -levent-2.2 -lm -lpthread

$(target):
	gcc $(src_file) $(INC) $(LIB) -o $@

clean:
	rm -rf $(target)

.PHONY: all clean
