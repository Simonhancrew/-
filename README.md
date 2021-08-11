# Recipe:stay humble

暂无经历整理成gitpage，只能在此记录了解过的网络编程相关技术，以及语言细节，还有数据库和操作系统相关。

单纯的算法学习记录移到了另一个repo，[SCL](https://github.com/Simonhancrew/SCL)下，永远喜欢算法，会一直学习QAQ。

项目写好了的会放在最底下，八股文背完以后会单独完成repo的QAQ。

## Linux高性能服务器

1. [Basic API](XJBX/Server/ComputerNetwork/basicAPI):

   > something about connect,recv,listen,bind,etc.

2. [IO multiplexing](XJBX/Server/ComputerNetwork/IOMultiplexing):

   > select + epoll + reactor

3. [协程]()

   > 用户态线程，切换采用汇编实现，只符合x86 cpu构架

4. [minigrep](XJBX/Server/OS/minigrep/):

   > 如何用rust实现一个grep？
   
5. [服务器后台监控框架的实现](XJBX/Server/Tools/monitor)

   > 简单实现一个后台进程监控脚本框架,目前实现了进程监控和挂掉重启以及邮件通知
   >

6. [BloomFilter](XJBX/Project/Bloomfilter)：

   > 一个参考Cassandra中的BloomFilter实现，Hash选用MurmurHash2，通过双重散列公式生成散列函数参考：http://hur.st/bloomfilter

7. [Consistent Hasing](XJBX/Server/MiddleWare/consistentHashing/README.md):

   > 一致哈希的理解和实现
   
8. [ThreadPool](XJBX/Server/OS/ThreadPool/)

## 语言特性

1. 实现一个string类

   > [string.h](./XJBX/LingChar/string/hstring.h)

2. [左值和右值](/XJBX/LingChar/LRValue/README.md):

   > 函数构造学

3. [递归的复杂度](/XJBX/LingChar/Recursion/recursion.h):

   > 多情况下的复杂度判断

4. [智能指针](XJBX/LingChar/smartPointer/README.md):

   > C++11特性

5. [正则](XJBX/LingChar/Regexr/README.md):

   > 最全re记录，反正我会了，然后parse了一个pgn file作为例子

6. 内存管理

   > [new/delete/operator new/placement new](XJBX/LingChar/MemManagement/new++.md):内存分配cheatsheet
   >
   > [malloc](XJBX/LingChar/MemManagement/About_malloc.md)：阅读华庭的malloc书籍备忘
   >
   > [malloc.c](XJBX/LingChar/MemManagement/Code/shmalloc.c)：a init version of malloc

## 数据库

1. [Redis sample](XJBX/Server/DB/redis/HowtoUse/README.md):

   > Redis从编译到放弃 + 使用场景分析,另外在在算法模板中实现了[跳表](/XJBX/SCL/Datastruct/)
   
2. [Redis SourceCode](./XJBX/Server/DB/redis/SourceCodeStudy/README.md)

   > 源码的注释和理解

3. 动手实现一个数据库？ 

4. [高性能mysql详解](XJBX/Server/DB/Mysql/README.md)
   
   > ^_^

## Operating System

1. [虚拟化](XJBX/Server/OS/虚拟化)
2. 并发
3. 持久化

## 多线程

1. [Posix多线程编程翻译](XJBX/Server/OS/PosixThreadsProgramming/README.md)

## 计网

1. [分层](XJBX/Server/ComputerNetwork/Blog/分层.md)
2. [TCP/UDP](XJBX/Server/ComputerNetwork/Blog/TCP.md)
3. [机制](XJBX/Server/ComputerNetwork/Blog/TCP的一些机制.md)
4. [http1-3](XJBX/Server/ComputerNetwork/Blog/HTTP.md)
5. [http优化](XJBX/Server/ComputerNetwork/Blog/优化HTTP_1.1.md)
6. [短网址解析](XJBX/Server/ComputerNetwork/Blog/短网址解析.md)
7. [https](XJBX/Server/ComputerNetwork/Blog/HTTPs.md)
8. [设计角度三次握手分析](XJBX/Server/ComputerNetwork/Blog/Tcp的三次握手.md)
9. [四次挥手分析](XJBX/Server/ComputerNetwork/Blog/Tcp的四次挥手.md)
10. [抓包](XJBX/Server/ComputerNetwork/Blog/抓包.md)
11. [IP](XJBX/Server/ComputerNetwork/Blog/网络层.md)
12. 用户态协议栈的设计和C10M的研究

## 设计模式

1. 23个设计模式

## 编译原理

1. 现代编译原理
2. 链接、装载与库

## Contribute 365

每天[C++](/Solution/)，[Rust](/Solution/)，[Golang](/Solution/)，[Python3](/Solution/)的手感保持

## 效率工具

1. [linux命令](XJBX/Server/Tools/Command)
4. [docker](XJBX/Server/Tools//Docker)
5. [valgrind](XJBX/Server/Tools/Valgrind/README.md)
6. [gdb](XJBX/Server/Tools/GDB/README.md)
7. [简单实现一个Rpc](XJBX/Server/Tools/simpleRpc/README.md)
6. [zeromp](XJBX/Server/Tools/ZeroMq)
7. [CAS+QUE](XJBX/Server/Tools/CAS)
8. cmake

## 项目

1. [分布式私有云](XJBX/Project/CloudDisk/README.md)
2. Rookies
3. ahanDB
4. LeptServer

