# linux中锁和条件变量的使用

## 锁

锁是多进程和多线程中进行同步和保护共享数据的一种机制。当多进程/多线程对共享数据都按锁的机制进行访问时，即按同一种数据访问规则，能够保证同一时刻都只有一个进程/线程访问临界资源。当一个进程/线程已经加锁，则其余进程/线程会阻塞在加锁函数处。

- 线程同步：在同一时刻只有一个线程可以对临界资源进行操作，而其余线程均不能对其进行操作，并处于等待状态。

### 1、锁的类型

- 普通锁(PTHREAD_MUTEX_NORMAL):该类型不能发现死锁。一个线程如果对一个已经加锁的普通锁再次加锁，将引发死锁；对一个已经被其他线程加锁的普通锁解锁，或者对一个已经解锁的普通锁再次解锁，将导致不可预期的后果；
- 检错锁(PTHREAD_MUTEX_ERRORCHECK)：该类型将提供错误检查。一个线程如果对一个已经加锁的检错锁再次加锁，将返回一个错误；对一个已经被其他线程加锁的检错锁解锁，或者对一个已经解锁的检错锁再次解锁，将返回一个错误；
- 嵌套锁(PTHREAD_MUTEX_RECURSIVE)：该锁允许一个线程在释放锁前多次对其加锁，而不发生死锁。但其他线程要使用前必须执行相同次数的解锁操作。对一个已经被其他线程加锁的纠错嵌套锁，或者对一个已经解锁的嵌套锁再次解锁，将返回一个错误；
- 默认锁(PTHREAD_MUTEX_DEFAULT)：一个线程如果对一个已经加锁的默认锁再次加锁，或者虽一个已经被其他线程加锁的默认锁解锁，或者对一个解锁的默认锁解锁，将导致不可预期的后果。

### 2、创建和初始化

锁的初始化共有两种方式：

- 静态初始化：

  ~~~C
  #include<pthread.h>
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//普通锁
  
  ~~~

- 动态初始化：

  ~~~c
  //第一种
  pthread_mutex_t mutex；
  pthread_mutex_init(&mutex, NULL);//默认初始化
  
  //第二种
  pthread_mutexattr_t attr;
  phtread_mutexattr_settype(&attr,PTHREAD_MUTEX_NORMAL)//设置属性中锁的类型
  pthread_mutex_init(&mutex, &attr);//初始化
  ~~~

### 3、加锁和解锁

~~~c
int pthread_mutex_lock(pthread_mutex_t *mutex);
//功能：锁定mutex指定的锁，若mutex已经被其它进程/线程加锁，则该函数会阻塞
//参数-mutex:锁
//返回：0 -> 成功 ； 非0返回值 -> 错误类型

int pthread_mutex_trylock(pthread_mutex_t *mutex);
//功能：锁定mutex指定的锁，若mutex已经被其它进程/线程加锁，则该函数会返回错误，而不阻塞线程。
//参数-mutex:锁
//返回：0 -> 成功 ； 非0返回值 -> 错误类型

int pthread_mutex_unlock(pthread_mutex_t *mutex);
//功能：解锁mutex指定的锁
//参数-mutex:锁
//返回：0 -> 成功 ； 非0返回值 -> 错误类型

~~~

### 4、释放

~~~c
int pthread_mutex_destroy(pthread_mutex_t *mutex);
//功能：销毁一个未加锁的mutex指定的锁对象，让其变成未初始化，使其可以重新初始化。
//参数-mutex:锁
//返回：0 -> 成功 ； 非0返回值 -> 错误类型
~~~

## 条件变量

条件变量是线程中另外一种同步机制，需要与互斥锁配合使用(互斥锁类型需是普通锁)。由于互斥锁只有两种状态，加锁(locked)和解锁(unlock)，导致所有需操作临界资源的线程都参与竞争，而条件变量则能避免一些需要满足条件才执行操作的线程进行的无意义且消耗资源的竞争。

例如：生产者和消费者场景中，当生产的数量没有达到消费的界限(如0)，生产者线程和消费者线程都会被唤醒，参与对临界资源的竞争，而消费者线程这时竞争成功，但由于不满足消费条件(消费品的数量为0)，只能继续挂起，等待下一次竞争。而这无疑造成了系统资源的浪费。

