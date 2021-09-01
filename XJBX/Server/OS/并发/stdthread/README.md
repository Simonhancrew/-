## C++ thread

前几天被问多线程循环打印居然没打出来，cond_variable，信号量，mutex，thread都懂，但是实际操作的时候就裂开了，发现还是要写一写。这里简单的把C++标准库的多线程操作大部分记录一遍。

### 1 线程 + 进程 + 并发的基本概念

首先关于并发：

1. 两个或者更多的任务（独立的活动）同时发生（进行）：一个程序同时执行多个独立的任务；
2. 以往计算机，单核cpu（中央处理器）：某一个时刻只能执行一个任务，由操作系统调度，每秒钟进行多次所谓的“任务切换”。并发的假象（不是真正的并发），切换（上下文切换）时要保存变量的状态、执行进度等，存在时间开销；
3. 随着硬件发展，出现了多处理器计算机：用于服务器和高性能计算领域。台式机：在一块芯片上有多核（一个CPU内有多个运算核心，对于操作系统来说，每个核心都是作为单独的CPU对待的）：双核，4核，8核，10核（自己的笔记本是4核8线程的）。能够实现真正的并行执行多个任务（硬件并发）
4. 使用并发的原因：主要就是同时可以干多个事，提高性能

之后关于进程和线程：

1. 每个进程（执行起来的可执行程序），都有唯一的一个主线程
2. 当执行可执行程序时，产生一个进程后，这个主线程就随着这个进程默默启动起来了
3. 线程这个东西，可以理解为一条代码的执行通路。除了主线程之外，可以通过写代码来创建其他线程，其他线程走的是别的道路，甚至区不同的地方。每创建一个新线程，就可以在同一时刻，多干一个不同的事
4. 线程并不是越多越好，每个线程，都需要一个独立的堆栈空间（大约1M），线程之间的切换要保存很多中间状态，切换也会耗费本该属于程序运行的时间

并发的实现方式有多种：

1. 通过多个进程实现并发
2. 多线程的并发

关于多进程并发，还要处理号进程间通信的问题，可以考虑的方法就是：管道，文件，消息队列，共享内存，socket（AF_LOCAL）

多线程并发，类似与轻量级的进程，一个进程中的所有线程共享地址空间（共享内存），全局变量、全局内存、全局引用都可以在线程之间传递，所以多线程开销远远小于多进程

### 2 线程的启动 + 结束 + 创建线程

传统多线程的执行流程

```c++
#include <iostream>
#include <thread>
using namespace std;

void myPrint()
{
	cout << "我的线程开始运行" << endl;
	//-------------
	//-------------
	cout << "我的线程运行完毕" << endl;
	return;
}

int main()
{
	//(1)创建了线程，线程执行起点（入口）是myPrint；(2)执行线程
	thread myThread(myPrint);

	//(2)阻塞主线程并等待myPrint执行完，当myPrint执行完毕，join()就执行完毕，主线程继续往下执行
	//join意为汇合，子线程和主线程回合
	myThread.join();

	//设置断点可看到主线程等待子线程的过程
	//逐语句，就是每次执行一行语句，如果碰到函数调用，它就会进入到函数里面
	//逐过程，碰到函数时，不进入函数，把函数调用当成一条语句执行

	//(3)传统多线程程序中，主线程要等待子线程执行完毕，然后自己才能向下执行
	//detach:分离，主线程不再与子线程汇合，不再等待子线程
	//detach后，子线程和主线程失去关联，驻留在后台，由C++运行时库接管
	//myThread.detach();

	//(4)joinable()判断是否可以成功使用join()或者detach()
	//如果返回true，证明可以调用join()或者detach()
	//如果返回false，证明调用过join()或者detach()，join()和detach()都不能再调用了
	if (myThread.joinable())
	{
		cout << "可以调用可以调用join()或者detach()" << endl;
	}
	else
	{
		cout << "不能调用可以调用join()或者detach()" << endl;
	}
	
	cout << "Hello World!" << endl;
	return 0;
}
```

