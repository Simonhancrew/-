## 关于重载（overload），重写（override）和重定义（redefining）

之前面试的一个问题，要讲细一点，但是其实平时写的话也接触不到这么深，就当八股文算了

首先简单的说一下三者的概念：

1. 重载(overload):是函数名相同，参数列表不同，override只是在类的内部存在。

2. 重写(override),也叫覆盖。子类重新定义父类中有相同名称和参数的虚函数(virtual)。在继承关系之间。C++利用虚函数实现多态。

   重写的特点有三个：

   1.  被重写的函数不能是static的。必须是virtual的
   2. 重写函数必须有**相同的类型，名称和参数列表**
   3. 重写函数的访问修饰符可以不同。尽管父类的virtual方法是private的，派生类中重写改写为public,protected也是可以的。

   这是因为被virtual修饰的成员函数，无论他们是private/protect/public的，都会被统一放置到虚函数表中。对父类进行派生时，子类会继承到拥有相同偏移地址的虚函数表(相同偏移地址指的是各虚函数先与VPTR指针的偏移)，因此就允许子类对这些虚函数进行重写

3. 重定义(redefining)，也叫隐藏。子类重新定义父类有相同名称的非虚函数(参数列表可以不同)。

   子类若有和父类相同的函数，那么，这个类将会隐藏其父类的方法。除非你在调用的时候，强制转换成父类类型。在子类和父类之间尝试做类似重载的调用时不能成功的。

### code的尝试

```
#include<iostream>
#include<typeinfo>
using namespace std;

class a {
public:
	void hi() {
		cout << "hello a" << endl;
	}
	virtual void func() {
		cout << "hh" << endl;
	}
};

class b :public a {
public:
	void hi() {
		cout << "hello b" << endl;
	}
	void hi(int i) {
		cout << "hi" << i << endl;
	}
	void func() {
		cout << "hh from b" << endl;
	}

};

int main() {
	cout << "1 重定义" << endl;
	a aa;
	auto vb = static_cast<b*>(&aa);
	vb->hi();
	cout << "2 重写" << endl;
	a* rb = new b;
	rb->func();
	cout << "3 重载" << endl;
	b tb;
	tb.hi();
	tb.hi(1);
	return 0;
}
```

