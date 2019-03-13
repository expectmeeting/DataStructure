[TOC]

- linux基础文件IO
  - 1、open
  - 2、read
  - 3、write
  - 4、lseek
  - 5、close

# linux基础文件IO

这里记录的是Linux中基础的IO函数，因为Linux中一切皆文件，因此这些基础IO函数能对文件描述符、套接字等使用。

## 1、open
**头文件：**

``` C++
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
```
**函数：**

```
 int open(const char *pathname, int flags);  
 int open(const char *pathname, int flags, mode_t mode);  
 int creat(const char *pathname, mode_t mode);
```
**功能：** 打开或创建一个文件，creat函数只能创建文件。  

**参数1-pathname:** 文件名或带路径的文件名。
**参数2-flags:** 指定文件的打开方式(只读：O_RDONLY;只写:O_WRONLY;读写：O_RDWR),以及该函数具体的运行方式，如阻塞或非阻塞(建议非阻塞)。  

- O_SYNC:文件被打开为同步I/O，对其进行写操作(write)，将保证数据写入底层存储设备才返回。
- O_APPEND:写入的所有数据将被追加到文件的末尾。
- O_CREAT:打开文件，如果文件不存在则建立文件。
- O_TRUNC:如果文件存在，且是普通文件，并且有写权限，将文件内容清空。  

**参数3-mode:** 声明该文件的文件访问权限。  

**返回值：** 

- -1 :打开或创建文件描述符出错，并将errno设置成恰当的错误描述符(枚举类型)。
- 小的整数:函数调用成功。  
## 2、read
**头文件：**
``` C++
 #include <unistd.h>
```
**函数：**

```
ssize_t pread(int fildes, void *buf, size_t nbyte, off_t offset);
ssize_t read(int fildes, void *buf, size_t nbyte);
```
**功能：** 从fildes指向的文件读取nbyte字节的数据到buf指向的内存中，读取成功后fildes的文件位置指针会向前移动。  

**参数1-fildes:** 文件描述符。  

**参数2-buf:** 存储读取的数据。 

**参数3-nbyte:** 读取的字节数。  

**返回值：**  

- -1 :读取文件出错，并将errno设置成恰当的错误描述符(枚举类型)。
- 小的整数(>=0):读取文件成功，共读取n个字节。  
## 3、write  
**头文件：**
``` C++
 #include <unistd.h>
```
**函数：**

```
ssize_t pwrite(int fildes, const void *buf, size_t nbyte,off_t offset);
ssize_t write(int fildes, const void *buf, size_t nbyte);
```
**功能：** 将buf指向的内存中前nbyte个字节的数据写入到fildes指向的文件中。  

**参数1-fildes:** 文件描述符。 

**参数2-buf:** 存储将写入文件的数据。 

**参数3-nbyte:** 写入的字节数。  

**参数4-offset:** pwrite()函数的文件指针不会随写入的数据而增加，因此需设置每次写入的偏移值。  

**返回值：**  

- -1 :读取文件出错，并将errno设置成恰当的错误描述符(枚举类型，如EAGAIN)。
- 小的整数(>=0):写文件成功，共写入n个字节的数据。  

**重点注意：** write()函数只是将数据从用户空间的缓存区拷贝到了内核空间的缓冲区，但并没有立即把数据写入磁盘，这称为**延迟写**。如果在数据写入磁盘前系统崩溃了，则数据就丢失了，因此linux系统也支持强制文件立即写入磁盘(sync(),fsync(),fdatasync())。 
## 4、lseek
``` C++
 #include <unistd.h>
```
**函数：**
```
off_t lseek(int fildes, off_t offset, int whence);
```
**功能：** 改变read/write中的文件偏移值。 
**参数1-fildes:** 文件描述符。  

**参数2-offset:** 偏移量。 
**参数3-whence:** 起始偏移位置。  

- SEEK_SET:起始位置
- SEEK_CUR:当前位置
- SEEK_END:末尾位置  

**返回值：**  
- 0 :成功关闭文件描述符。
- -1:关闭失败，并将errno设置成恰当的错误描述符(枚举类型，如EINTR)。 
## 5、close
``` C++
 #include <unistd.h>
```
**函数：**

```
int close(int fildes);
```
**功能：** 关闭文件描述符，使其可重用。  

**参数1-fildes:** 文件描述符。
**返回值：**  

- 整数:返回从文件开始以字节为单位的偏移量。
- -1:改变偏移值失败。 