```c++
thread mythread1(func); // 创建一个线程并开始执行
mythread1.join(); // 主线程阻塞在这里，等待子线程结束了才继续。
mythread1.detach(); 
/*
主线程与子线程分离,之后主线程不与子线程汇合了 , 主线程不必等待子线程结束一旦detach()之后,与这个主线程关联的thread对象就会失去与这个主线程的关联关系 , 此时这个子线程就会驻留在后台运行.相当于这个子线程被c++运行时库接管,当该子线程运行结束,有运行时库清理该线程的相关资源(守护线程).一旦detach了,就不能再用join了,系统会报异常
*/
```

另外还有一些操作

```c++
joinable()
if(线程对象.joinable()){
        线程对象.join();//可以
        //线程对象.detach();
}
/*
判断是否可以成功使用join()或detach()的
返回 true(就可以join或detach) 或者flase
*/
```

另外线程类参数是一个可调用对象，这就表示**函数、函数指针、lambda表达式、bind创建的对象或者重载了函数调用运算符的类对象**都可以放进去

比如,这里可以看到的是复制构造函数也执行了一次，说明创造了新的对象，是一个深拷贝。

```c++
#include <iostream>
#include <thread>

using namespace std;


class A {
public:
	A() {
		cout << "init " << endl;
	}
	A(A& a) {
		cout << "copy" << endl;
	}
	void operator()() { // 里面可以有参数，thread相应也要传参
		cout << "child thread" << endl;
	}
};

int main() {
	A a;
	//A::i = 1;
	thread job1(ref(a));
	// 主线程等待子线程结束，回收子线程
	job1.join();
	//job1.detach();
	cout << "main thread" << endl;
	return 0;
}
```

另外，lambda函数也可以的

```c++
//main函数中
auto lambdaThread = [] {
    cout << "我的线程开始执行了" << endl;
    //-------------
    //-------------
    cout << "我的线程开始执行了" << endl;
};

thread myThread(lambdaThread);
myThread.join();
```

### 3 线程传参 + 成员函数做thread参数 + 易错点

关于thread传参的坑还是挺多的，直接看代码比较直接。

```c++
#include <iostream>
#include <thread>

using namespace std;


void print(const int& i,char* buff) {
	cout << i << endl;
	cout << buff << endl;
}

int main() {
	char buf[] = "hello";
	int num = 10;
	thread t1(print, num, buf);
	t1.detach();
	return 0;
}
```

join的时候大概率不会碰到问题，detach的时候应该有概率小裂开，这里thread里还是会重新拷贝一个i，哪怕传引用也是fake引用，但是buf在主线程结束的时候是被释放了的。

**临时对象是在主线程里面构造的，但是如果是传值，可能是在子线程里面构造出来的。比较简单的验证方法就是线程函数里给类引用接，thread中给一个值。**

改进的方法就是

```c++
#include <iostream>
#include <thread>

using namespace std;


void print(const int& i,const string& buff) {
	cout << i << endl;
	cout << buff << endl;
}


int main() {
	char buf[] = "hello";
	int num = 10;
	thread t1(print, num, string(buf));
	t1.detach();
	return 0;
}
```

注意thread的时候就要做出零时对象，否则你还是不知道buff构建的具体时机是什么时候。

最后大概有几点建议吧：

1. 如果传递int这种简单类型，推荐使用值传递，不要用引用
2. 如果传递类对象，避免使用隐式类型转换，全部都是创建线程这一行就创建出临时对象，然后在函数参数里，用引用来接，否则还会创建出一个对象
3. 不要用detach

另外传递类对象，智能指针作为线程参数，看一看代码就懂了.另外获取thread_id也在下面的code中。

