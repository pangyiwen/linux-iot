
SOURCE_PATH=$(PWD)/source
FILE_LIST+=source_Makefile

#*************************just for test********************#
.PHONY:all build clean test
all:
	echo "ERROR"

build:
	@make build -C $(SOURCE_PATH)/thread

clean:
	@make clean -C $(SOURCE_PATH)/thread

test:
	@echo "==>>>>source_Makefile:$(FILE_LIST)"
	@make test -C $(SOURCE_PATH)/thread
	#make -C $(SOURCE_PATH)/system
	#@make -C $(SOURCE_PATH)/process
	@echo "==>>>>source_Makefile:$(FILE_LIST)"

