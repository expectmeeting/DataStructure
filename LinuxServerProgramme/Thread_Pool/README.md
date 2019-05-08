# 线程池的实现

## 1、什么是线程池？

**线程池**:一个保存了一定数量线程的资源池，系统提前申请好空间和资源，在没有任务时一直阻塞，有任务时则抽调线程去执行，直到线程池被销毁。

## 2、为什么要使用线程池？

对于高并发而完成任务时间短的服务而言，采用一任务一线程的模式会消耗大量的系统资源，因为创建线程和销毁线程的花销比处理任务的花销还大，这会造成性能瓶颈。而线程池技术则

## 3、应用场景

- 高并发且完成任务时间短，如web服务器完成网页请求
- 要求迅速响应
- 突发性大量请求

## 4、线程池的实现

线程池是通过一个结构体类型threadpool_t实现，其定义如下：

~~~c
typedef struct threadpool{
	pthread_mutex_t m_mutex;
	pthread_cond_t	m_cond;
	int m_num;
	threadpool_status_t m_run;
	threadpool_queue_t m_task;
	pthread_t *m_head;
}threadpool_t;
~~~

其通过互斥锁和条件变量来实现任务的同步，与之有关的函数如下：

~~~c
//初始化，创建工作线程
int threadpool_init(threadpool_t *tp, int num);
//添加任务
int threadpool_add(threadpool_t *tp, threadpool_task_t *task);
//销毁线程，使其可以重新初始化
int threadpool_destory(threadpool_t *tp);
~~~

结构体threadpool_task_t中包含函数指针和参数指针，其定义如下：

~~~c
typedef struct threadpool_task{
	void *(*m_fp)(void *);//function point
	void *m_arg;//argument
}threadpool_task_t;
~~~

