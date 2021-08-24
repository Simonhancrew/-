## 智能指针（shared_ptr）

### 0 资源管理

C++ 里也是有垃圾回收的，不过不是 Java、Go 那种严格意义上的垃圾回收，而是广义上的垃圾回收，这就是构造 / 析构函数和 RAII 惯用法（Resource Acquisition Is Initialization）。把裸指针包装起来，在构造函数里初始化，在析构函数里释放。这样当对象失效销毁时，C++ 就会自动调用析构函数，完成内存释放、资源回收等清理工作。

智能指针就是对此的一层替你干活的封装，因为重载了 * 和 -> 操作符，用起来和原始指针一模一样。

常用的有两种智能指针，分别是 unique_ptr 和 shared_ptr。

### 1 传统指针的存在什么问题

C++的特点之一就是很相信程序员本身的能力，也因为这个原因在指针的问题上如果你没有处理好，会带来很多的问题

你在堆上创建了一个对象的时候，系统就会把这个对象的生命周期交由你来管理，用完之后不想栈区的会自动销毁，这个对象的生命需要由你来手动结束。由此引出两个问题 ，什么时候释放？在哪里释放？

这两个问题没有处理好的话就会导致内存泄露或者程序的崩溃。

```
//泄露
{
	string *s1 = new string("hello");
	string *s2 = new string("hc 我是你爸爸");
}//离开段，没有释放，内存泄漏 

{
	string *s1 = new string("haah");
	delete s1;
	
	
	//delete again
	delete S1;
}//重复的delete会引发panic导致程序崩溃
```

在这种代码量小，一个作用域的时候，这种问题很容易被看出来，但是当项目更大的时候，逻辑更为复杂的情况下，这种问题是很难被排查出来的（某个作用域的对象可能要到另外一份代码的逻辑中释放）。

### 2  智能指针如何解决这个问题的

问题的核心在于引用计数

1. 从堆上申请一个资源的时候就创建一个智能指针，这个指针指向这个资源，这个智能指针的内部会存在一个引用计数器，所有后来的指向该资源的对象都共享这个计数器，引用计数唯一
2. 后续的赋值给其他智能指针的时候，共享计数器的值会++
3. 删除一个后续的智能指针，计数器会--
4. 删除最初的智能指针的时候，计数器归零，之后释放资源

### 3 如何实现一个shared_ptr

```
template<class T>
class SharedPointer{
public:
	SharedPointer() :refCount(nullptr), pointer(nullptr){}
	SharedPointer(T* adoptTarget) :refCount(nullptr), pointer(adoptTarget)
    {
        addReference();
    }
    
     SharedPointer(const SharedPointer<T>& copy)
        :refCount(copy.refCount), pointer(copy.pointer)
    {
        addReference();
    }
    
    virtual ~SharedPointer()
    {
        removeReference();
    }
    
    //左值，被赋值的时候不再指向之前的值，要先减少之前的引用计数，然后指向新的资源，++新对象的计数
    SharedPointer<T>& operator=(const SharedPointer<T>& that)
    {
        if (this != &that)
        {
            removeReference();
            this->pointer = that.pointer;
            this->refCount = that.refCount;
            addReference();
        }
        return *this;
    }
    bool operator==(const SharedPointer<T>& other)
    {
        return pointer == other.pointer;
    }
    
    bool operator!=(const SharedPointer<T>& other)
    {
        return !operator==(other);
    }
    
     T& operator*() const
    {
        return pointer;
    }
    //成员调用
    T* operator->() const
    {
        return pointer;
    }
    //获取引用计数值
    int GetReferenceCount() const
    {
        if (refCount)
        {
            return *refCount;
        } 
        else
        {
            return -1;
        }
    }
protected:
	void addReference()
    {
        if (refCount)
        {
            (*refCount)++;
        }
        else
        {
            refCount = new int(0);
            refCount = 1;
        }
    }
    
     void removeReference()
    {
        if (refCount)
        {
            (*refCount)--;
            if (*refCount == 0)
            {
                delete refCount;
                delete pointer;
                refCount = 0;
                pointer = 0;
            }
        }
    }
private:
	int *refCount;
	T *pointer;

};
```

### 4 关于unique_ptr

unique_ptr 是最简单、最容易使用的一个智能指针，在声明的时候必须用模板参数指定类型：

```
unique_ptr<int> ptr1(new int(10));      // int智能指针
assert(*ptr1 == 10);                     // 可以使用*取内容
assert(ptr1 != nullptr);                // 可以判断是否为空指针

unique_ptr<string> ptr2(new string("hello"));  // string智能指针
assert(*ptr2 == "hello");                // 可以使用*取内容
assert(ptr2->size() == 5);               // 可以使用->调用成员函数
```

切记，很关键的一点就是声明的时候一定要初始化，否则等于直接操作了裸指针。

```
unique_ptr<int> ptr3;                // 未初始化智能指针
*ptr3 = 42 ;                         // 错误！操作了空指针
```

为了避免这种低级错误，你可以调用工厂函数 make_unique()

```
auto ptr3 = make_unique<int>(42);               // 工厂函数创建智能指针
assert(ptr3 && *ptr3 == 42);

auto ptr4 = make_unique<string>("god of war");  // 工厂函数创建智能指针
assert(!ptr4->empty());
```

