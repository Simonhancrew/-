#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

// 应该尽可能的移动每个到前面第一个箱子去，从前往后枚举每个箱子

const int N = 110;

int a[N];


int main()
{
    int t;
    cin >> t;
    while(t--){
        int n,d;
        cin >> n >> d;
        for(int i = 1;i <= n;i++) cin >> a[i];
        int res = a[1];
        int cur = 0;
        bool flag = true;
        for(int i = 2;i <= n;i++){
            while(a[i] && d >= i - 1){
                d -= (i - 1);
                res++;
                a[i]--;
            }
        }
        cout << res << endl;
    }
    return 0;
}