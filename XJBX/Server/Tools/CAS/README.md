## CAS

锁看起来是解决并发冲突的万能要是，但是实际上并发问题还能通过一些额外的方式（比如原子操作）来解决

### 什么是CAS

CAS，比较交换，原子操作的一种，⽤于在多线程编程中实现不被打断的数据交换操作，这种方式也能解决多线程的时候因修改执行顺序不确定和一些中断问题导致的脏数据

```c++
// fake code
bool CAS( int * pAddr, int nExpected, int nNew )
 atomically {
     if ( *pAddr == nExpected ) {
          *pAddr = nNew ;
          return true ;
}
     else
         return false ;
}
```

最后的结果会告知原子交换是否成功了。

### 关于操作的原子性问题

实际上可以观察一下`i++`和`++i`的操作,发现这两者其实都不是原子操作

```
gcc -S atomic_test.c
```

i++的操作就是先把i从内存加载到寄存器，之后把寄存器的值+1，最后再把寄存器的值写回内存。在多个线程去做i++的时候，就可能出现自增的结果只出现了一次的情况。实际执行的时候就能看到和期望不一致的情况。

在gcc下提供了一套原子操作API

```
bool __sync_bool_compare_and_swap (type *ptr, type oldval type newva l, ...)
```

具体的实现原理就是X86指令集提供了指令前缀lock⽤于锁定前端串⾏总线FSB，保证了指令执⾏时不会收到其他处理器 的⼲扰。

```
static int lxx_atomic_add(int *ptr, int increment)
{
     int old_value = *ptr;
     __asm__ volatile("lock; xadd %0, %1 \n\t"
 						: "=r"(old_value), "=m"(*ptr)
                       	: "0"(increment), "m"(*ptr)
                       	: "cc", "memory");
     return *ptr;
}
```

使⽤lock指令前缀之后，处理期间对count内存的并发访问（Read/Write）被禁止。

### 自旋锁&互斥&原子操作的比较

测试的代码

```
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <atomic>

#define MAX_THREAD_NUM 2
#define FOR_LOOP_COUNT 2000
#define FOR_ADD_COUNT 100000

static int counter = 0;
static pthread_spinlock_t spinlock;
static pthread_mutex_t mutex;

typedef void *(*thread_func_t)(void *argv);


void do_for_add(int count)
{
    long sum = 0;
    for(int i = 0; i < count; i++)
    {
        sum += i;
    }
}


class atomic_flag_spinlock
{
    std::atomic_flag flag;
public:
    atomic_flag_spinlock():
        flag(ATOMIC_FLAG_INIT)
    {}
    void lock()
    {
        while(flag.test_and_set(std::memory_order_acquire));
    }
    void unlock()
    {
        flag.clear(std::memory_order_release);
    }
};

static atomic_flag_spinlock s_atomic_flag_spinlock;

static int lxx_atomic_add(int *ptr, int increment)
{
    int old_value = *ptr;
    __asm__ volatile("lock; xadd %0, %1 \n\t"
                     : "=r"(old_value), "=m"(*ptr)
                     : "0"(increment), "m"(*ptr)
                     : "cc", "memory");
    return *ptr;
}


void *mutex_thread_main(void *argv)
{
    for (int i = 0; i < FOR_LOOP_COUNT; i++)
    {
        pthread_mutex_lock(&mutex);
        // counter++;
        do_for_add(FOR_ADD_COUNT);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *atomic_thread_main(void *argv)
{
    for (int i = 0; i < FOR_LOOP_COUNT; i++)
    {
        lxx_atomic_add(&counter, 1);
        // counter++;
    }
    return NULL;
}
void *spin_thread_main(void *argv)
{
    for (int i = 0; i < FOR_LOOP_COUNT; i++)
    {
        pthread_spin_lock(&spinlock);
        // counter++;
        do_for_add(FOR_ADD_COUNT);
        pthread_spin_unlock(&spinlock);
    }
    return NULL;
}

void *atomic_flag_spinlock_thread_main(void *argv)
{
    for (int i = 0; i < FOR_LOOP_COUNT; i++)
    {
        s_atomic_flag_spinlock.lock();
        // counter++;
        do_for_add(FOR_ADD_COUNT);
        s_atomic_flag_spinlock.unlock();
    }
    return NULL;
}

int test_lock(thread_func_t func, char **argv)
{
    clock_t start = clock();
    pthread_t tid[MAX_THREAD_NUM] = {0};
    for (int i = 0; i < MAX_THREAD_NUM; i++)
    {
        int ret = pthread_create(&tid[i], NULL, func, argv);
        if (0 != ret)
        {
            printf("create thread failed\n");
        }
    }
    for (int i = 0; i < MAX_THREAD_NUM; i++)
    {
        pthread_join(tid[i], NULL);
    }
    clock_t end = clock();
    printf("spend clock : %ld, ", (end - start) / CLOCKS_PER_SEC);
    return 0;
}

int main(int argc, char **argv)
{
    printf("THREAD_NUM:%d\n\n", MAX_THREAD_NUM);
    counter = 0;
    printf("use mutex ----------->\n");
    test_lock(mutex_thread_main, NULL);
    printf("counter = %d\n", counter);

    // counter = 0;
    // printf("\nuse atomic ----------->\n");
    // test_lock(atomic_thread_main, NULL);
    // printf("counter = %d\n", counter);

    counter = 0;
    printf("\nuse spin ----------->\n");
    pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);
    test_lock(spin_thread_main, NULL);
    printf("counter = %d\n", counter);

    counter = 0;
    printf("\nuse atomic_flag_spinlock ----------->\n");
    test_lock(atomic_flag_spinlock_thread_main, NULL);
    printf("counter = %d\n", counter);

    return 0;
}
```

实际测试在mutex中临界区比较大的时候，做的事务多的时候是比自旋锁有优势的。自旋锁是一种一致忙等待锁的过程，死循环中检测锁的状态，mutex拿不到锁就进入休眠，放在锁的等待队列上。

### 无锁队列的实现

见folder下CAS_que.cpp
