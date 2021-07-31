#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_THREAD 4
#define INC 100000

using namespace std;

#if 0
int main(int argc,char *arcv[]){
    int i = 0;
    ++i;
    return 0;
}
#endif

static int num = 0;

void *increase(void *arg){
    for(int i = 0;i < INC;i++){
        num++;
    }
    printf("end\n");
    pthread_exit(NULL);
}


int main(){
    pthread_t threadArry[MAX_THREAD];
    for(int i = 0;i < MAX_THREAD;i++){
        pthread_create(&threadArry[i],NULL,increase,NULL);
    }

    for(int i = 0;i < MAX_THREAD;i++){
        pthread_join(threadArry[i],NULL);
    }
    printf("Real:%d,fake:%d\n",num,MAX_THREAD * INC);
    return 0;
}