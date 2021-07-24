#include <iostream>
#include <cstring>
#include <algorithm>

// 注意是子序列，不是连续的序列

using namespace std;

const int N = 5010;
int a[N],mp[N];
int n;

bool check(int a[]){
    memset(mp,-1,sizeof mp);
    for(int i = 0;i < n;i++){
        if(mp[a[i]] == -1){
            mp[a[i]] = i;
        }else{
            if(mp[a[i]] != i - 1) return true; 
        }
    }
    return false;
}

int main()
{
    int t;
    scanf("%d",&t);
    while(t --){
        scanf("%d", &n);
        for(int i = 0;i < n;i++){
            scanf("%d", &a[i]);
        }
        if(check(a)) cout << "YES"<< endl;
        else puts("NO");
    }
}