# Makefile的使用

在大型项目中，为了快速编译项目，一般都会用到Makefile，因此掌握Makefile的一般语法和使用，对编写和阅读大型项目的Makefile文件很有帮助。

## 模板

~~~shell
EXE = main
SUBDIR = .
# foreach循环中，变量SUBDIR中的单词会被逐一取出放到变量dir中，然后执行第三项的命令，并返回一个字符串，在循环结束后，所有返回的字符串会以空格分隔并返回。
# $(wildcard pattern) pattern为任意通配符如*.c，展开任意符合pattern的文件名，并以空格分隔
CXX_SOURCES = $(foreach dir,&(SUBDIR),$(wildcard $(dir)/*.cpp))
CXX_OBJECTS = $(patsubst %.cpp,%.o,$(CXX_SOURCES))
# .d文件为包含了.o文件依赖关系的文件(行如main.c: stdio.h)
DEP_FILES = $(patsubst %.o,%.d,(CXX_SOURCES))

$(EXE): $(CXX_OBJECTS)
	g++ $(CXX_OBJECTS) -o $(EXE)
%.o: %.cpp
	g++ -c -MMD $< -o $@
# gcc编译器中的选项-MMD将自动寻找依赖的头文件，但输出的依赖文件不包含标准头文件，并生成对应目标文件的.d文件
# -include表示当编译时，包含的头文件不存在时，忽略该错误，继续编译
-include $(DEP_FILES)

clean:
	rm -rf $(CXX_OBJECTS)  $(DEP_FILES)	$(EXE)
~~~

## 语法

- 规则

  ~~~shell
  target: depend
  (tab)cmd
  ~~~
  
- 赋值

  | =    | 最基本的赋值           |
  | ---- | ---------------------- |
  | ：=  | 覆盖之前的值           |
  | ？=  | 如果之前没赋值，则赋值 |
  | +=   | 添加等号后面的值       |

- 取值

  $(var)

- 自动变量

  - $<：规则的第一个依赖
  - $@：规则中的目标
  - $^：规则中的所有依赖

- 模式自动匹配

  子规则中：

  target: depend %.o:%.c 即 main.o:main.c

- 常用函数

  - wildcard : 查找当前目录下所有*.c文件，返回值给src

    src=$(wildcard ./*.c)

  - patsubst 替换所有.c文件为.o文件

    obj=$(patsubst ./%.c, ./%.o, $(src))

