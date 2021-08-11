## 网络编程

#### [端序](judgeEndian.c):

一个整数在内存排列中的顺序将影响他被加载成为整数的值，一般pc选用的都是小端序。而网络序一般都是大端序。一般我们默认把发送的数据转换成为大端序之后再去发送，接收端总是采用大端序接收，考虑需不需要转换的问题。大端序一般是高位放在内存的低位。linux的netinet/in.h提供了4个函数完成端序的转换，端序的判断和系统转换接口详见[judgeEndian.c](judgeEndian.c)

#### [socket监听](socketListen.c):

初始socket，命名socket，调用listen监听socket。listen中的backlog提示监听队列的最大长度，超过这个长度不接受新的客户连接。

#### [socket接受连接](socketAccept.c):

accept只是从监听的队列中取出连接，对于连接的状态和变化一概不知。成功的时候返回一个socketfd，这个socketfd唯一的标识了被接受的这个连接。服务器可以通过这个fd来与客户端通信

#### [数据send](tcpSend.c)

服务器调用listen被动接受连接，client需要主动connect

```
#include <sys/socket.h>
int connect(int sockfd,const struct sockaddr *server_addr,socklen_t addresslen)
```

sockfd参数由系统调用返回一个socket，server_addr是服务器监听的socket。成功的时候返回0，sockfd唯一的标识了这个连接，客户端可以用sockfd来与服务器通信。

