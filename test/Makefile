target := lift_ctrl_board
all: $(target) 

src_file := lift_ctrl_board.c \
            uart/src/serial.c \
	    utils/list/src/list_head.c \
	    utils/log/src/log.c

INC := -Iinclude \
       -Iuart/inc \
       -Iutils/list/inc \
       -Iutils/log/inc

LIB := -lm -lpthread

$(target):
	gcc $(src_file) $(INC) $(LIB) -o $@

clean:
	rm -rf $(target)

.PHONY: all clean
