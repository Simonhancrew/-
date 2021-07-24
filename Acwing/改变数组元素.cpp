#include <iostream>
#include <cstring>
#include <algorithm>

// 修改数组中一个区间的元素,第一反应应该就想到差分.
// 第一种是从后往前维护一个区间

using namespace std;

const int N = 2e5 + 10;
int a[N];

// int main(){
//     int t;
//     cin >> t;
//     while(t --){
//         int n;
//         cin >> n;
//         for(int i = 1;i <= n;i++) cin >> a[i];
//         int l = 0x3f3f3f3f;
//         for(int i = n;i >= 1;i--){
//             l = min(l,i - a[i] + 1);
//             if(l <= i) a[i] = 1;
//         }
//         for(int i = 1;i <= n;i++) cout << a[i] << ' ';
//         cout << endl;
//     }
// }


// 这里差分数组的意义就是某个区间的操作次数,看看每一点有没有被操作

int main()
{
    int t;
    cin >> t;
    while(t--){
        int n;
        scanf("%d", &n);
        memset(a,0,(n + 1) * 4);
        for(int i = 1;i <= n;i++){
            int x;
            scanf("%d", &x);;
            int l = max(1,i - x + 1),r = i;
            a[l]++,a[r + 1]--;
        }
        for(int i = 1;i <= n;i++){
            a[i] += a[i - 1];
            printf("%d ",!!a[i]);
        }
        puts("");
    }
}