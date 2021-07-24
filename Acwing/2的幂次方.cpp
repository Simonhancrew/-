#include <iostream>
#include <cstring>
#include <algorithm>
#include <cmath>


/*
    1 分析，要处理出log2（x）
    2 log2(x)里的x还需要继续处理吗？x > 1的时候
    3 重复调用一个处理，递归
    4 剩下x - 2 log2(x)log2(x)部分继续递归
    5 边界条件？什么时候停？x == 0
    
*/
using namespace std;

int n;

void solve(int u){
    if(n == 0) return;
    
    int p = log2(u);
    if(p == 0) cout << "2(0)";
    else if(p == 1) cout << "2";
    else if(p > 1) {
        cout << "2(";
        solve(p);
        cout << ")";
    }
    
    
    if(u != (1 << p)){
        cout << '+';
        solve(u - (1 << p));
    }
}

int main()
{
    while(cin >> n){
        solve(n);
        puts("");
    }
    return 0;
}