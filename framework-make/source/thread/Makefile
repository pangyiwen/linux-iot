
FILE_LIST+=source_thread_Makefile

THREAD_OUT_DIR=$(OUR_DIR)/thread
THREAD_SRC_DIR=$(PWD)
CFLAGS+= -g -Wall


INCLUDE+=$(THREAD_SRC_DIR) 

.PHONY:build clean test
build:
	$(shell mkdir -p $(THREAD_OUT_DIR))
$(THREAD_OUT_DIR)/thread.o:$(THREAD_SRC_DIR)/thread.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

clean:
	rm $(THREAD_OUT_DIR) -rf
#*************************just for test********************#
test:
	@echo ">>>>source_thread_Makefile:$(FILE_LIST)"
	