#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

// 分析后发现，无论是在奇数区间内改变，还是在偶数区间中亮灯，都是一样的公式表示最大可能值
// 遍历枚举就可以了

const int N = 1e5 + 10;

int a[N],s1[N],s2[N];

int main(){
    int t;
    cin >> t;
    while(t --){
        int n,m;
        cin >> n >> m;
        for(int i = 1;i <= n;i++) cin >> a[i];
        a[++n] = m;
        for(int i = 1;i <= n;i++){
            s1[i] = s1[i - 1],s2[i] = s2[i - 1];
            if(i & 1) s1[i] += a[i] - a[i - 1];
            else s2[i] += a[i] - a[i - 1];
        }
        // cout << s1[n] << endl;
        int res = s1[n];
        for(int i = 1;i <= n;i++){
            int t = a[i] - a[i - 1];
            if(t == 1) continue;
            res = max(res,s1[i - 1] + s2[n] - s2[i] + t - 1);
        }
        cout << res << endl;
    }
}