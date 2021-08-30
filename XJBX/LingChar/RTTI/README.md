## RTTI

RTTI是Runtime Type Identification的缩写，意思是运行时类型识别。C++引入这个机制是为了让程序在运行时能根据基类的指针或引用来获得该指针或引用所指的对象的实际类型。但是现在RTTI的类型识别已经不限于此了，它还能通过typeid操作符识别出所有的基本类型（int，指针等）的变量对应的类型。

C++通过以下的两个操作提供RTTI：

1. typeid运算符，该运算符返回其表达式或类型名的实际类型。
2. dynamic_cast运算符，该运算符将基类的指针或引用安全地转换为派生类类型的指针或引用。

### 存在原因

RTTI是有用的，首先是得有态度，其次，必须谨慎得用，很多类型系统中的东西就是为避免RTTI做的努力。

C++原初是一个静态语言，编译的时候就确定了类型，不能在运行时更改，但是由于面向对象的多态性要求，C++中的指针或引用本身的类型，与其实际指代的类型并不一致。有时候我们需要将一个多态指针转换为其实际指向的对象类型，需要知道运行时类型信息，这就产生了运行时类型识别的需求。

但是RTTI的缺点就是：

1. 破坏了抽象性，一些理论和方法的原因，面向对象的纯洁性被波坏
2. 运行时类型是不确定的，带来了不确定性和脆弱性。
3. 扩展性的问题，新类型的加入，或许需要看`dynamic_cast`的代码并改动，保证不出bug。

### For Example

```
#include<iostream>
#include<typeinfo>
using namespace std;

class Base
{
public:
    virtual void funcA() { cout << "Base" << endl; }
};

class Derived : public Base
{
public:
    void funcB() { cout << "Derived" << endl; }
};

void funcC(Base* p)
{
    Derived* dp = dynamic_cast<Derived*>(p);
    if (dp != NULL) {
        dp->funcB();
    }
    else {
        p->funcA();
    }
};

void funcD(Base* p)
{
    Derived* dp = NULL;
    if (typeid(*p) == typeid(Derived))
    {
        dp = static_cast<Derived*>(p);
        dp->funcB();
    }
    else {
        p->funcA();
    }
}

int main(int argc, char const* argv[])
{
    Base* p = new Derived;
    //Derived* d = new Derived;
    cout << typeid(p).name() << endl;
    cout << typeid(*p).name() << endl;
    funcD(p);
    funcC(p);
    delete p;

    Base* dp = new Base;
    funcC(dp);
    funcD(dp);
    delete dp;
    return 0;
}
```

此时的输出

```
class Base * __ptr64
class Derived
Derived
Derived
Base
Base
```

funcC是用`dynamic_cast`类型转换是否成功来识别类型的，`dynamic_cast`操作符将基类类型对象的引用或指针转化为同一继承层次中的其他类型的引用或指针。

如果绑定到引用或指针的对象不是目标类型的对象，则`dynamic_cast`失败。

如果转换到指针类型的`dynamic_cast`失败，则`dynamic_cast`的结果是`nullptr`值;

funcD是用`typeid`判断基类地址是否一致的办法来识别类型的。

### typeid -> typeinfo

`typeid`是C++的关键字之一, 等同于`sizeof`这类operator。 typeid 操作符的返回结果是名为 `type_info`的标准库类型的对象的引用，在头文件typeinfo 中定义。 有两种形式:

1. typeid(type)
2. typeid(expression)

表达式的类型是类类型，且至少含有一个虚函数，则typeid操作符返回表达式的动态类型，需要在运行时计算，否则返回表达式的静态类型，在编译时就可以计算。

举个例子，如果p是基类指针，并且指向一个派生类型的对象，并且基类中有虚函数，那么`typeid(*p)`返回p所指向的派生类类型，`typeid(p)`返回基类类型。

其必须实现4个操作：

