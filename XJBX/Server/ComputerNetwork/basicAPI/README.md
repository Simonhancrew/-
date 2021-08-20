## 网络编程

#### [端序](./code/judgeEndian.cpp):

一个整数在内存排列中的顺序将影响他被加载成为整数的值，一般pc选用的都是小端序。而网络序一般都是大端序。一般我们默认把发送的数据转换成为大端序之后再去发送，接收端总是采用大端序接收，考虑需不需要转换的问题。大端序一般是高位放在内存的低位。linux的netinet/in.h提供了4个函数完成端序的转换，端序的判断和系统转换接口详见[judgeEndian.c](judgeEndian.c)

### [Socket地址](SocketAndAddress.md)

简单记录socket创建流程，具体code详见[code](./code)
