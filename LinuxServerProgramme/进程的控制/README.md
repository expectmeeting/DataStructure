# 进程的控制

- 一个进程绑定一个核
- 多进程绑定一个端口(accept函数不会引起惊群，因为三次握手中连接队列与进程无关；epoll_wait会引起惊群)

## 1、理解fork()

调用一次fork()，其对应两次返回值。由于其作用是创建一个子进程，因此其子进程返回值为0，父进程返回值为大于0。



## 2、写时复制，copy on write

## 3、绑定CPU

~~~c
int num = sysconf(_SC_NPROCESSORS_CONF);//CPU数量
~~~



- linux命令htop可以查看cpu