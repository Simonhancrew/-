#include <iostream>
#include <cstring>
#include <algorithm>

// 贪心，不要想复杂了。就遍历所有可能的数，贪心的变数组中的元素，一次变k个。
// 找到最小的那个变化的结果。

using namespace std;

const int N = 1e5 + 10;
int a[N];


int main(){
    int t;
    cin >> t;
    while(t--){
        int n,k;
        cin >> n >> k;
        for(int i = 1;i <= n;i++) cin >> a[i];
        int res = n;
        for(int i = 1;i <= 100;i++){
            int s = 0;
            for(int j = 1;j <= n;j++){
                if(a[j] != i){
                    s++;
                    j += k - 1;
                }
            }
            res = min(res,s);
        }
        cout << res << endl;
    }
    return 0;    
}