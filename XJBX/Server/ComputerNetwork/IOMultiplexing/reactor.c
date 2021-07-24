#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


#define BUFFER_LENGTH 4096
#define MAX_EPOLL_EVENTS 1024
//port 8888
typedef int NCALLBACK(int,int,void*);

//并发小的时候select就可以，百万千万并发的时候，就要考虑epoll
//然后我们思考如何管理这些连接，如何管理这些数据？--->
//1 io如何存储？ 2 对应每一个io，他有那些属性？
//引入反应堆（reactor），一堆io，每个都有可读可写的事件


//io ---> event,把以前io为单位的改为一个个事件
//把以前的accept和recvs/end关注的变成两个状态：读和写
struct hanevent{
    int fd;
    //读写事件
    int events; //方便了解现在监控的是什么事件（read/wrrite）
    int *arg;//传参，回调函数里
    //触发的时候回调函数
    int (*callback)(int fd,int events,void *arg);

    int status;//在不在反应堆里(epoll里)
    //接收区域
    char buffer[BUFFER_LENGTH];
    //send数据和recv数据的时候长度要知道
    int length;
    long last_active;
};


struct hanreactor{
    //epoll的epollfd，io的event事件集合
    int epfd;
    //一个数组，这里不能用柔性数组（数组不知道有多长，但是空间已经分配好了）
    struct hanevent *events;//可以用rbtree，这里存根节点，hanevent的fd当作key
};

int recv_callback(int fd, int events, void *arg);
int send_callback(int fd, int events, void *arg);

//fd->events
int han_event_set(struct hanevent *ev,int fd, NCALLBACK cb,void *arg){
    ev->fd = fd;
    ev->callback = cb;
    ev->arg = arg;
    return 0;
    //后面所有的都是一个事件，只要经过了set，抽象成为一个事件
}


//epoll oper,往里面加入一个新来的io
int han_event_add(int epfd,int events,struct hanevent *ev){
    struct epoll_event ep_ev = {0};
    
    ep_ev.data.ptr = ev;
    ep_ev.events = ev->events = events;//epollin/out
    
    
    int op = 0;
    if(ev->status == 1){
        op = EPOLL_CTL_MOD;
    }else{
        op = EPOLL_CTL_ADD;
        ev->status = 1; 
    }

    //在就update，不在才添加
    if(epoll_ctl(epfd,op,ev->fd,&ep_ev)){
        //添加失败
        return -1;
    }
    return 0;
}

//1 close 2 监听到write/read，需要更改update
//LT一直触发的话也要删（我要一次性读完） 
int han_event_del(int epfd,struct hanevent *ev){
    struct epoll_event ep_ev;
    if(ev->status != 1){
        return -1;
    }
    ep_ev.data.ptr = ev;
    ev->status = 0;
    epoll_ctl(epfd,EPOLL_CTL_DEL,ev->fd,&ep_ev);
    return 0;
}


//分包，没接收完再次关注是否可读，多次调用recvcallback
int recv_callback(int fd,int events,void *arg){
    struct hanreactor *reactor = (struct hanreactor*)arg;
	struct hanevent *ev = reactor->events+fd;


    //recv没完
	int len = recv(fd, ev->buffer, BUFFER_LENGTH, 0);
	han_event_del(reactor->epfd, ev);

	if (len > 0) {
		
		ev->length = len;
		ev->buffer[len] = '\0';

        //没读完的话，


        //此处end，设置可写
		printf("C[%d]:%s\n", fd, ev->buffer);

		han_event_set(ev, fd, send_callback, reactor);
		han_event_add(reactor->epfd, EPOLLOUT, ev);
		
		
	} else if (len == 0) {
        //io已经调用close
		close(ev->fd);
		printf("[fd=%d] pos[%ld], closed\n", fd, ev-reactor->events);
		 
	} else {
		close(ev->fd);
		printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));
	}

	return len;
}


int send_callback(int fd,int events,void *arg){
    struct hanreactor *reactor = (struct hanreactor*)arg;
    if(reactor == NULL) return -1;

	struct hanevent *ev = reactor->events+fd;

    int len = send(fd, ev->buffer, ev->length, 0);
    
    if (len > 0) {
		printf("send[fd=%d], [%d]%s\n", fd, len, ev->buffer);

		han_event_del(reactor->epfd, ev);
		han_event_set(ev, fd, recv_callback, reactor);
		han_event_add(reactor->epfd, EPOLLIN, ev);
		
	} else {

		close(ev->fd);

		han_event_del(reactor->epfd, ev);
		printf("send[fd=%d] error %s\n", fd, strerror(errno));

	}

	return len;
}