```c++
#include <iostream>
#include <thread>
using namespace std;

class A {
public:
	mutable int m_i; //m_i即使实在const中也可以被修改
	A(int i) :m_i(i) {}
};

void myPrint(const A& pmybuf)
{
	pmybuf.m_i = 199;
	cout << "子线程myPrint的参数地址是" << &pmybuf << "thread = " << std::this_thread::get_id() << endl;
}

int main()
{
	A myObj(10);
	//myPrint(const A& pmybuf)中引用不能去掉，如果去掉会多创建一个对象
	//const也不能去掉，去掉会出错
	//即使是传递的const引用，但在子线程中还是会调用拷贝构造函数构造一个新的对象，
	//所以在子线程中修改m_i的值不会影响到主线程
	//如果希望子线程中修改m_i的值影响到主线程，可以用thread myThread(myPrint, std::ref(myObj));
	//这样const就是真的引用了，myPrint定义中的const就可以去掉了，类A定义中的mutable也可以去掉了
	thread myThread(myPrint, myObj);
	myThread.join();
	//myThread.detach();

	cout << "Hello World!" << endl;
}
```

智能指针就只能join了.

```c++
#include <iostream>
#include <thread>
#include <memory>
using namespace std;

void myPrint(unique_ptr<int> ptn)
{
	cout << "thread = " << std::this_thread::get_id() << endl;
}

int main()
{
	unique_ptr<int> up(new int(10));
	//独占式指针只能通过std::move()才可以传递给另一个指针
	//传递后up就指向空，新的ptn指向原来的内存
	//所以这时就不能用detach了，因为如果主线程先执行完，ptn指向的对象就被释放了
	thread myThread(myPrint, std::move(up));
	myThread.join();
	//myThread.detach();

	return 0;
}
```

### 4 互斥及其用法 + 死锁 + unique_lock

关于互斥mutex：

1. 互斥量就是个类对象，可以理解为一把锁，多个线程尝试用lock()成员函数来加锁，只有一个线程能锁定成功，如果没有锁成功，那么流程将卡在lock()这里不断尝试去锁定。
2. 互斥量使用要小心，保护数据不多也不少，少了达不到效果，多了影响效率。

```
#include<mutex>
#include<thread>

class A{
public:
	void inmRQ(){
		for(int i=1;i<=100000000;i++){
			my_mutex.lock();
			mRQ.push(i);
			my_mutex.unlock();
			cout<<"start"<<endl;
		}
	}
	void outmRQtem(int &command){
		my_mutex.lock();
		if( !mRQ.empty() ){
				command=mRQ.front();
				mRQ.pop();
		}
		my_mutex.unlock();
	}

	void outmRQ(){
		int command=0;
		for(int i=1;i<=100000000;i++){
			outmRQtem(command);
			cout<<command<<endl;
		}
		cout<<"end"<<endl;
	}
private:
	std::queue<int> mRQ;
	std::mutex my_mutex;//创建一个互斥量
};

int main(){
	A a;
	
	thread myout(&A::outmRQ,ref(a));
	thread myin(&A::inmRQ,ref(a));

	myin.join();
	myout.join();
	return 0;
}
```

还可以用lock_guard取代lock和unlock的过程,不能再使用lock和unlock

```
void outmRQtem(int &command){
	std::lock_guard<std::mutex> myguard(my_mutex);
	//构造函数执行了lock(),析构函数执行了unlock()
	//这个函数结束,生命周期结束自动调用析构函数
	if( !mRQ.empty() ){
			command=mRQ.front();
			mRQ.pop();
	}
}
```

```'
{//加大括号,使lock_guard生命周期在大括号内
	std::lock_guard<std::mutex> myguard(my_mutex);
	if( !mRQ.empty() ){
			command=mRQ.front();
			mRQ.pop();
	}	
}
```

之后经常讨论的问题就是死锁,在许多应用中进程需要以**独占**的方式访问资源，当操作系统允许多个进程并发执行时可能会出现进程永远被阻塞现象，如两个进程分别等待对方所占的资源，于是两者都不能执行而**处于永远等待状态**，此现象称为**死锁**。

