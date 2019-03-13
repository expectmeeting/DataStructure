# linux高级I/O函数解析
这里对高级I/O函数进行解析。  
- 用于创建文件描述符的函数：pipe、dup/dup2
- 用于读写数据的函数：readv/writev、sendfile、mmap/munmap、splice和tee
- 用于控制I/O行为和属性的函数：fcntl  
## pipe
- pipe:创建一个管道来实现进程间通信  
``` C++
//函数定义
#include<unistd.h>
int pipe(int fd[2]);
```
- 返回0：表示函数执行成功，并将一对打开的文件描述符值填入形参数组中，fd[0]只能用于从管道中读取数据，fd[1]只能用于从管道中写入数据。
- 返回-1并设置errno，表示函数执行失败。
- pipe函数创建的管道是单向的，因此要实现双向通信需要建立两个管道，需要注意的是管道内部传输的数据是字节流，且管道容量为65536字节。
- 默认情况下：这对文件描述符都是阻塞的，即管道内没有数据却要读取数据时，读取函数将被阻塞，直至管道有数据可读，或向一个满的管道写入数据时，写入函数将被阻塞，直至管道有足够的空间。
- 非默认情况下：将fd[0]和fd[1]都设置为非阻塞的；  
    - 当fd[1]的引用计数减少至0，即没有进程需要写入数据，则对fd[0]的read操作将返回0。
    - 当fd[0]的引用计数减少至0，即没有进程需要读取数据，则对fd[1]的write操作将失败，并引发SIGPIPE信号。
## socketpair
socketpair:创建双向管道
``` C++
#include<sys/types.h>
#include<sys/socket.h>
int socketpair(int domain,int type, int protocol,int fd[2]);
```
- 前三个参数与socket函数的三个参数完全相同，但domain只能使用AF_UNIX。
- 函数执行成功返回0，失败返回-1并设置errno。
## dup函数和dup2函数
- 作用：将标准输入重定向到一个文件或一个网络连接。  
``` C++
#include<unistd.h>
int dup(int file_description);
int dup2(int file_description_one, int file_description_two);
```
## readv和writev
**readv:** 将文件描述符中的数据读到分散的内存块中。  

**writev:** 将分散的内存块中的数据集中写到文件描述符中。  

``` C++
#include <sys/uio.h>
ssize_t readv(int fildes, const struct iovec *iov, int iovcnt);
ssize_t writev(int fildes, const struct iovec *iov, int iovcnt);
```
**参数1-fildes:** 文件描述符。 

**参数2-iov:** 指向iovec类型的指针，该类型含有具体的buf地址和大小。  

**参数3-iovcnt:** 分散内存块的数量。
**返回值:** 成功则返回字节数，失败则返回-1并设置errno。

## sendfile  
**功能：** 在内核中从一个文件描述符传输数据到另一个文件描述符，避免内核缓冲区和用户缓冲区之间的数据拷贝，提升效率。  

``` C++
#include <sys/sendfile.h>
ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
```
**参数1-out_fd:** 待写入的的文件描述符(必须是一个socket)。 

**参数2-in_fd:** 待读取的的文件描述符(必须是真实的文件，而不是socket和管道)。  

**参数3-offset:** 指定从读入文件流的哪个位置开始读(NULL则默认为起始位置)。 

**参数4-count:** 读取的字节数。 

**返回值:** 成功则返回字节数，失败则返回-1并设置errno。  

## mmap和munmap  
**功能：** 申请一段内存空间(用户虚拟内存)，并将这段内存作为进程间通信的共享内存，也可以将文件(实际物理内存)直接映射到其中。  

```
 #include <sys/mman.h>
void *mmap(void *addr, size_t len, int prot, int flags, int fildes, off_t off);  
int munmap(void *addr, size_t len);
int msync(void *addr, size_t len, int flags);//将改动的数据直接写入磁盘
```
**参数1-addr:** 欲映射的内存起始地址，常设置为NULL，由系统自行选定地址，映射成功后返回该地址。 

**参数2-len:** 映射空间的大小，建议为4K的整数倍，因为内存按页进行管理，页大小为4K。  

**参数3-prot:** 映射区域的保护方式。  

- PROT_READ : 内存段可读。
- PROT_WRITE : 内存段可写。
- PROT_EXEC : 内存段可执行。
- PROT_NONE :内存段不能被访问。  

**参数4-flags:** 控制内存段内容被修改后程序的行为。

- MAP_SHARED：与其它所有映射这个对象的进程共享映射空间。对共享区的写入，反映到被映射的文件中(内核)。直到msync()或者munmap()被调用，改动的文件才从内核缓存中写入磁盘。  
- MAP_PRIVATE：建立一个写入时拷贝的私有映射。内存区域的写入不会影响到原文件。这个标志和以上标志是互斥的，只能使用其中一个。  
- MAP_ANONYMOUS:这段内存不是从文件映射而来。其内容被初始化为全0。这种情况下，mmap函数的最后两个参数将被忽略。  
- MAP_FIXED:内存段必须位于addr参数指定的地址处，且必须是内存页面大小4K的整数倍。  
- MAP_HUGETLB:安装大内存页面来分配内存空间。  

**参数5-fildes:**  文件描述符。  

**参数6-off:** 偏移值。 

## splice  
功能：在两个文件描述符之间移动数据，零拷贝操作。  
``` C++
#include <fcntl.h>
ssize_t splice(int fd_in, loff_t *off_in, int fd_out, loff_t *off_out, size_t len, unsigned int flags);
```
**参数1-fd_in:** 待输入数据的文件描述符，如果是管道文件描述符，则off_in必须为NULL。  

**参数2-off_in:** 输入数据流的偏移。 

**参数3-fd_out:** 待输出数据的文件描述符，如果是管道文件描述符，则off_out必须为NULL。 

**参数4-off_out:** 输出数据流的偏移。 

**参数5-len:** 数据的长度。 

**参数6-flags:** 控制数据如何移动。  

- SPLICE_F_MOVE :内存按整页移动数据。  
- SPLICE_F_NONBLOCK:非阻塞的splice操作。  
- SPLICE_F_MORE:给内核的提示。