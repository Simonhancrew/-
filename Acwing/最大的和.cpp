#include <iostream>
#include <cstring>
#include <algorithm>

//贝壳找房面试题
using namespace std;
using ll = long long;

const int N = 1e5 + 10;

int n,k;
int a[N],st[N];

int main()
{
    cin >> n >> k;
    ll res = 0,add = 0;
    for(int i = 0;i < n;i++) cin >> a[i];
    for(int i = 0;i < n;i++){
        cin >> st[i];
        if(st[i] == 1){
            res += a[i];
        }
    }
    //滑动窗口 也就是双指针
    ll cur = 0;
    for(int i = 0;i < n;i++){
        if(i > k - 1 && st[i - k] == 0) cur -= a[i - k];
        if(st[i] == 0) cur += a[i];
        add = max(add,cur);
    }
    cout << res + add << endl;
}

//还可以考虑前缀和，不过要做两次，一次记录全部的前缀和，一次记录1状态的前缀和，这样减一下就可以拿到区间的add