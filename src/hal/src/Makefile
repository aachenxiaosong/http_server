
MODULE_NAME := libsyshal.so
CFLAGS := -pthread -fPIC -shared $(INC_FLAGS) $(DEF_FLAGS)


TARGET := $(OUTDIR)/$(MODULE_NAME)
SRC := $(wildcard *.c)  

.PHONY: all clean
all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
	mkdir -p $(LIBS)
	cp -fv $(TARGET) $(LIBS)/
	
clean:
	rm -rf *.o
	rm -rf $(OUTDIR)/*.o
	rm -rf $(TARGET)
	rm -rf $(LIBS)/$(MODULE_NAME)