1. t1 == t2: 如果两个对象t1和t2类型相同，则返回true;否则返回false
2. t1 != t2: 如果两个对象t1和t2类型不同，则返回true;否则返回false
3. t.name(): 返回类型的c-style字符串，类型名字用系统相关的方法产生
4. t1.before(t2): 返回指出t1是否出现在t2之前 的bool值

`type_info`类提供了public虚析构函数，以使用户能够用其作为基类。它的默认构造函数和复制构造函数及赋值操作符都定义为private，所以不能定义或复制type_info 类型的对象。 程序中创建type_info对象的唯一方法是使用typeid操作符。 由此可见，如果把typeid看作函数的话，其应该是type_info的友元。 这具体由编译器的实现所决定，标准只要求实现为每个类型返回唯一的字符串。

通过在虚表中放一个额外的指针，每个新类只产生一个typeinfo实例，额外指针指向typeinfo, typeid返回对它的一个引用。

## 关于c++的cast

C的强制转换表面上看起来功能强大什么都能转，但是转化不够明确，不能进行错误检查，容易出错。

### static_cast

`static_cast < new_type > ( expression )`

经常使用的场景就是：

1. 基本数据类型之间的转换
2. initializing conversion

```
int n = static_cast<int>(3.14);
cout << "n = " << n << '\n';
vector<int> v = static_cast<vector<int>>(10);
cout << "v.size() = " << v.size() << '\n';
```

3. 类指针或引用向下转换。
4. 将类型转为右值类型，进行move操作，这个在标准库中有体现

```
vector<int> v = static_cast<vector<int>>(10);
vector<int> v2 = static_cast<vector<int>&&>(v);
cout << "after move, v.size() = " << v.size() << '\n';
cout << v2.size() << endl;
```

5. 子类数组指针向上转成父类的指针

```
struct B {
    int m = 0;
    void hello() const {
        cout << "Hello world, this is B!\n";
    }
};
struct D : B {
    void hello() const {
        cout << "Hello world, this is D!\n";
    }
};

D a[10];
B* dp = static_cast<B*>(a);
dp->hello();
```

6. 枚举转换成int or float

```
enum E { ONE = 1, TWO, THREE };
E e = E::ONE;
int one = static_cast<int>(e);
cout << one << '\n';
```

7. int to enum, enum to another emum

```
enum class E { ONE = 1, TWO, THREE };
enum EU { ONE = 1, TWO, THREE };

E e = E::ONE;
int one = static_cast<int>(e);
E e2 = static_cast<E>(one);
EU eu = static_cast<EU>(e2);
```

8. void* to any type

```
int a = 100;
void* voidp = &a;
int *p = static_cast<int*>(voidp);
```

需要注意的是：

1. static_cast不能转换掉expression的const、volatile和__unaligned属性,
2. 编译器隐式执行任何类型转换都可由static_cast显示完成

### dynamic_cast

`dynamic_cast < new-type > ( expression )`

动态映射可以映射到中间层级，将派生类映射到任何一个基类，然后在基类之间可以相互映射。

先恢复源指针的RTTI信息，再取目标的RTTT信息，比较两者是否相同，不同取目标类型的基类; 由于它需要检查一长串基类列表，故动态映射的开销比typeid大。

**dynamic_cast的安全性**: 如实现原理所说，`dynamic_cast`会做一系列的类型检查，转换成功会返回目标类型指针，失败则会返回NULL, 相对于`static_cast`安全，因为 `static_cast`即使转换失败也不会返回nullptr。

另外注意的是下面的例子,这个情况下，staticcast也是安全的。

```
class Base
{
public:
    void func() { cout << "Base func" << endl; }
};

class Derived : public Base
{
public:
    void func() { cout << "Derived func" << endl; }
};

int main(int argc, char const* argv[])
{
    Derived *pd = new Derived;
    pd->func();
    Base* pb1 = dynamic_cast<Base*>(pd);
    pb1->func();
    Base* pb2 = static_cast<Base*>(pd);
    pb2->func();
    return 0;
}
```

