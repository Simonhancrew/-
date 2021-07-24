#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

//构成完美矩阵的话，四个角是要一样的。找到中位数，然后去算一下距离就可以了
//行列是奇数的话，中心的那一行（列）是无法判断到的。

using LL = long long;
const int N = 110;

LL a[N][N];
int t;

int main()
{
    cin >> t;
    for(int i = 0;i < t;i++){
        int n,m;
        cin >> n >> m;
        LL res = 0;
        for(int i = 1;i <= n;i++){
            for(int j = 1;j <= m;j++){
                cin >> a[i][j];
            }
        }
        if(n & 1){
            for(int i = 1;i <= m / 2;i++){
                res += abs(a[n / 2 + 1][i] - a[n / 2 + 1][m - i + 1]);
            }
        }
        if(m & 1){
            for(int i = 1;i <= n / 2;i++){
                res += abs(a[i][m / 2 + 1] - a[n - i + 1][m / 2 + 1]);
            }
        }
        for (int i = 1; i <= n / 2; i ++) {
            for (int j = 1; j <= m / 2; j ++) {
                vector<LL> temp;
                temp.push_back(a[i][j]);
                temp.push_back(a[i][m - j + 1]);
                temp.push_back(a[n - i + 1][j]);
                temp.push_back(a[n - i + 1][m - j + 1]);
                sort(temp.begin(), temp.end());
                for (int k = 0; k < temp.size(); k ++) {
                    res += abs(temp[k] - temp[1]);
                }
            }
        }
        cout << res << endl;
    }
    return 0;
}