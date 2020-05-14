#列出工作目录下所有以“.c”结尾的文件，以空格分隔，将文件列表赋给变量SOURCE
#SOURCE = $(wildcard *.c)
#调用patsubst函数，生成与源文件对应的“.o”文件列表
#OBJECTS = $(patsubst %.c,%.o,$(SOURCE))
#gcc -MM main.c (-MM可以查看main.c的依赖文件)

#***********************************************************#
include base.mk
#**********************************************************#
#export将FILE_LIST变量向下传递,同级目录无论先后,其值不变
export FILE_LIST+=Makefile
export SDK_DIR=$(PWD)
export OUR_DIR=$(PWD)/out

#**********************************************************#
.PHONY:all build clean test
all:
	@echo "make: make build;make clean;make test" 
build:
	@make build -C $(PWD)/source
	
clean:
	@make clean -C $(PWD)/source
	rm -rf $(OUR_DIR)
#*************************just for test********************#
test:
	@echo "==============================================="
	@echo "=>>>>Makefile:$(FILE_LIST)"
	@make test -C $(PWD)/source
	@echo "==============================================="