> 死锁通常被定义为：如果一个进程集合中的每个进程都在等待只能由此集合中的其他进程才能引发的事件，而无限期陷入僵持的局面称为死锁。

产生的条件就是：

1. 互斥条件，临界资源是独占资源，进程应互斥且排他的使用这些资源。
2. 占有和等待条件，进程在请求资源得不到满足而等待时，不释放已占有资源。
3. 不剥夺条件，又称不可抢占，已获资源只能由进程自愿释放，不允许被其他进程剥夺
4. 循环等待条件，又称环路条件，存在循环等待链，其中，每个进程都在等待链中等待下一个进程所持有的资源，造成这组进程处于永远等待状态。

避免和检测的算法可以留到下次再学习。

另外标准库还有些其余的方法，可以继续讲讲

1. 首先`std::lock()`,一次锁住`>=2`个互斥量 (谨慎使用),它不存在因为多个线程因为锁的顺序而导致的死锁风险.只有同时锁住才会继续向下执行(要么都锁住,要么都不锁)

```
std::lock(my_mutex1,my_mutex2,..);
my_mutex1.unlock();//解锁还是要继续写
my_mutex2.unlock();
```

2. 还有`std::adopt_lock`,在用这个参数前,我们一定要记得先`lock`,`std::lock()`与`std::lock_guard`结合使用,有了`std::adopt_lock` `std::lock_guard`就在构造时就不`lock`了,析构正常`unlock`

```
std::lock(my_mutex1,my_mutex2);
std::lock_guard<std::mutex> a_guard(my_mutex1,std::adopt_lock);
std::lock_guard<std::mutex> b_guard(my_mutex2,std::adopt_lock);
```

3. **还有一个很重要的类模板**`std::unique_lock`,`unique_lock`是一个类模板 , 可以完全取代`lock_guard` , 但一般用`lock_guard`(推荐使用).`unique_lock`比`lock_guard`灵活一点, 效率上差一点 , 内存多一点

```
std::unique_lock<std::mutex> myguard(my_mutex);
std::unique_lock<std::mutex> myguard(my_mutex,std::adopt_lock);//std::adopt_lock放在第二个,表示这个互斥量已经被lock了
```

4. 之后的`std::try_to_lock`,我们尝试用`lock()`去这个`mutex` , 如果没有锁成功 , 就执行没锁成功的代码 , 然后再执行后续代码

   使用前提 : 这个线程不能自己先`lock`,这个一般是配合unique_lock使用，可以用owns_lock看有没有获取到锁。

```
int command=0;

class A{
public:
	void inmRQ(){
		for(int i=1;i<=100000000;i++){
			my_mutex1.lock();
			mRQ.push(i);
			my_mutex1.unlock();
			cout<<"start"<<endl;
			
		}
	}
	int outmRQtem(){
		std::unique_lock<std::mutex> myguard(my_mutex1,std::try_to_lock);
		if(myguard.owns_lock()){//拿到锁了
			if( !mRQ.empty() ){
				command=mRQ.front();
				mRQ.pop();
			}
		}else{//没拿到锁
			cout<<"I have no lock"<<endl;
		}
		return command;
	}

	void outmRQ(){
		command=0;
		for(int i=1;i<=100000000;i++){
			command=outmRQtem();
			cout<<command<<endl;
		}
		cout<<"end"<<endl;
	}
private:
	std::queue<int> mRQ;
	std::mutex my_mutex1;
};

int main(){
	A a;
	
	thread myout(&A::outmRQ,ref(a));
	thread myin(&A::inmRQ,ref(a));

	myin.join();
	myout.join();
	return 0;
}
```

5. `std::defer_lock`,使用前提 : 不能自己先`lock`,否者会报异常.初始化一个没有加锁的`mutex`,所以加锁还是要自己去做，解锁忘了也无所谓。

```
std::unique_lock<std::mutex> myguard(my_mutex1,std::defer_lock);//将my_mutex1与myguard绑定到一起
myguard.lock();//这样就可以自己亲自加锁
myguard.unlock();//也可以自己亲自解锁,如果不写也会可以生命周期调用析构函数自动解锁
```

