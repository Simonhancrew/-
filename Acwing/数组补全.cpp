#include <iostream>

using namespace std;

// 补全的时候，考虑中位数y，>=y的有n/2 + 1个数，小于y的数有n/2个。
// 所以在前k个的输入中，统计以下sum，大于等于y的有多少个
// 之后小于y的部分补1，大于y的补y就行了。

const int N = 1010;

int a[N];

int main()
{
    int n,k,p,x,y;
    int gy = 0;
    int sum = 0;
    
    cin >> n >> k >> p >> x >> y;
    for(int i = 1;i <= k;i++){
        cin >> a[i];
        sum += a[i];
        if(a[i] >= y) gy++; 
    }
    if(sum + (n / 2 - k + gy) + (n / 2 + 1 - gy) * y > x){
        cout <<  -1 << endl;
    }else{
        for(int i = 1;i <= n / 2 - k + gy;i++) cout << 1 << ' ';
        for(int i = 1;i <= n / 2 + 1 - gy;i++) cout << y << ' ';
    }
    
    return 0;
}