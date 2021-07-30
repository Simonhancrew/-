#include <iostream>
#include <algorithm>

//kruskal相对的比较简单，只要存下所有的边就可以
//然后遍历这些边，看看二者在不在一个集合中，不在的话就把这条边加入
//集合可以选用并查集去做

using namespace std;

//存下所有的边就可以了
const int N = 2e5 + 10,INF = 0x3f3f3f3f;

int n,m;
struct Edge{
    int a,b,w;
    bool operator < (const Edge &E) const {
        return w < E.w;
    }
}edge[N];
//并查集的集合
int p[N];

int find(int x){
    if(p[x] != x){
        p[x] = find(p[x]);
    }
    return p[x];
}

int kruskal(){
    //按照权重从小到大排序所有的边
    sort(edge,edge + m);
    //初始并查集
    for(int i = 1;i <= n;i++) p[i] = i;
    //res是生成树最后的权重，cnt是经过的边数
    int res = 0,cnt = 0;
    for(int i = 0;i < m;i++){
        int a = edge[i].a,b = edge[i].b,w = edge[i].w;
        a = find(a),b = find(b);
        if(a != b){
            //加入集合
            p[a] = b;
            res += w;
            cnt ++;
        }
    }
    //应该会经过至少n-1条边
    if(cnt < n - 1) return INF;
    return res;
}

int main(){
    cin >> n >> m;
    
    for(int i = 0;i < m;i++){
        int a,b,w;
        scanf("%d%d%d",&a,&b,&w);
        edge[i] = {a,b,w};
    }


    int t = kruskal();

    if(t == INF) puts("impossible");
    else cout << t;

    return 0;
}

#if 0 // 2021/07/30 update
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 1e5 + 10,M = 2e5 + 10;

int n,m;
int p[N];
struct{
    int a,b,w;
}edge[M];

int find(int a){
    if(p[a] != a) p[a] = find(p[a]);
    return p[a];
}

int main()
{
    cin >> n >> m;
    // kruskal只需要遍历所有边就可以了
    for(int i = 0;i < m;i++){
        int u,v,w;
        cin >> u >> v >> w;
        edge[i] = {u,v,w};
    }
    // 1 按照边的权重排序这些边
    sort(edge,edge + m,[](auto& lhs,auto& rhs){ return lhs.w < rhs.w;});
    // 2 维护一个并查集，用于记录集合
    for(int i = 1;i <= n;i++) p[i] = i;
    int cnt = 0,res = 0;
    // 3 遍历所有的边，合并这些边到集合中
    for(int i = 0;i < m;i++){
        int a = edge[i].a,b = edge[i].b,w = edge[i].w;
        a = find(a),b = find(b);
        if(find(a) != find(b)){
            p[a] = b;
            res += w;
            cnt ++;
        }
    }
    // 4 如果加的边数小于n - 1说明是不连通的
    if(cnt < n - 1) puts("impossible");
    else cout << res << endl;
    return 0;
}
#endif