6.  `try_lock()`，尝试给互斥量加锁 , 如果拿不到锁 , 则返回false , 如果拿到锁了 , 就返回true , 这个函数不会阻塞。一般和`std::defer_lock`搭配使用

```
std::unique_lock<std::mutex> myguard(my_mutex1,std::defer_lock);
if(myguard.try_lock()){//拿到锁了
	if( !mRQ.empty() ){
		command=mRQ.front();
		mRQ.pop();
	}
}else{//没拿到锁
	cout<<"I have no lock"<<endl;
}
```

7. `release()`返回它所管理的`mutex`对象指针 , 并释放所有权 即`unique_lock`和`mutex`不再有关系。如果原来的`mutex`对象处于加锁状态 , 你有责任接管过来并负责解锁

```
    std::unique_lock<std::mutex> myguard(my_mutex1);

    if( !mRQ.empty() ){
        command=mRQ.front();
        mRQ.pop();
    }
    std::mutex *p=myguard.release();
    p->unlock();
    //my_mutex1.unlock();//release之后也可以这样用
```

关于unique_lock锁的传递方法

```
std::unique_lock<std::mutex> myguard1(my_mutex1);//myguard1拥有my_mutex1的所有权
//这个所有权可以转移但不能复制,类似于智能指针
std::unique_lock<std::mutex> myguard2(std::move(myguard1));//移动所有权,与myguard2绑到一起,myguard1指向空
```

或者

```
class A{
public:
	std::unique_lock<std::mutex> my_move(){
		std::unique_lock<std::mutex> tem(my_mutex1);
		return tem;
	}
	void inmRQ(){
		for(int i=1;i<=100000000;i++){
			my_mutex1.lock();
			mRQ.push(i);
			my_mutex1.unlock();
			cout<<"start"<<endl;
		}
	}
	int outmRQtem(){
		std::unique_lock<std::mutex> myguard=my_move();
		if( !mRQ.empty() ){
			command=mRQ.front();
			mRQ.pop();
		}
		return command;
	}

	void outmRQ(){
		command=0;
		for(int i=1;i<=100000000;i++){
			command=outmRQtem();
			cout<<command<<endl;
		}
		cout<<"end"<<endl;
	}
private:
	std::queue<int> mRQ;
	std::mutex my_mutex1;
};
```

### 5 单例设计模式共享数据 + call_once

关于单例模式, 在整个项目中 , 有属于某个或者某些特殊的类 , 属于该类的对象 , 我只能创建一个 , 多了创建不了。看代码就行了

```
class A{
private:
	A(){} //私有化构造函数
	static A *mya;//静态成员函数
public:
	static A* GetInstance(){
		if( mya==NULL ){
			mya = new A();
		}
		return mya;
	}
	void test(){
		cout<<"测试"<<endl;
	}
};
A* A::mya = NULL;

int main(){
	A *p_a = A::GetInstance();//创建一个对象 , 返回该类对象指针
	A *p_b = A::GetInstance();
	return 0;
}
```

单例技巧

```
class A{
private:
	A(){} //私有化构造函数
	static A *mya;//静态成员函数
public:
	static A* GetInstance(){
		if( mya==NULL ){
			mya = new A();
			static CGhuishou c;
		}
		return mya;
	}
	class CGhuishou{//类中套类 , 用来释放对象
	public:
		~CGhuishou(){
			if( A::mya ){
				delete A::mya;
				A::mya = NULL;
			}
		}
		
	};
	void test(){
		cout<<"test"<<endl;
	}
};
A* A::mya = NULL;

int main(){
	A *p_a = A::GetInstance();//创建一个对象 , 返回该类对象指针
	A *p_b = A::GetInstance();
	p_a->test();
	return 0;
}
```

最后看一看单例模式里面的数据共享问题.建议在主线程初始化单例对象.当在线程中初始化单例时 , 就要注意数据共享问题

