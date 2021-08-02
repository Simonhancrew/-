#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 110;

int n,m;
int res[N][N];

int main()
{
    cin >> n >> m;
    // 1 方向数组考虑方向的问题
    int dx[] = {0,1,0,-1},dy[] = {1,0,-1,0};
    // 2 遍历所有放过的数，往一个方向走，当走到边界或者走到的位置放了数的话
    // 就改变方向。
    for(int x = 0,y = 0,k = 1,d = 0;k <= m * n;k++){
        res[x][y] = k;
        int nx = x + dx[d],ny = y + dy[d];
        if(nx < 0 || ny < 0 || nx >= n || ny >= m || res[nx][ny]){
            d = (d + 1) % 4;
            nx = x + dx[d],ny = y + dy[d]; 
        }
        x = nx,y = ny;
    }
    for(int i = 0;i < n;i++){
        for(int j = 0;j < m;j++){
            cout << res[i][j] << ' ';
        }
        puts("");
    }
}