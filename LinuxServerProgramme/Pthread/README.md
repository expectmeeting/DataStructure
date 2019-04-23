# linux下多线程

linux下的多线程遵循POSIX线程接口，参考[大神博客](https://computing.llnl.gov/tutorials/pthreads/)

**头文件：**

~~~c
#include<pthread.h>
~~~

## 1、创建线程

~~~c
int pthread_create(pthread_t * thread, pthread_attr_t * attr, void * (*start_routine)(void *), void * arg);
//参数-thread:指向新创建线程的线程ID的指针类型，线程ID是不透明的对象，因此不能靠==来比较两个线程ID。
//参数-attr  :指定各种不同的线程属性(采用函数初始化pthread_attr_init和销毁pthread_attr_destroy )，若为NULL则为默认属性()
//参数-start_routine:新创建的线程从start_routine函数的地址开始运行。
//参数-arg：指向参数的指针，若参数不止一个，则将这些参数放到一个结构中，然后把这个结构的地址作为arg的参数传入。
//返回： 0 -> 创建成功 ； -1 -> 创建失败，并设置errno
~~~

## 2、结束线程

结束线程有多种方式：

- 从线程的起始程序start_routine返回
- 在创建的线程中调用pthread_exit()，主线程调用pthread_exit()后结束线程，但若存在其他线程则进程不会结束
- 另外的线程调用pthread_cancal()来结束当前线程
- 调用exit()结束进程，则线程也被结束
- main()完成，且没有调用pthread_exit()

~~~c
void pthread_exit(void *retvalue);
//结束线程，当线程状态为joinable则可通过retvalue返回值，另外的线程可通过pthread_join()获取该值

void exit(int status);//需含头文件#include<stdlib.h>
//结束进程，并将status值返回给父进程
~~~

## 3、回收线程

线程所占用的资源在线程结束后并不返还给操作系统，而是仍属于进程所有，一个joinable状态的线程，其所占有的资源在对其执行pthread_join后才会释放，为了防止内存泄漏，一个线程要么是detached状态，要么是joinable状态并对其调用pthread_join。

~~~c
int pthread_join(pthread_t thread, void **retval);
//调用该函数的函数会一直阻塞，直到线程ID对应的线程结束
//参数-thread:线程ID
//参数—retval：若不为空，则会拷贝线程ID对应线程的返回值(pthread_exit)
//返回： 0 -> 执行成功 
~~~

## 4、线程分离

分离的(detached)线程在结束后，由系统自动回收资源

分离线程的几种方式：

- 在创建线程时，设置其属性

  ~~~c
  pthread_attr_t attr;//第一步，创建pthread_attr_t对象
  pthread_attr_init(&attr);//第二步，初始化
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);//第三步，设置线程分离状态PTHREAD_CREATE_DETACHED
  ...//创建线程时，attr作为第二个参数
  pthread_attr_destroy(&attr);//第四步，释放库函数资源
  ~~~

- 在创建的线程中，调用pthread_detach(pthread_self());

- 在主线程创建线程后，调用pthread_detach(pid);//pid为线程ID

## 5、栈管理

POSIX标准没有指定线程中栈空间的具体大小，不同的架构、CPU数量和内存大小都会影响线程中栈空间的大小。因此显示的设置线程栈空间的大小有利于程序安全和稳定。

~~~c
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
//功能：设置创建线程时栈分配的最小大小
//参数-attr:pthread_attr_t类型的变量，管理线程属性
//参数-stacksize:线程栈空间大小，单位是字节
//返回： 0 -> 成功  非0 -> 错误代码

int pthread_attr_getstacksize(pthread_attr_t *attr, size_t *stacksize);
//功能：得到线程栈的大小
//参数-attr:pthread_attr_t类型的变量，管理线程属性
//参数-stacksize:传入size_t类型对象的地址，调用结束后将size_t类型对象置为线程栈空间大小，单位是字节
//返回： 0 -> 成功  非0 -> 错误代码

int pthread_attr_setstack(pthread_attr_t *attr, void *stackaddr, size_t stacksize);
//功能：设置创建线程时栈空间的地址和分配的最小大小
//参数-attr:pthread_attr_t类型的变量，管理线程属性
//参数-stackaddr:线程栈空间的地址
//参数-stacksize:线程栈空间大小，单位是字节
//返回： 0 -> 成功  非0 -> 错误代码

