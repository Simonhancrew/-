#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

// 因为数据唯一，行能确定列，列能确定行

const int N = 510;
int ans[N][N],tmp[N * N];

int main()
{
    int t;
    cin >> t;
    while(t--){
        int n,m;
        cin >> n >> m;
        for(int i = 1;i <= n;i++){
            for(int j = 1;j <= m;j++){
                int x;
                cin >> x;
                tmp[x] = j;
            }
        }
        
        for(int i = 1;i <= m;i++){
            for(int j = 1;j <= n;j++){
                int x;
                cin >> x;
                ans[j][tmp[x]] = x;
            }
        }
        
        for(int i = 1;i <= n;i++){
            for(int j = 1;j <= m;j++){
                cout << ans[i][j] << ' ';
            }
            puts("");
        }
    }
    
    return 0;
}