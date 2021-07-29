#include <cstring>
#include <iostream>
#include <algorithm>

//pirm算法和dijsktra算法很像，都是首先初始距离无穷大
//之后去做n次更新，每次找到不在集合中的距离集合的最近的点
//然后把他加入集合，更新生成权值，此时如果他离集合的距离是无穷大的话就代表树不连通
//最后更新其周围的点到集合的距离（dijsktra是到起点）
using namespace std;

const int N = 510,INF = 0x3f3f3f3f;

int g[N][N],d[N];
int n,m;
bool st[N];

int prim(){
    // 集合st中是已经建好的树
    // 1 初始化距离都为无穷大
    memset(d,0x3f,sizeof d);
    // d[1] = 0;
    int res = 0;
    // 2 更新n次
    for(int i = 0;i < n;i++){
        int t = -1;
        // 找到不在集合中距离集合最小的节点
        for(int j = 1;j <= n;j++){
            if(!st[j] && (t == -1 || d[t] > d[j])){
                t = j;
            }
        } 
        // 3 第一次不更新距离，如果某次t到集合的距离是INF则无法联通
        // 最近都是无穷大，必不可能联通
        if(i && d[t] == INF) return INF;
        // 4 更新距离，更新剩下节点到集合的距离。//考虑负环的情况，需要先更新距离再去更新d 
        if(i) res += d[t]; 
        for(int j = 1;j <= n;j++) d[j] = min(d[j],g[t][j]);
        // 5 将这个节点加入生成集合
        st[t] = true;
    }
    return res;
}

int main()
{
    cin >> n >> m;
    // 初始化无穷大
    memset(g,0x3f,sizeof g);
    while(m--){
        int u,v,w;
        cin >> u >> v >> w;
        g[u][v] = min(g[u][v],w);
        g[v][u] = min(g[u][v],w);
    }
    int t = prim();
    if(t == INF) puts("impossible");
    else cout << t << endl;
    return 0;
}