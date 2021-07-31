#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <mutex>
#include <time.h>
#include <atomic>
#include <list>
#include <memory>

#define MAX_THREAD_NUM 1
#define FOR_LOOP_COUNT 10000000
static int counter = 0;
static pthread_spinlock_t spinlock;
static std::mutex s_mutex;
static int s_count_push = 0;
static int s_count_pop = 0;

// using namespace lock_free;
// 有锁队列，直接使用list
static std::list<int> s_list;

typedef void *(*thread_func_t)(void *argv);

static int lxx_atomic_add(int *ptr, int increment)
{
  int old_value = *ptr;
  __asm__ volatile("lock; xadd %0, %1 \n\t"
                   : "=r"(old_value), "=m"(*ptr)
                   : "0"(increment), "m"(*ptr)
                   : "cc", "memory");
  return *ptr;
}

template <typename ElemType>
struct qnode // 链表节点
{
  struct qnode *_next;
  ElemType _data;
};

template <typename ElemType>
class Queue
{
private:
  struct qnode<ElemType> *volatile _head = NULL;  // 随着pop后指向的位置是不一样的, head不是固定的
  struct qnode<ElemType> *volatile _tail = NULL;

public:
  Queue()
  {
    _head = _tail = new qnode<ElemType>;
    _head->_next = NULL;
    _tail->_next = NULL;
    printf("Queue _head:%p\n", _head);
  }
  void push(const ElemType &e)
  {
    struct qnode<ElemType> *p = new qnode<ElemType>;
    p->_data = e;
    p->_next = NULL;

    struct qnode<ElemType> *t = _tail; // 获取尾部

    t->_next = p; // 插到尾部

    _tail = p; // 更新尾部节点
  }

  void push2(const ElemType &e)
  {
    struct qnode<ElemType> *p = new qnode<ElemType>;
    // printf("push head:%p, p:%p\n", _head, p);
    p->_next = NULL;
    p->_data = e;

    struct qnode<ElemType> *t = _tail;
    struct qnode<ElemType> *old_t = _tail;
    int count = 0;
    do
    {
      while (t->_next != NULL) // 非空的时候要去更新 t->_next
        t = t->_next;          // 找到最后的节点
      if (count++ >= 1)
      {
        printf("push count:%d, t->_next:%p\n", count, t->_next);
      }
      // 将null换为p即是插入的节点
    } while (!__sync_bool_compare_and_swap(&t->_next, NULL, p));

    // 将最后的节点_tail更换为p节点
    __sync_bool_compare_and_swap(&_tail, old_t, p);
  }

  bool pop(ElemType &e)
  {
    struct qnode<ElemType> *p = _head;         // 头结点
    struct qnode<ElemType> *np = _head->_next; // 首元素节点
    if (!np)
    {
      return false;
    }
    e = np->_data;

    _head->_next = np->_next;
    delete np;
    return true;
  }

  bool pop2(ElemType &e)
  {
    struct qnode<ElemType> *p = NULL;
    struct qnode<ElemType> *np = NULL;
    int count = 0;
    do
    {
      p = _head; // 头节点，不真正存储数据
      np = p->_next;
      if (p->_next == NULL) // 首元节点为空，则返回
      {
        return false;
      }
      if (count++ >= 1)
      {
        printf("pop count:%d, p->_next:%p\n", count, p->_next);
      }
      // 更新头结点位置
    } while (!__sync_bool_compare_and_swap(&_head, p, p->_next));
    e = p->_next->_data;
    // printf("pop p:%p\n", p);
    delete p; // 因为我们已经将头部节点换成了p->_next, 所以可以释放掉
    return true;
  }

  ~Queue()
  {
    struct qnode<ElemType> *volatile tmp;
    while (_head)
    {
      tmp = _head->_next;
      printf("_head:%p\n", _head);
      delete _head;
      _head = tmp;
    }
  }
};

