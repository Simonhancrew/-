#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 510;

int g[N][N];
int n,m;
int d[N];
bool st[N];

// 朴素dijkstra求最短路
int dijsktra(){
    // 1 初始的时候距离都为无穷大，起点距离为0
    memset(d,0x3f,sizeof d);
    d[1] = 0,st[1] = 0;
    // 2 遍历n次，找到起点到每个点的最小距离
    for(int i = 0;i < n - 1;i++){
        int t = -1;
        // 3 找到未确定最短距离，且距离起点值最小的点
        for(int j = 1;j <= n;j++){
            if(!st[j] && (t == -1 || d[t] > d[j])){
                t = j;
            }
        }
        // 4 之后这个点到起点的最小距离就确定了，加入集合st
        st[t] = 1;
        // 5 然后用这个确定了最小距离的点，更新一下周围的距离（注意，这些点的距离还是未确定最小的）
        for(int j = 1;j <= n;j++){
            d[j] = min(d[j],d[t] + g[t][j]);
        }
    }
    // 6 如果此时的距离还是无穷大，说明根本就不可能到达.否则就是一个可行解
    if(d[n] == 0x3f3f3f3f) return -1;
    return d[n];
}

int main()
{
    cin >> n >> m;
    memset(g,0x3f,sizeof g);
    // 稠密图的构建，因为每个点的边几乎拉满，没必要邻接表
    // 不是不行，是没必要。
    while(m--){
        int x,y,z;
        cin >> x >> y >> z;
        // 因为存在自环和重边，所以找到最短的就可以了。
        g[x][y] = min(g[x][y],z);
    }
    cout << dijsktra() << endl;
}