```
using namespace std;

std::mutex mymutex;
class A{
private:
	A(){} //私有化构造函数
	static A *mya;//静态成员函数
public:
	static A* GetInstance(){//这里可能会出现reorder问题,请自行百度
		if( mya==NULL ){//双重锁定(双重检查)
			mymutex.lock();
			if( mya==NULL ){
				mya = new A();
				static CGhuishou c;
			}
			mymutex.unlock();
		}
		return mya;
	}
	class CGhuishou{//类中套类 , 用来释放对象
	public:
		~CGhuishou(){
			if( A::mya ){
				delete A::mya;
				A::mya = NULL;
			}
		}
		
	};
	void test(){
		cout<<"test"<<endl;
	}
};
A* A::mya = NULL;

void run1(){
	while (true){
		A* ptr=A::GetInstance();
		cout<<"run1"<<endl;
	}
}
void run2(){
	while (true){
		A* ptr=A::GetInstance();
		cout<<"run2"<<endl;
	}
}

int main(){
	std::thread myjob1(ref(run1));
	std::thread myjob2(ref(run2));

	myjob1.join();
	myjob2.join();
	return 0;
}
```

最后看一眼`std::call_one()`

> 第二个参数是一个函数名a( )
>
> 功能 : call_one功能是保证函数a()只被调用一次
>
> 具备互斥量能力 , 比互斥量消耗少一点
>
> call_one()需要与一个标记结合使用 , 这个标记 std::once_flag (结构体)
>
> call_one()通过这个标记来决定对应的函数a()是否执行 , 调用call_one()标记std::once_flag 以后a()就不会再执行了

```
std::mutex mymutex;
std::once_flag flag;

class A{
private:
	A(){} //私有化构造函数
	static A *mya;//静态成员函数
public:
	static void CreatInstance(){//只被调用一次
		cout<<"I was called"<<endl;
		mya = new A();
		static CGhuishou c;
	}
	static A* GetInstance(){
		std::call_once(flag,CreatInstance);
		return mya;
	}
	class CGhuishou{//类中套类 , 用来释放对象
	public:
		~CGhuishou(){
			if( A::mya ){
				delete A::mya;
				A::mya = NULL;
			}
		}
		
	};
	void test(){
		cout<<"test"<<endl;
	}
};
A* A::mya = NULL;

void run1(){
	while (true){
		A* ptr=A::GetInstance();
		cout<<"run1"<<endl;
	}
}
void run2(){
	while (true){
		A* ptr=A::GetInstance();
		cout<<"run2"<<endl;
	}
}
```

### 6 条件变量

首先看一下之前经常问的题目，多线程循环打印的问题

```c++
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;


class print {
public:
	print():flag (1) {}
	void printa() {
		unique_lock<mutex> lk(mtx);
		for (int i = 0; i < 10; i++) {
			while (flag != 1) cv.wait(lk);
			cout << "a" << endl;
			flag = 2;
			cv.notify_all();
		}
		cout << "a finish" << endl;
	}
	void printb() {
		unique_lock<mutex> lk(mtx);
		for (int i = 0; i < 10; i++) {
			while (flag != 2) cv.wait(lk);
			cout << "b" << endl;
			flag = 3;
			cv.notify_all();
		}
		cout << "b finish" << endl;
	}
	void printc() {
		unique_lock<mutex> lk(mtx);
		for (int i = 0; i < 10; i++) {
			while (flag != 3) cv.wait(lk);
			cout << "c" << endl;
			flag = 1;
			cv.notify_all();
		}
		cout << "c finish" << endl;
	}
private:
	mutex mtx;
	condition_variable cv;
	int flag;
};

int main() {
	print pt;
	thread t1(&print::printa, ref(pt));
	thread t2(&print::printb, ref(pt));
	thread t3(&print::printc, ref(pt));
	t1.join();
	t2.join();
	t3.join();
	return 0;
}
```

condition_variable主要的操作就几个