继承子类是包含父类的，相当于在父类的基础上在添加子类的成员。

相反，如果指针指向的内存是父类成员，转成子类指针，`dynamic_cast` 则会失败，返回NULL, 但是`static_cast`不会失败，强制转过去了，如果此时子类指针访问父类中不存在，但是子类中存在的成员，则会发生意想不到的问题。

```
class Base
{
public:
    virtual void func() { cout << "Base func" << endl; }
};

class Derived : public Base
{
public:
    virtual void func() { cout << "Derived func" << endl; }
    int m_value = 0;
};

int main(int argc, char const* argv[])
{
    Base *pb = new Base;
    pb->func();
    Derived* pd1 = dynamic_cast<Derived*>(pb);
    if (pd1 != NULL) {
        pd1->func();
    } else {
        cout << "dynamic_cast failed" << endl;
    }
    Derived* pd2 = static_cast<Derived*>(pb);
    pd2->func();
    cout << "m_value: " << pd2->m_value << endl;
    return 0;
}
```

对于下行转换: 你必须确定要转换的数据确实是目标类型的数据，即需要注意要转换的父类类型指针是否真的指向子类对象，如果是，static_cast和dynamic_cast都能成功；如果不是static_cast能返回，但是不安全，可能会出现访问越界错误，而dynamic_cast在运行时类型检查过程中，判定该过程不能转换，返回nullptr。

### const_cast

`const_cast < new_type > ( expression )`

`static_cast`是不能去掉 `const`属性的，之前也讲过了，而 `const_cast`是专门用来去掉 `const`

而添加const, `static_cast` 也是可以添加上 const，只是不能去掉const

```
const int a = 26;
const int* pa = &a;
//*pa = 3;  // 编译不过，指针常量不能通过指针修改值

int* b = const_cast<int*>(pa);  // 把const转换掉
*b = 3;

cout << "a: " << a << endl;  // 26
cout << "*b: " << *b << endl;  // 3
```

a 为 `const int` 类型，不可修改，pa 为 `const int*` 类型，不能通过pa指针修改a的值

b 通过 `const_cast` 转换掉了const，成功修改了 a 的值。

为什么a输出是26呢,这里就涉及到编译的问题，编译器会认为const int是一个编译期可计算出结果的常量，会像宏定义一样，用到的地方会被编译器替换成26。

实际上应该也没人这么用const_cast吧

使用场景：

1. 如果有一个函数，它的形参是non-const类型变量，而且函数不会对实参的值进行改动，这时我们可以使用类型为const的变量来调用函数。但是其实比较蠢，形参前面加const不久好了嘛。。。

```
void func(int* a)
{ }

int main()
{
    const int a = 26;
    const int* pa = &a;
    func(const_cast<int*>(pa));
}
```

2. 定义了一个non-const的变量，却使用了一个指向const值的指针来指向它，在程序的某处我们想改变这个变量的值了，但手头只持有指针，这时`const_cast`就可以用到了.但其实这个还是比较蠢，，，

```
int a = 26;
const int* pa = &a;
// *pa = 1; 编译不过
int* pa2 = const_cast<int*>(pa);
*pa2 = 1;
```

### reinterpret_cast

`reinterpret_cast < new_type > ( expression )`

reinterpret_ cast 通常为操作数的位模式提供较低层的重新解释，没用过，一般为了移植性也不会用吧。

```
int n = 9; double d = static_cast<double>(n); 
cout << n << " " << d; // 输出9 9

int n2 = 9; double d2 = reinterpret_cast<double&>(n2);
cout << n2 << " " << d2; // 输出9 -9.25596e+61
```

上面的例子中，我们将一个变址从int 转换到 double。这些类型的二进制表达式是不同的。要将整数9转换到双精度整数9, static_cast需要正确地为双精度整数d补足比特位。其结果为9.0。

而 reinterpret_cast 的行为却不同，仅仅是把内存拷贝到目标空间，解释出来是一个大数。