int accept_callback(int fd,int events,void *arg){
    struct hanreactor *reactor = (struct hanreactor*)arg;
    if (reactor == NULL) return -1;

    struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
    int clientfd = accept(fd, (struct sockaddr*)&client_addr, &len);
    if(clientfd == -1){
        if(errno != EAGAIN && errno != EINTR){

        }
        printf("acc:%s\n",strerror(errno));
        return -1;
    }
    int i = 1;
    do{ 
        for (i = 0;i < MAX_EPOLL_EVENTS;i ++) {
			if (reactor->events[i].status == 0) {
				break;
			}
		}
        if (i == MAX_EPOLL_EVENTS) {
			printf("%s: max connect limit[%d]\n", __func__, MAX_EPOLL_EVENTS);
			break;
		}

		int flag = 0;
		if ((flag = fcntl(clientfd, F_SETFL, O_NONBLOCK)) < 0) {
			printf("%s: fcntl nonblocking failed, %d\n", __func__, MAX_EPOLL_EVENTS);
			break;
		}

        han_event_set(&reactor->events[clientfd], clientfd, recv_callback, reactor);
		han_event_add(reactor->epfd, EPOLLIN, &reactor->events[clientfd]);

    }while(0);

    printf("new connect [%s:%d][time:%ld], pos[%d]\n", 
		inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), reactor->events[i].last_active, i);


    return 0;
}



// 就是redis里面的ae里的baseloop，ngx和libevent也是类似的写法
int hanreactor_run(struct hanreactor *reactor){
    if(reactor == NULL) return -1;
    if(reactor->epfd < 0) return -1;
    if(reactor->events == NULL) return -1; 

    struct epoll_event events[MAX_EPOLL_EVENTS];
    //baseloop
    while(1){
        int nready = epoll_wait(reactor->epfd,events,MAX_EPOLL_EVENTS,1000);
        if(nready < 0){
            continue;
        }
        int i = 0;
        for(i = 0;i < nready;i++){
            struct hanevent *ev = (struct hanevent *)events[i].data.ptr;
            //一起去做，避免两个状态不一致。这里应用层和epoll的做一个分离
            //不是必要的
            //read
            if((ev->events & EPOLLIN) && (events[i].events & EPOLLIN)){
                ev->callback(ev->fd,events[i].events,ev->arg);
            }
            //write
            if((ev->events & EPOLLOUT) && (events[i].events & EPOLLOUT)){
                ev->callback(ev->fd,events[i].events,ev->arg);
            }
        }
    }  
}

//管理服务器端众多io事件
int hanreactor_init(struct hanreactor *reactor){
    //1 epoll
    if(reactor == NULL) return -1;
    memset(reactor,0,sizeof(struct hanreactor));
    reactor->epfd = epoll_create(1);
    if(reactor->epfd < 0){
        return -2;
    }
    //2 事件放一起
    reactor->events = (struct hanevents*)malloc((MAX_EPOLL_EVENTS) * sizeof(struct hanevent));
    if(reactor ->events == NULL){
        close(reactor->epfd);
        return -3;
    }
    return 0;
}

int hanreactor_destroy(struct hanreactor *reactor){
    close(reactor->epfd);
    free(reactor->events);
}


//tcp server
int init_sock(short port){
    int fd = socket(AF_INET,SOCK_STREAM,0);
    fcntl(fd,F_SETFL,O_NONBLOCK);
    struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port);

    bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (listen(fd, 20) < 0) {
		printf("listen failed : %s\n", strerror(errno));
	}

	return fd;
}

int hanreactor_addlistener(struct hanreactor *reactor,int sockfd,NCALLBACK *acceptor){
    if (reactor == NULL) return -1;
	if (reactor->events == NULL) return -1;

    han_event_set(&reactor->events[sockfd], sockfd, acceptor, reactor);
	han_event_add(reactor->epfd, EPOLLIN, &reactor->events[sockfd]);

	return 0;

}

int main(){
    unsigned short port = 8888;
    int sockfd = init_sock(port);
    
    struct hanreactor *reactor = (struct hanreactor*)malloc(sizeof(struct hanreactor));    
    hanreactor_init(reactor);

    hanreactor_addlistener(reactor, sockfd, accept_callback);
    hanreactor_run(reactor);

	hanreactor_destroy(reactor);
    close(sockfd);
    return 0;
}