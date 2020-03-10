# 内核同步机制分析

## 1、自旋锁

自旋锁是内核中重要的同步机制，后面介绍的同步机制基本都使用了自旋锁。自旋锁顾名思义是指进程在没有申请到锁时，自旋等待(忙等待)，因此适用于临界区短小且不阻塞的情况。

### 定义

~~~c
typedef struct raw_spinlock {
    arch_spinlock_t raw_lock;
#ifdef CONFIG_GENERIC_LOCKBREAK
    unsigned int break_lock;
#endif
#ifdef CONFIG_DEBUG_SPINLOCK
    unsigned int magic, owner_cpu;
    void *owner;
#endif
#ifdef CONFIG_DEBUG_LOCK_ALLOC
    struct lockdep_map dep_map;
#endif
} raw_spinlock_t;

typedef struct spinlock {
    union {
        struct raw_spinlock rlock;

#ifdef CONFIG_DEBUG_LOCK_ALLOC
# define LOCK_PADSIZE (offsetof(struct raw_spinlock, dep_map))
        struct {
            u8 __padding[LOCK_PADSIZE];
            struct lockdep_map dep_map;
        };
#endif
    };
} spinlock_t;
~~~

如果忽略CONFIG_DEBUG_LOCK_ALLOC话，spinlock主要包含一个arch_spinlock_t的结构，从名字可以看出，这个结构是跟体系结构有关的。

### 加锁

|--spin_lock

​     |--_raw_spin_lock

​          |--__raw_spin_lock

~~~c
#define raw_spin_lock(lock) _raw_spin_lock(lock)

static inline void spin_lock(spinlock_t *lock)
{
    raw_spin_lock(&lock->rlock);
}

void __lockfunc _raw_spin_lock(raw_spinlock_t *lock)
{
	__raw_spin_lock(lock);
}

static inline void __raw_spin_lock(raw_spinlock_t *lock)
{
    //禁止抢占
	preempt_disable();
    //调试
	spin_acquire(&lock->dep_map, 0, 0, _RET_IP_);
    //实际工作
	LOCK_CONTENDED(lock, do_raw_spin_trylock, do_raw_spin_lock);
}
~~~

- **禁止抢占的作用**：

  **根本目的是防止死锁**，防止调度器重新调度一个高优先级且访问同一临界区的进程，使得当前进程始终无法释放自旋锁，从而死锁。

  调度时机：

  - 用户抢占：
    - 从系统调用返回用户空间时
    - 从中断处理程序返回用户空间时
  - 系统抢占：
    - 中断处理程序正在执行，且返回内核空间之前
    - 内核代码再一次具有可抢占性的时间，即不持有锁
    - 内核中的任务显示地调用schedule()
    - 内核中的任务阻塞

- 实际工作流程：

  - 尝试锁（do_raw_spin_trylock），通过回调实现，