### 1、条件变量的初始化

- 静态初始化：

  ~~~c
  pthread_cond_t myconvar = PTHREAD_COND_INITIALIZER;
  ~~~

- 动态初始化：

  ~~~C
  pthread_cond_t myconvar；
  pthread_cond_init(&myconvar, NULL);
  
  //函数原型：
  int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
  //功能：初始化一个条件变量。
  //参数-cond:条件变量
  //参数-attr：条件变量属性，若为NULL，则使用默认属性
  //返回：0 -> 成功 ； 非0返回值 -> 错误类型
  ~~~

### 2、等待信令和发送信令

~~~c
int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
//功能：使用该函数前需加锁，然后调用该函数后，会将对互斥锁解锁，并进入阻塞状态，等待信令通知。注意的是解锁和进入阻塞状态是原子操作。接收到信令后，唤醒线程，并尝试加锁，加锁成功才返回。
//参数-cond:条件变量
//参数-mutex：锁
//返回：0 -> 成功 ； 非0返回值 -> 错误类型

int pthread_cond_timewait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime);
//功能：使用该函数前需加锁，然后调用该函数后，会将对互斥锁解锁，并进入阻塞状态，等待信令通知。注意的是解锁和进入阻塞状态是原子操作。接收到信令后，唤醒线程，并尝试加锁，加锁成功才返回。
//参数-cond:条件变量
//参数-mutex：锁
//参数-abstime：等待时间(绝对时间)
//返回：0 -> 成功 ； 非0返回值 -> 错误类型

int pthread_cond_signal(pthread_cond_t *cond);
//功能：唤醒至少一个因条件变量阻塞的线程。若没有因条件变量阻塞的线程，不影响调用线程的执行。
//参数-cond:条件变量
//返回：0 -> 成功 ； 非0返回值 -> 错误类型

int pthread_cond_broadcast(pthread_cond_t *cond);
//功能：唤醒所有因条件变量阻塞的线程。若没有因条件变量阻塞的线程，不影响调用线程的执行。
//参数-cond:条件变量
//返回：0 -> 成功 ； 非0返回值 -> 错误类型
~~~

### 3、释放条件变量

~~~c
int pthread_cond_destroy(pthread_cond_t *cond);
//功能：销毁一个cond指定的条件变量对象，让其变成未初始化，使其可以重新初始化。
//参数-cond:条件变量
//返回：0 -> 成功 ； 非0返回值 -> 错误类型
~~~

### 4、QA

**Q：**条件变量和互斥锁为什么要结合使用？

**A：**防止race-condition，即**条件竞争**。先解释下含义：

- **竞争**：计算机运行过程中，并发、无序、大量的进程在使用有限、独占、不可抢占的资源，由于进程无限，资源有限，产生矛盾，这种矛盾称为竞争。
- **条件竞争**：由于两个或者多个进程竞争使用不能被同时访问的资源，使得这些进程有可能因为时间上推进的先后原因而出现问题，这叫做竞争条件。

不加锁的话，当线程A调用pthread_cond_wait()，由于指令执行需要时间，线程A无法立即进入阻塞状态，而这时另一线程B调用pthread_cond_signal()发送信令，因为线程A还未进入阻塞状态，因此该信令没有起到唤醒线程作用。而加锁之后，当线程A还未进入阻塞状态时，其余线程无法对临界资源进行操作，因此不存在需要发送信令的线程，也就不会导致信令丢失。

**Q：**pthread_cond_wait()内部机制中，解锁和进入阻塞状态为什么是原子操作？

**A：**先解释**原子操作**的含义：

- **原子操作**：不会被线程调度机制打断的操作，要么一次执行完成，要么不执行。

若解锁和进入阻塞状态不是原子操作，解锁后和进入阻塞状态前的间隔，存在与前一问一样的问题，可能会丢失信令。

**Q：**虚假唤醒(spurious wakeup)?

**A：**虚假唤醒是说，即使没有线程调用pthread_cond_signal()或pthread_cond_broadcast()，调用的pthread_cond_wait()函数也可能返回。这是因为在多核操作系统中，完全避免虚假唤醒会极大的影响条件变量的执行效率，且出现虚假唤醒的条件又极为苛刻，因此并没有对其进行修正。而需要我们在代码编写时使用while取代if，避免因为虚假唤醒而执行相应操作。**需要注意**的是pthread_cond_wait()返回而条件不满足的原因不止是虚假唤醒这一个。

