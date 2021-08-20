#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc,char *argv[]){
    if(argc < 2){
        //printf()
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);


    struct sockaddr_in address;
    bzero(&address,sizeof address);
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);//ip绑一下
    address.sin_port = htons(port);

    int sock = socket(PF_INET,SOCK_STREAM,0);
    assert(sock >= 0);

    int ret = bind(sock,(struct sockaddr*)&address,sizeof address);
    assert(res != -1);

    ret = listen(sock,5);//backlog取经典值5
    assert(ret != -1);

    sleep(20);//睡20，等待连接的相关操作

    //accept,远端的socket信息
    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);
    int connfd = accept(sock,(struct sockaddr*)&client,&client_len);

    if(connfd < 0){
        printf("errno:%d\n",errno);
    }else{
        //成功连接，打出端口号和ip
        char remote[INET_ADDRSTRLEN];
        printf("ip:%s,port:%d",inet_ntop(AF_INET,&client.sin_addr,remote,client_len),ntohs(client.sin_port));
        close(connfd);//关闭连接
    }
    close(sock);
    return 0;
}