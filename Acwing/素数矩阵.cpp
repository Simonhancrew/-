#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 110;
int a[N][N];

// 1不是质数，2是质数
bool is_prime(int x)  // 判定质数
{
    if (x < 2) return false;
    for (int i = 2; i <= x / i; i ++ )
        if (x % i == 0)
            return false;
    return true;
}


int main()
{
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        //思路很简单，是质数就全1
        if(is_prime(n)){
            for(int i = 0;i < n;i++){
                for(int j = 0;j < n;j++){
                    cout << 1 << ' ';
                }
                cout << endl;
            }
        }else{//不是质数就每行两个1，其余全0
            memset(a,0,sizeof a);
            for(int i = 0;i < n - 1;i++) a[i][i] = a[i][i + 1] = 1;
            a[n - 1][0] = a[n - 1][n - 1] = 1;
            for(int i = 0;i < n;i++){
                for(int j = 0;j < n;j++){
                    cout << a[i][j] << ' ';
                }
                cout << endl;
            }
        }
    }
    return 0;
}