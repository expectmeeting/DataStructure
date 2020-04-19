[TOC]

# 实验介绍

本实验采用qemu搭建虚拟机进行，因此需要qemu环境和实验源码

## qemu环境

- [qemu官方网站下载](https://www.qemu.org/download/)

- 安装

  ~~~shell
  # 1、解压
  # 2、配置
  ./configure --target-list=x86_64-softmmu,i386-softmmu
  # 3、编译
  make -j8
  # 4、安装
  make install
  ~~~

## 实验源码

- [git源码地址](https://github.com/chyyuu/ucore_os_lab)

## 调试

~~~shell
# 1、编译(打印执行的命令)
make V=
# 2、qemu运行(终端1)
qemu -S -s -hda ./bin/ucore.img -monitor stdio
# 3、gdb调试(终端2)
gdb
（gdb） target remote 127.0.0.1:1234
（gdb） file ./bin/kernel
（gdb） r
# 或
gdb -x tools/gdbinit
~~~

- 常用gdb命令

  ~~~shell
  # 1、查看寄存器
  (gdb) i r       # 查看所有寄存器的值,i、r分别是info和registers的简称
  (gdb) i r a     # 查看所有寄存器的值(包括浮点和多媒体)
  (gdb) i r esp   # 查看寄存器esp的值
  # 2、查看内存
  (gdb) x /wx 0x80040000    # 以16进制显示指定地址处连续4字节数据
  (gdb) x /8x $esp		  # 显示寄存器esp的值作为内存地址所对应的连续8*4字节数据
  (gdb) x /16x $esp+12      # 显示寄存器esp的值 + 12作为内存地址所对应的连续16*4字节数据，
  (gdb) x /16s 0x86468700   # 以字符串形式显示指定地址处的数据
  (gdb) x /24i 0x8048a51    # 以指令形式显示指定地址处的数据（24条）
  # 3. 修改寄存器的值
  (gdb) set $epc = 0xbfc00000
  # 4. 修改内存的值
  (gdb) set {unsigned int}0x8048a51=0x0
  (gdb) set *(unsigned int*)0x8048a54=0x55aa55aa
  ~~~
  
  