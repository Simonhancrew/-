#include <bits/stdc++.h>

using namespace std;
const int N =  110;

int t;
int n;
int cnt[N];

// cnt统计一下整个集群中出现过的数的次数
// 要让数组的mex最大，应该尽量让出现多次的数分给两个集合，至少是一个的
// 所以，从小遍历到大，0，或者1出现的时候，不够分了，一个数组的mex定下来了
// 再遍历一次，0出现的时候，另外一个数组的mex又被固定了。

int main()
{
    cin >> t;
    while(t --){
        memset(cnt,0,sizeof cnt);
        cin >> n;
        for(int i = 0;i < n;i++){
            int t;
            cin >> t;
            cnt[t]++;
        }
        int res = 0;
        for(int i = 0;i < N;i++){
            if(cnt[i] < 2){
                res = i;
                break;
            }
        }
        for(int i = 0;i < N;i++){
            if(cnt[i] < 1){
                res += i;
                break;
            }
        }
        cout << res << endl;
    }
    return 0;
}