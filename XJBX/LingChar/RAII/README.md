## RAII

RAII（Resource Acquisition Is Initialization）,使用局部对象来管理资源,存储在栈的对象，它的生命周期是由操作系统来管理的，无需人工介入.

### 原理

资源的获取往往比较简单，但是释放的时机却比较容易被遗忘，自动的释放销毁就算是一个需求了。

设计一个类的时候，在main函数中声明一个局部对象，会自动调用构造函数进行对象的初始化，当整个main函数执行完成后，自动调用析构函数来销毁对象，整个过程无需人工介入，由操作系统自动完成。于是一些资源的封装也可以考虑这样。

所以可以参考一下四步骤：

1. 设计类封装
2. 构造函数初始化
3. 析构函数设计销毁逻辑
4. 使用的时候就直接声明一个类

### 应用

首先可以考虑的是mutex的封装

```
#include <exception>
#include <pthread.h>
#include <semaphore.h>

class locker
{
public:
    locker()
    {
        if( pthread_mutex_init( &m_mutex, NULL ) != 0 )
        {
            throw std::exception();
        }
    }
    ~locker()
    {
        pthread_mutex_destroy( &m_mutex );
    }
    bool lock()
    {
        return pthread_mutex_lock( &m_mutex ) == 0;
    }
    bool unlock()
    {
        return pthread_mutex_unlock( &m_mutex ) == 0;
    }

private:
    pthread_mutex_t m_mutex;
};
```

使用的时候直接声明一个类就可以了，然后用里面的lock和unlock。

甚至还可以继续用类封装lock，参考unique_lock。
