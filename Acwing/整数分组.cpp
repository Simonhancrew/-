#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 5010;
int q[N],l[N];
int dp[N][N];

int n,k;

int main()
{
    cin >> n >> k;
    if(n == k){
        cout << k << endl;
        return 0;
    }
    
    for(int i = 1;i <= n;i++) cin >> q[i];
    sort(q + 1,q + n + 1);
    for(int i = 1;i <= n;i++){
        int p = i;
        while(p > 1 && q[i] - q[p - 1] <= 5) p--;
        l[i] = p;
    }
    for(int i = 1;i <= n;i++){
        for(int j = 1;j <= k;j++){
            dp[i][j] = dp[i - 1][j];//不选i
            dp[i][j] = max(dp[i][j],dp[l[i] - 1][j - 1] + i - l[i] + 1);
        }
    }
    cout << dp[n][k] << endl;
    return 0;
}