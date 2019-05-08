
#***********************************************************************
#** File name: makefile
#** Function decription: auto complie and link,and output exe file
#** Author:Peng Wen
#** date:20180726
#** change:
#** Version:v1.0
#***********************************************************************

#** Varaible
EXE = out
SUBDIR = .
CXX_SOURCES = $(foreach dir,&(SUBDIR),$(wildcard $(dir)/*.c))
CXX_OBJECTS = $(patsubst %.c,%.o,$(CXX_SOURCES))
DEP_FILES = $(patsubst %.o,%.d,(CXX_SOURCES))

$(EXE): main.o threadpool.o
	gcc main.o threadpool.o -o out -lpthread
main.o: main.c
	gcc -c main.c -o main.o -lpthread
threadpool.o: threadpool.c
	gcc -c threadpool.c -o threadpool.o -lpthread
