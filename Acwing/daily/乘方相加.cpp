#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
typedef long long LL;
const int N = 55;
int s[N];

// 进制的问题。这里化成k进制，每一位至多只有一个1。因为第i次的时候只能给数组中一个数加上pow(k,i)
// 所以枚举所有的位，拿到每一位的数的k进制，然后在相对应的位置+1。结果判断有没有位置的数大于1就可以了。

int main()
{
    int t;
    cin >> t;
    while(t--){
        int n,k;
        cin >> n >> k;
        memset(s,0,sizeof s);
        while(n--){
            LL x;
            cin >> x;
            for(int i = 0;x;i++,x /= k){
                s[i] += x % k;
            }
        }
        bool flag = false;
        for(int i = 0;i < N;i++){
            if(s[i] > 1) {
                flag = true;
                break;
            }
        }
        if(flag) cout << "NO" <<endl;
        else cout << "YES" << endl;
    }
}、