1. wait(),`wait` 导致当前线程阻塞直至条件变量被通知，或虚假唤醒发生，可选地循环直至满足某谓词
2. notidy_one()
3. notify_all(),尝试把该`condition_variable`对象的`wait`线程全都唤醒

有几个点要注意一下

1. 若当前线程未锁定 [lock.mutex()](mk:@MSITStore:E:\cppreference-zh-20210212.chm::/chmhelp/cpp-thread-unique_lock-mutex.html) ，则调用此函数是未定义行为。
2. 若 [lock.mutex()](mk:@MSITStore:E:\cppreference-zh-20210212.chm::/chmhelp/cpp-thread-unique_lock-mutex.html) 与所有其他当前等待在同一条件变量上的线程所用的互斥不相同，则调用此函数是未定义行为。
3. `notify_one()`/`notify_all()` 的效果与 `wait()`/`wait_for()`/`wait_until()` 的三个原子部分的每一者（解锁+等待、唤醒和锁定）以能看做原子变量[修改顺序](mk:@MSITStore:E:\cppreference-zh-20210212.chm::/chmhelp/cpp-atomic-memory_order.html#.E4.BF.AE.E6.94.B9.E9.A1.BA.E5.BA.8F)单独全序发生：顺序对此单独的 condition_variable 是特定的。譬如，这使得 `notify_one()` 不可能被延迟并解锁正好在进行 `notify_one()` 调用后开始等待的线程。

wait的函数原型

```
void wait( std::unique_lock<std::mutex>& lock );
template< class Predicate >
void wait( std::unique_lock<std::mutex>& lock, Predicate pred );
```

看一个标注库的实例

```
#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
 
std::condition_variable cv;
std::mutex cv_m; // 此互斥用于三个目的：
                 // 1) 同步到 i 的访问
                 // 2) 同步到 std::cerr 的访问
                 // 3) 为条件变量 cv
int i = 0;
 
void waits()
{
    std::unique_lock<std::mutex> lk(cv_m);
    std::cerr << "Waiting... \n";
    cv.wait(lk, []{return i == 1;});
    std::cerr << "...finished waiting. i == 1\n";
}
 
void signals()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lk(cv_m);
        std::cerr << "Notifying...\n";
    }
    cv.notify_all();
 
    std::this_thread::sleep_for(std::chrono::seconds(1));
 
    {
        std::lock_guard<std::mutex> lk(cv_m);
        i = 1;
        std::cerr << "Notifying again...\n";
    }
    cv.notify_all();
}
 
int main()
{
    std::thread t1(waits), t2(waits), t3(waits), t4(signals);
    t1.join(); 
    t2.join(); 
    t3.join();
    t4.join(); 
}
```

另外还是这里发表一下我自己对于condition_variable的感觉吧。当前的时候，并不是所有的场景下，我们都希望独占某个资源，不然的话你可能要写出很发麻的解锁的代码

```
// 这是「生产者消费者问题」中的消费者的部分逻辑
// 等待队列非空，再从队列中取走元素进行处理

加锁(lock);  // lock 保护对 queue 的操作
while (queue.isEmpty()) {  // 队列为空时等待
    解锁(lock);
    // 这里让出锁，让生产者有机会往 queue 里安放数据
    加锁(lock);
}
data = queue.pop();  // 至此肯定非空，所以能对资源进行操作
解锁(lock);
消费(data);  // 在临界区外做其它处理
```

里面的while基本就又是一个自旋锁，所以，只要条件没有发生改变，while里的就没必要再去加锁，判断，等待。条件是否达成，就完全可以用condition来管理。这个解决的问题，不是互斥，而是条件等待。

```
触发通知(用来收发通知的东西);
// 一般有两种方式：
//   通知所有在等待的（notifyAll / broadcast）
//   通知一个在等待的（notifyOne / signal）
```

### 7 新特性



### 8 原子操作



### 9 windows临界区和其他互斥量



### 10 线程池和更多的解决方法 

