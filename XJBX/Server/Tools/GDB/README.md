## GDB调试

GDB熟悉的话看源码的时候下断点调试会方便很多

GDB主要做四件事帮助捕获bug

+ 在程序启动之前指定一些可以影响程序行为的变量或条件
+ 在某个指定的地方或条件下暂停程序
+ 在程序停止时检查已经发生了什么
+ 在程序执行过程中修改程序中的变量或条件，这样就可以体验修复一个 bug 的成果，并继续了解其他 bug

### 调试原理

一般要调试某个程序，为了能清晰地看到调试的每一行代码、调用的堆栈信息、变 量名和函数名等信息，需要调试程序含有调试符号信息。使用 gcc 编译程序时，如果加 上 -g 选项即可在编译后的程序中保留调试符号信息。

在实际生成调试程序时，一般不仅要加上 -g 选项，也建议关闭编译器的程序优化 选项。编译器的程序优化选项一般有五个级别，从 O0 ~ O4 

#### 启动gbd

主要有三种方式

1. gdb filename 直接调试程序
2. gdb attach pid 附加进程
3. gdb filename corename 调试core文件

对于知道进程id的,当用 gdb attach 上目标进程后，调试器会暂停下来，此时可以使用 continue 命令 让程序继续运行，或者加上相应的断点再继续运行程序。当调试完程序想结束此次调试时，而且不对当前进程 redis 有任何影响，也就是说 想让这个程序继续运行，可以在 GDB 的命令行界面输入 detach 命令让程序与 GDB 调试器分离，这样 redis 就可以继续运行了：

有时候，服务器程序运行一段时间后会突然崩溃，这并不是我们希望看到的，需要 解决这个问题。只要程序在崩溃的时候有 core 文件产生，就可以使用这个 core 文件 来定位崩溃的原因。当然，Linux 系统默认是不开启程序崩溃产生 core 文件这一机制 的，我们可以使用 ulimit -c 命令来查看系统是否开启了这一机制

#### 调试简介

首先编译，记得在编译的时候产生调试信息,另外建议观点-O2级别或是以上的编译优化。

```c
gcc hello_server.c -o -g hello_server
gdb ./hello_server
```

一般确认调试信息产生时，最后一行会有

```
Reading symbols from ./hello_server
```

另外还可以attach到一个进程上，注意要加入sudo权限

```
ps aux | grep nginx
sudo gdb attach 9390
```

当最后像结束调试的时候，且对于当前进程没有任何影响

```
detach
```

关于coredump文件的调试，首先的看看系统相关的设置。Linux 系统默认是不开启程序崩溃产生core文件的。

```
han@han-server:~$ ulimit -a
core file size          (blocks, -c) 0
data seg size           (kbytes, -d) unlimited
scheduling priority             (-e) 0
file size               (blocks, -f) unlimited
pending signals                 (-i) 47257
max locked memory       (kbytes, -l) 65536
max memory size         (kbytes, -m) unlimited
open files                      (-n) 1024
pipe size            (512 bytes, -p) 8
POSIX message queues     (bytes, -q) 819200
real-time priority              (-r) 0
stack size              (kbytes, -s) 8192
cpu time               (seconds, -t) unlimited
max user processes              (-u) 47257
virtual memory          (kbytes, -v) unlimited
file locks                      (-x) unlimited
```

可以用`ulimit -c unlimited`来在这次直接修改成无限大。

之后编译调试

```
ulimit -c unlimited
gcc -g -o core_dump core_dump.c
gdb core_dump core
```

常用的命令基本在下面

| 命令        | 简写    | 说明                                                   |
| ----------- | ------- | ------------------------------------------------------ |
| run         | r       | 运行一个程序                                           |
| continue    | c       | 让暂停的程序继续运行                                   |
| next        | n       | 运行到下一行                                           |
| step        | s       | 如果有调用函数，进入调用的函数内部，相当于   step into |
| until       | u       | 运行到指定行停下来                                     |
| finish      | fi      | 结束当前调用函数，到上一层函数调用处                   |
| return      | return  | 结束当前调用函数并返回指定值，到上一层函数调用处       |
| jump        | j       | 将当前程序执行流跳转到指定行或地址                     |
| print       | p       | 打印变量或寄存器值                                     |
| backtrace   | bt      | 查看当前线程的调用堆栈                                 |
| frame       | f       | 切换到当前调用线程的指定堆栈，具体堆栈通过堆栈序号指定 |
| thread      | thread  | 切换到指定线程                                         |
| break       | b       | 添加断点                                               |
| tbreak      | tb      | 添加临时断点                                           |
| delete      | del     | 删除断点                                               |
| enable      | enable  | 启用某个断点                                           |
| disable     | disable | 禁用某个断点                                           |
| watch       | watch   | 监视某一个变量或内存地址的值是否发生变化               |
| list        | l       | 显示源码                                               |
| info        | info    | 查看断点   / 线程等信息                                |
| ptype       | ptype   | 查看变量类型                                           |
| disassemble | dis     | 查看汇编代码                                           |
| set args    |         | 设置程序启动命令行参数                                 |
| show args   |         | 查看设置的命令行参数                                   |

### 实际看一下redis调试

```
redis-server
ps -ef | grep redis
sudo gdb attach threadid
```