void *mutex_thread_push(void *argv)
{
  for (int i = 0; i < FOR_LOOP_COUNT; i++)
  {
    s_mutex.lock();
    s_count_push++;
    s_list.push_back(i);
    s_mutex.unlock();
  }
  return NULL;
}

void *mutex_thread_pop(void *argv)
{
  while (true)
  {
    int value = 0;
    s_mutex.lock();
    if (s_list.size() > 0)
    {
      value = s_list.front();
      s_list.pop_front();
      s_count_pop++;
    }
    s_mutex.unlock();
    if (s_count_pop >= FOR_LOOP_COUNT * MAX_THREAD_NUM)
    {
      printf("%s dequeue:%d\n", __FUNCTION__, value);
      break;
    }
  }
  printf("%s exit\n", __FUNCTION__);
  return NULL;
}

static Queue<int> s_queue;
void *queue_free_thread_push(void *argv)
{
  for (int i = 0; i < FOR_LOOP_COUNT; i++)
  {
    s_queue.push2(i);
    lxx_atomic_add(&s_count_push, 1);
    // printf("s_count_push:%d\n",s_count_push);
  }
  return NULL;
}

void *queue_free_thread_pop(void *argv)
{
  // for (int i = 0; i < FOR_LOOP_COUNT*5; i++)
  int last_value = 0;
  static int s_pid_count = 0;
  s_pid_count++;
  int pid = s_pid_count;
  while (true)
  {
    int value = 0;
    if (s_queue.pop2(value))
    {
      last_value = value;
      if (s_count_pop != value)
      {
        printf("pid:%d, -> value:%d, expected:%d\n", pid, value, s_count_pop);
      }
      s_count_pop++;
      // printf("pid:%d, -> value:%d\n", pid, value);
    }
    else
    {
      // printf("pid:%d, null\n", pid);
    }

    if (s_count_pop >= FOR_LOOP_COUNT * MAX_THREAD_NUM)
    {
      printf("%s dequeue:%d\n", __FUNCTION__, last_value);
      break;
    }
  }
  printf("%s exit\n", __FUNCTION__);
  return NULL;
}

int test_queue(thread_func_t func_push, thread_func_t func_pop, char **argv)
{
  clock_t start = clock();
  pthread_t tid_push[MAX_THREAD_NUM] = {0};
  for (int i = 0; i < MAX_THREAD_NUM; i++)
  {
    int ret = pthread_create(&tid_push[i], NULL, func_push, argv);
    if (0 != ret)
    {
      printf("create thread failed\n");
    }
  }
  pthread_t tid_pop[MAX_THREAD_NUM] = {0};
  for (int i = 0; i < MAX_THREAD_NUM; i++)
  {
    int ret = pthread_create(&tid_pop[i], NULL, func_pop, argv);
    if (0 != ret)
    {
      printf("create thread failed\n");
    }
  }

  for (int i = 0; i < MAX_THREAD_NUM; i++)
  {
    pthread_join(tid_push[i], NULL);
  }
  for (int i = 0; i < MAX_THREAD_NUM; i++)
  {
    pthread_join(tid_pop[i], NULL);
  }
  clock_t end = clock();
  printf("spend clock : %ld, push:%d, pop:%d\n", (end - start) / CLOCKS_PER_SEC,
         s_count_push, s_count_pop);
  return 0;
}

// 多尝试几次 g++ -o CAS_que.cpp -lpthread -latomic -std=c++11
int main(int argc, char **argv)
{
  printf("THREAD_NUM:%d\n\n", MAX_THREAD_NUM);

  for (int i = 0; i < 100; i++)
  {
    s_count_push = 0;
    s_count_pop = 0;
    printf("\n\n--------->i:%d\n\n", i);
    printf("use mutex queue ----------->\n");
    test_queue(mutex_thread_push, mutex_thread_pop, NULL);

    s_count_push = 0;
    s_count_pop = 0;

    printf("\nuse queue_free queue ----------->\n");
    test_queue(queue_free_thread_push, queue_free_thread_pop, NULL);
  }

  printf("finish\n");
  return 0;
}
