#include <iostream>

using namespace std;

//现代机器一般使用小端序，但是网络之间的传输一般使用大端序
//接收处理需要保持端序一致

//测试端序
void byteorder(){

    //共用体占用的内存等于最长的成员占用的内存
    //共用体使用了内存覆盖技术，同一时刻只能保存一个成员的值
    union{
        short value;
        char union_byte[sizeof(short)];
    }test;
    test.value = 0x0102;
    //大端序，数字的高位存在了二进制的低位
    if((test.union_byte[0] == 1) && (test.union_byte[1] == 2)){
        printf("big endian\n");
    }else if((test.union_byte[0] == 2) && (test.union_byte[1] == 1)){
        printf("little endian");
    }else{
        printf("unknow");
    }
}
#if 1
int main(){
    byteorder();
    return 0;
}
#else

#include <netinet/in.h>

unsigned long int htonl(unsigned long int hostlong);
unsigned short int htons(unsigned short int hostshort);
//host to net long(short)

#endif