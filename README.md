# Recipe:stay humble
## Basic Algorithm

1. [SCL](XJBX/SCL):

   > 基础算法模板，大部分使用C++实现。其中编译器要符合C01标准。

2. [BloomFilter](XJBX/Project/Bloomfilter)：

   > 一个参考Cassandra中的BloomFilter实现，Hash选用MurmurHash2，通过双重散列公式生成散列函数参考：http://hur.st/bloomfilter

3. [Consistent Hasing](XJBX/Server/MiddleWare/consistentHashing/README.md):

   > 一致哈希的理解和实现

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

## 语言特性

1. [左值和右值](/XJBX/LingChar/LRValue/README.md):

   > 函数构造学

2. [递归的复杂度](/XJBX/LingChar/Recursion/recursion.h):

   > 多情况下的复杂度判断

3. [智能指针](XJBX/LingChar/smartPointer/README.md):

   > C++11特性

4. [正则](XJBX/LingChar/Regexr/README.md):

   > 最全re记录，反正我会了，然后parse了一个pgn file作为例子
   
5. 内存管理

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
## 炼丹从入门到放弃

1. what is attention
2. transformer
3. vit
4. xxxx：行人属性检测的

## Operating System

1. [虚拟化](XJBX/Server/OS/虚拟化)
2. 并发
3. 持久化

## 多线程

1. [Posix多线程编程翻译](XJBX/Server/OS/PosixThreadsProgramming/README.md)

## 计网

1. [分层](XJBX/Server/ComputerNetwork/Blog/分层.md)
2. [TCP/UDP](XJBX/Server/ComputerNetwork/Blog/TCP.md)
3. [http1-3](XJBX/Server/ComputerNetwork/Blog/HTTP.md)
4. http优化
5. https优化
6. [设计角度三次握手分析](XJBX/Server/ComputerNetwork/Blog/Tcp的三次握手.md)
7. [四次挥手分析](XJBX/Server/ComputerNetwork/Blog/Tcp的四次挥手.md)
8. 抓包
9. 用户态协议栈的设计和C10M的研究

## 设计模式

1. 23个设计模式

## 编译原理

1. 现代编译原理
2. 链接、装载与库

## Contribute 365

每天[C++](/Solution/)，[Rust](/Solution/)，[Golang](/Solution/)，[Python3](/Solution/)的手感保持

## 效率工具

1. git详解
2. vim
3. [linux命令](XJBX/Server/Tools/Command)
4. [docker](XJBX/Server/Tools//Docker)
5. [valgrind](XJBX/Server/Tools/Valgrind/README.md)
6. [gdb](XJBX/Server/Tools/GDB/README.md)
7. [简单实现一个Rpc](XJBX/Server/Tools/simpleRpc/README.md)
8. cmake

## 项目

1. [分布式私有云](XJBX/Project/CloudDisk/README.md)
2. ahanDB
3. LeptServer