**另外还要关注unique_ptr的所有权问题，指针的所有权是“唯一”的，不允许共享，任何时候只能有一个“人”持有它。**

所以，在向另一个 unique_ptr 赋值的时候，要特别留意，**必须用 std::move() 函数显式地声明所有权转移。**

赋值操作之后，指针的所有权就被转走了，原来的 unique_ptr 变成了空指针，新的 unique_ptr 接替了管理权，保证所有权的唯一性：

```
auto ptr1 = make_unique<int>(42);    // 工厂函数创建智能指针
assert(ptr1 && *ptr1 == 42);         // 此时智能指针有效

auto ptr2 = std::move(ptr1);         // 使用move()转移所有权
assert(!ptr1 && ptr2);               // ptr1变成了空指针
```

尽量不要对 unique_ptr 执行赋值操作就好了，让它“自生自灭”，完全自动化管理。切记unique_ptr是一个对象，所以，不要企图对它调用 delete，它会自动管理初始化时的指针，在离开作用域时析构释放内存。

### 5 关于share_ptr

share_ptr相比unique_ptr显得更加智能,他的最大特点就是他的所有权是可以安全共享的：

```
shared_ptr<int> ptr1(new int(10));     // int智能指针
assert(*ptr1 = 10);                    // 可以使用*取内容

shared_ptr<string> ptr2(new string("hello"));  // string智能指针
assert(*ptr2 == "hello");                      // 可以使用*取内容

auto ptr3 = make_shared<int>(42);  // 工厂函数创建智能指针
assert(ptr3 && *ptr3 == 42);       // 可以判断是否为空指针

auto ptr4 = make_shared<string>("zelda");  // 工厂函数创建智能指针
assert(!ptr4->empty());                   // 可以使用->调用成员函数
```

shared_ptr 具有完整的“值语义”（即可以拷贝赋值），所以，它可以在任何场合替代原始指针，而不用再担心资源回收的问题，比如用于容器存储指针、用于函数安全返回动态创建的对象，等等。

**注意事项：**

1. 引用计数的存储和管理是有成本的，shared_ptr 的销毁动作需要额外关心，在运行阶段，引用计数的变动是很复杂的，很难知道它真正释放资源的时机，析构函数中，一旦有特别复杂的，阻塞的操作，就会直接阻塞整个线程。

   比如:

   ```
   class DemoShared final      //  危险的类，不定时的地雷   
   {
   public:
       DemoShared() = default;
      ~DemoShared()            // 复杂的操作会导致shared_ptr析构时世界静止
      {
          // Stop The World ...
      }
   };
   ```

2. 循环引用,最常见的例子就是链表节点。有些时候这个在代码量大的时候很容易不自觉地造成环。

   ```
   class Node final
   {
   public:
       using this_type     = Node;
       using shared_type   = std::shared_ptr<this_type>;
   public:
       shared_type     next;      // 使用智能指针来指向下一个节点
   };
   
   auto n1 = make_shared<Node>();   // 工厂函数创建智能指针
   auto n2 = make_shared<Node>();   // 工厂函数创建智能指针
   
   assert(n1.use_count() == 1);    // 引用计数为1
   assert(n2.use_count() == 1);
   
   n1->next = n2;                 // 两个节点互指，形成了循环引用
   n2->next = n1;
   
   assert(n1.use_count() == 2);    // 引用计数为2
   assert(n2.use_count() == 2);    // 无法减到0，无法销毁，导致内存泄漏
   ```

### 6 关于weak_ptr

想要从根本上杜绝循环引用，光靠 shared_ptr 是不行了，必须要用到它的“小帮手”：weak_ptr。

它专门为打破循环引用而设计，只观察指针，不会增加引用计数（弱引用），但在需要的时候，可以调用成员函数 lock()，获取 shared_ptr（强引用）。

```
class Node final
{
public:
    using this_type     = Node;

    // 注意这里，别名改用weak_ptr
    using shared_type   = std::weak_ptr<this_type>;
public:
    shared_type     next;    // 因为用了别名，所以代码不需要改动
};

auto n1 = make_shared<Node>();  // 工厂函数创建智能指针
auto n2 = make_shared<Node>();  // 工厂函数创建智能指针

n1->next = n2;             // 两个节点互指，形成了循环引用
n2->next = n1;

assert(n1.use_count() == 1);    // 因为使用了weak_ptr，引用计数为1
assert(n2.use_count() == 1);   // 打破循环引用，不会导致内存泄漏

if (!n1->next.expired()) {     // 检查指针是否有效
    auto ptr = n1->next.lock();  // lock()获取shared_ptr
    assert(ptr == n2);
}
```

### Summary

1. 如果指针是“独占”使用，就应该选择 unique_ptr，它为裸指针添加了很多限制，更加安全。如果指针是“独占”使用，就应该选择 unique_ptr，它为裸指针添加了很多限制，更加安全。注意std::move才能转移所有权
2. 如果指针是“共享”使用，就应该选择 shared_ptr，它的功能非常完善，用法几乎与原始指针一样，**shared_ptr是强引用，无论如何都需要持有共享对象的时候就用它。weak_ptr是弱引用，不一定要持有对象，只是“偶尔”想去看看对象在不在，不在也可以接受。**
3. 应当使用工厂函数 make_unique()、make_shared() 来创建智能指针，强制初始化，而且还能使用 auto 来简化声明。

