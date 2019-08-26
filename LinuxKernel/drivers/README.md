# linux驱动模块

**模块**是linux内核提供的一种机制，具有如下特点：

- 模块本身不被编译入内核印象，从而控制内核大小。
- 模块一旦被加载，就跟内核中其他部分完全一样。

驱动 

## linux设备模型

重要的结构体：

- kobject
- kset
- sysfs
- udev



## 内核的基本调试方法

内核有如下几种调试方法：

### printk

打印是所有程序调试的基本方法，在内核中打印信息使用printk函数。

- 内核打印定义了如下几个级别

  ~~~c
  #define KERN_EMERG "<0>"	#emgergent紧急
  #define KERN_ALERT "<1>"	#alert警戒
  #define KERN_CRIT "<2>"		#critical重要的
  #define KERN_ERR "<3>"		#error错误
  #define KERN_WARNING "<4>"	#warning警告
  #define KERN_NOTICE "<5>"	#notice注意
  #define KERN_INFO "<6>"		#information信息
  #define KERN_DEBUG "<7>"	#debug调试信息
  ~~~

  日志级别范围为0~7，没有指定级别则默认级别为4，其通过kernel/printk.c中的宏定义：

  ~~~c
  #define DEFAULT_MESSAGE_LOGLEVEL 4
  ~~~

  当级别高于4时，即0~3，将在终端上打印。

  **调试时**，可通过如下命令使所有打印均输出到终端：

  ~~~shell
  echo 8 > /proc/sys/kernel/printk
  ~~~

- dmesg 命令：通过查看/proc/kmsg文件可以打印所有的内核信息。

- 打开和关闭调试信息：通过宏封装printk，当不调试时不使用printk。

### oops

内核发生panic时，所产生的一个调试信息。

### kprobe



### kcore

