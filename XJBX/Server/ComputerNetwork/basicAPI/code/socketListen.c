#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

/*
1 通用socket地址
struct sockaddr{
    sa_family_t sa_family; //地址族类型
    char sa_data[14];   //存放socket的地址值
}
因为14字节的sa_data基本无法容纳多数的协议族地址，所以有

struct sockaddr_storage
  {
    __SOCKADDR_COMMON (ss_);	// Address family, etc. 
    char __ss_padding[_SS_PADSIZE];
    __ss_aligntype __ss_align;	// Force desired alignment. 
  };

*/


static bool stop = false;

static void handle_term(int sig){
    stop = true;
}

int main(int argc,char *argv[]){
    signal(SIGTERM,handle_term);

    if(argc < 3){
        printf("usgae:%s ip_address port_number backlogh\n",basename[argv[0]]);
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int backlog = atoi(argv[3]);

    int sock = socket(PF_INET,SOCK_STREAM,0);
    assert(sock > 0);

    //创建一个ipv4 socket地址
    struct sockaddr_in address;
    bzero(&address,sizeof address);
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);//present 2 number
    address.sin_port = htons(port);//变网络序

    //命名socket，给他地址绑定
    int ret = bind(sock,(struct sockaddr *)&address,sizeof address);
    assert(res != -1);

    ret = listen(sock,backlog);
    assert(res != -1);

    //循环等待，直到kill他
    while(!stop){
        sleep(1);
    }
    close(sock);
    return 0;
}



