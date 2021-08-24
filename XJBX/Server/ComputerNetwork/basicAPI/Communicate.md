## How to communicate?

连接的根本目的是为了数据的收发，我们天然的不能感受到连接的建立，但是能够实际的感受到数据的交互。

### 1 发送数据

发送数据常用的函数就三个

```
ssize_t write (int socketfd, const void *buffer, size_t size)
ssize_t send (int socketfd, const void *buffer, size_t size, int flags)
ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags)
```

这里具体区分一下他们的使用场景：

1. 第一个函数是常见的文件写函数，如果把 socketfd 换成文件描述符，就是普通的文件写入。
2. 如果想指定选项，发送带外数据，就需要使用第二个带 flag 的函数。所谓带外数据，是一种基于 TCP 协议的紧急数据，用于客户端 - 服务器在特定场景下的紧急处理。
3. 如果想指定多重缓冲区传输数据，就需要使用第三个函数，以结构体 msghdr 的方式发送数据。

另外的一个坑就是，虽然socketfd是一种fd，但是他的write操作和其余的文件fd有一定的不同。

普通的文件描述符，一个文件的fd代表了一个打开的文件handler，通过write，操作系统内核会帮助我们不断的往文件系统中写入字节流，写入的字节流通常会和最后的size大小相等，否则就出错了。

**但是**，对于一个套接字而言，这个fd代表了一个双向的连接，在这个socketfd上调用write的字符很可能是比写入的实际请求量少的，这在普通文件fd下就直接挂壁了。

操作系统和内核在莫名的场景下，为读取数据和发送数据做了n多看不到的工作。主要的内容就是缓冲区的概念。

### 2 发送缓冲区

最关键的概念就是，发送缓冲区，这个TCP连接三次握手之后，操作系统内核会为每一个连接创建的基础设施。

缓冲区的大小可以由套接字选项设置，当我们调用write的时候，实际做的事情就是把数据从应用程序拷贝到内核的发送缓冲区中，并不一定是把数据直接发出去了。

因此也要讨论集中情况：

1. 缓冲区足够大，能容纳全部的数据，返回的值就直接是size
2. 缓冲区不足够大的时候，就比较尴尬，这个时候，应用程序会处于一个阻塞的状态，操作系统内核并不会直接返回。

接下来的事情就是关于返回的了，每个操作系统内核的处理是不同的。大部分 UNIX 系统的做法是一直等到可以把应用程序数据完全放到操作系统内核的发送缓冲区中，再从系统调用中返回。

可以想象，这就像一个工作队列，一端不断的发送数据出去，等到队列中又能读的时候，继续从应用程序搬一部分数据到发送缓冲区的，知道全部的数据都经由发送缓冲区走了一遍。

### 3 读取数据

按照linux的everything is file的描述，sockfd和file fd是没有区别的，为本地文件服务的函数也能为socketfd服务。

```
ssize_t read (int socketfd, void *buffer, size_t size)
```

read函数下，内核从socketfd读取最多size字节的数据，并且将这些数据转存到buffer中。返回值告诉我们读取的实际字节数目。

特殊情况下，返回0，表示EOF，表示对端发送了fin包，连接要断开了。返回-1，表述出错（当然，非阻塞的时候另当别论）

如果你每次都要读size字节的话，就需要人为写如下函数了

```
/* 从socketfd描述字中读取"size"个字节. */
size_t readn(int fd, void *buffer, size_t size) {
    char *buffer_pointer = buffer;
    int length = size;

    while (length > 0) {
        int result = read(fd, buffer_pointer, length);

        if (result < 0) {
            if (errno == EINTR)
                continue;     /* 考虑非阻塞的情况，这里需要再次调用read */
            else
                return (-1);
        } else if (result == 0)
            break;                /* EOF(End of File)表示套接字关闭 */

        length -= result;
        buffer_pointer += result;
    }
    return (size - length);        /* 返回的是实际读取的字节数*/
}
```

这里的非阻塞直接继续调用read，不涉及多路复用的配合。

### 4 缓冲区实验

服务端代码

````
#include "header.h"

void read_data(int sockfd) {
    ssize_t n;
    char buf[1024];

    int time = 0;
    for (;;) {
        fprintf(stdout, "block in read\n");
        if ((n = readn(sockfd, buf, 1024)) == 0)
            return;

        time++;
        fprintf(stdout, "1K read for %d \n", time);
        usleep(1000);
    }
}


int main(int argc, char **argv) {
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(12345);

    /* bind到本地地址，端口为12345 */
    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    /* listen的backlog为1024 */
    listen(listenfd, 1024);

    /* 循环处理用户请求 */
    for (;;) {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
        read_data(connfd);   /* 读取数据 */
        close(connfd);          /* 关闭连接套接字，注意不是监听套接字*/
    }
}
````

客户端代码

```
#include "header.h"

#define MESSAGE_SIZE 102400

void send_data(int sockfd) {
    char *query;
    query = malloc(MESSAGE_SIZE + 1);
    for (int i = 0; i < MESSAGE_SIZE; i++) {
        query[i] = 'a';
    }
    query[MESSAGE_SIZE] = '\0';

    const char *cp;
    cp = query;
    size_t remaining = strlen(query);
    while (remaining) {
        int n_written = send(sockfd, cp, remaining, 0);
        fprintf(stdout, "send into buffer %ld \n", n_written);
        if (n_written <= 0) {
            error(1, errno, "send failed");
            return;
        }
        remaining -= n_written;
        cp += n_written;
    }

    return;
}

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;

    if (argc != 2)
        error(1, 0, "usage: tcpclient <IPaddress>");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(12345);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    int connect_rt = connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    if (connect_rt < 0) {
        error(1, errno, "connect failed ");
    }
    send_data(sockfd);
    exit(0);
}
```