int pthread_attr_getstack(pthread_attr_t *attr, void **stackaddr, size_t *stacksize);
//功能：设置创建线程时栈空间的地址和分配的最小大小
//参数-attr:pthread_attr_t类型的变量，管理线程属性
//参数-stackaddr:传入void *类型对象的地址，调用结束后将void *类型对象置为线程栈空间的地址
//参数-stacksize:传入size_t类型对象的地址，调用结束后将size_t类型对象置为线程栈空间大小，单位是字节
//返回： 0 -> 成功  非0 -> 错误代码
~~~

## 6、其他函数

~~~c
pthread_t pthread_self(void);
//功能：返回当前线程的线程ID

int pthread_equal(pthread_t t1, pthread_t t2);
//功能：比较两个线程ID
//返回： 非0 -> 相等 ； 0 -> 不相等
~~~

## 7、函数列表

~~~c
pthread_atfork 
pthread_attr_destroy 
pthread_attr_getdetachstate 
pthread_attr_getguardsize 
pthread_attr_getinheritsched 
pthread_attr_getschedparam 
pthread_attr_getschedpolicy 
pthread_attr_getscope 
pthread_attr_getstack 
pthread_attr_getstackaddr 
pthread_attr_getstacksize 
pthread_attr_init 
pthread_attr_setdetachstate 
pthread_attr_setguardsize 
pthread_attr_setinheritsched 
pthread_attr_setschedparam 
pthread_attr_setschedpolicy 
pthread_attr_setscope 
pthread_attr_setstack 
pthread_attr_setstackaddr 
pthread_attr_setstacksize 
pthread_barrier_destroy 
pthread_barrier_init 
pthread_barrier_wait 
pthread_barrierattr_destroy 
pthread_barrierattr_getpshared 
pthread_barrierattr_init 
pthread_barrierattr_setpshared 
pthread_cancel 
pthread_cleanup_pop 
pthread_cleanup_push 
pthread_cond_broadcast 
pthread_cond_destroy 
pthread_cond_init 
pthread_cond_signal 
pthread_cond_timedwait 
pthread_cond_wait 
pthread_condattr_destroy 
pthread_condattr_getclock 
pthread_condattr_getpshared 
pthread_condattr_init 
pthread_condattr_setclock 
pthread_condattr_setpshared 
pthread_create 
pthread_detach 
pthread_equal 
pthread_exit 
pthread_getconcurrency 
pthread_getcpuclockid 
pthread_getschedparam 
pthread_getspecific 
pthread_join 
pthread_key_create 
pthread_key_delete 
pthread_kill 
pthread_mutex_destroy 
pthread_mutex_getprioceiling 
pthread_mutex_init 
pthread_mutex_lock 
pthread_mutex_setprioceiling 
pthread_mutex_timedlock 
pthread_mutex_trylock 
pthread_mutex_unlock 
pthread_mutexattr_destroy 
pthread_mutexattr_getprioceiling 
pthread_mutexattr_getprotocol 
pthread_mutexattr_getpshared 
pthread_mutexattr_gettype 
pthread_mutexattr_init 
pthread_mutexattr_setprioceiling 
pthread_mutexattr_setprotocol 
pthread_mutexattr_setpshared 
pthread_mutexattr_settype 
pthread_once 
pthread_rwlock_destroy 
pthread_rwlock_init 
pthread_rwlock_rdlock 
pthread_rwlock_timedrdlock 
pthread_rwlock_timedwrlock 
pthread_rwlock_tryrdlock 
pthread_rwlock_trywrlock 
pthread_rwlock_unlock 
pthread_rwlock_wrlock 
pthread_rwlockattr_destroy 
pthread_rwlockattr_getpshared 
pthread_rwlockattr_init 
pthread_rwlockattr_setpshared 
pthread_self 
pthread_setcancelstate 
pthread_setcanceltype 
pthread_setconcurrency 
pthread_setschedparam 
pthread_setschedprio 
pthread_setspecific 
pthread_sigmask 
pthread_spin_destroy 
pthread_spin_init 
pthread_spin_lock 
pthread_spin_trylock 
pthread_spin_unlock 
pthread_testcancel
~~~