~~~c
while(条件不满足){  
   pthread_cond_wait(&cond, &mutex);  
}  
//而不是:  
if(条件不满足){  
   pthread_cond_wait(&cond, &mutex);  
}   
~~~

Q：线程signal()和unlock()先后顺序的区别?

A：(1)第一种，先signal()后unlock()

~~~c
pthread_mutex_lock(&mutex);
...
pthread_cond_signal(&cond);
pthread_mutex_unlock(&mutex);
~~~

某些操作系统中，在线程A执行signal()后，为了减少延迟，会立即将上下文切换到被唤醒的线程B中，而这时由于A还没有解锁，因此B中wait()尝试进行的加锁操作不会成功，使其继续阻塞，从而使上下文重新切换到A中，在A中进行解锁操作后，B才得以执行后面的操作。而这中间的来回切换就影响程序的执行效率，而为了弥补这种缺陷，某些实现采用了**等待转移**(waitmorphing)的优化措施。这种措施是采用两个队列，分别是cond_wait队列和mutex_wait队列，即当锁被其它线程持有，但接收到信令时，直接将线程从cond_wait队列移动到mutex_wait队列，而无需上下文切换，从而避免性能损耗。

因此，采用这种方式有可能存在性能损耗，但可以避免隐藏的bug。建议选择第一种方式。

（2）第二种，先unlock()后signal()

~~~c
pthread_mutex_lock(&mutex);
...
pthread_mutex_unlock(&mutex);
pthread_cond_signal(&cond);
~~~

按这种顺序执行，可能会因为条件竞争，而导致pthread_cond_wait()返回而条件不满足。例如，消费者线程A和B都阻塞在pthread_cond_wait()处，这时生产者线程C发出信令signal()，线程A返回并执行完相关操作解锁后，切换到线程C，C再次进行lock()和unlock()的操作，在未执行signal()前线程切换到线程A，A执行lock()后判断条件为真执行相关操作后解锁，这时切换回线程C执行signal()，然后线程切换到B，这时pthread_cond_wait()返回但条件不满足。

因此，采用这种方式虽然不存在性能损耗，但可能存在隐藏的bug。

## 示例

~~~c
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int i = 0;

void *thread1(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		++i;
		if(i%10 == 0){
			pthread_cond_signal(&cond);
		}
		pthread_mutex_unlock(&mutex);
		
		usleep(100*1000);
	}
}

void *thread2(void *arg){
	while(1){
		pthread_mutex_lock(&mutex);
		++i;
		if(i%10 == 0){
			pthread_cond_signal(&cond);
		}
		pthread_mutex_unlock(&mutex);
		
		usleep(100*1000);
	}
}

void *thread3(void *arg){
	int temp = 0;
	while(1){
		pthread_mutex_lock(&mutex);
		if(temp != i){
			while(i%10 != 0){
				pthread_cond_wait(&cond, &mutex);
			}
			printf("thread ID: 3, i = %d\n", i);
			temp = i;
		}	
		pthread_mutex_unlock(&mutex);
		usleep(100*1000);
	}
}

void *thread4(void *arg){
	int temp = 0;
	while(1){
		pthread_mutex_lock(&mutex);
		if(temp != i){
			while(i%10 != 0){
				pthread_cond_wait(&cond, &mutex);
			}
			printf("thread ID: 4, i = %d\n", i);
			temp = i;
		}	
		pthread_mutex_unlock(&mutex);
		usleep(100*1000);
	}
}

int main()
{
	pthread_t pid1;
	pthread_t pid2;
	pthread_t pid3;
	pthread_t pid4;
	
	pthread_create(&pid1,NULL,thread1,(void *)NULL);
	pthread_detach(pid1);
	pthread_create(&pid2,NULL,thread2,(void *)NULL);
	pthread_detach(pid2);
	
	pthread_create(&pid3,NULL,thread3,(void *)NULL);
	pthread_detach(pid3);
	pthread_create(&pid4,NULL,thread4,(void *)NULL);
	pthread_detach(pid4);
	
	pthread_exit(NULL);
	return 0;
}
~~~

