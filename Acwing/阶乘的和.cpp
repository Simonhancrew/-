#include <iostream>
#include <cstring>
#include <algorithm>

//1 根据给定数据范围确定算出一个阶乘范围的数组
//2 dfs搜索每一个阶乘，拿与不拿。回溯(这里是两个状态分别深搜的)

using namespace std;

int fac[15];
int n;

void init(){
    fac[0] = 1;
    for(int i = 1;i < 15;i++){
        fac[i] = fac[i - 1] * i;
    }
}

bool dfs(int idx,int sum){
    if(idx > 15 || sum > n) return false;
    if(sum == n) return true;
    if(dfs(idx + 1,sum + fac[idx]) || dfs(idx + 1,sum)) return true;
    return false;
}

int main(){
    init();
    while(cin >> n && n >= 0){
        if(n == 0) puts("NO");
        else{
            if(dfs(0,0)) puts("YES");
            else puts("NO");
        }
    }    
    return 0;
}