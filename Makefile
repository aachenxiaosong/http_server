target := http_server
all: $(target) 

src_file := src/Main.cpp \
						src/http_server/HttpServer.cpp \
						src/http_server/IHttpRequestHandler.cpp \
						src/http_service/IHttpService.cpp \
						src/http_service/rili/RiliHttpService.cpp \
						src/http_service/rili/RiliCallLiftHttpHandler.cpp \
						src/http_service/rili/RiliBookLiftHttpHandler.cpp \
						src/http_service/rili/RiliTakeLiftHttpHandler.cpp \
						src/uart/src/serial.c \
						src/utils/cJSON/src/cJSON.c \
						src/utils/list/src/list_head.c \
						src/utils/log/src/log.c

INC := -Iinclude \
			-Isrc/http_server \
			-Isrc/http_service \
			-Isrc/http_service/rili \
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
