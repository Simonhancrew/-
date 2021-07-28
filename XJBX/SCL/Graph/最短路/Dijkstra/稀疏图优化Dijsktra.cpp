#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>

using namespace std;

const int N = 150000;

int h[N],e[N],ne[N],w[N],idx;
int d[N];
bool st[N];

int n,m;

typedef pair<int, int> PII;

void add(int a,int b,int c)
{
    // w[i]，当前b到a的距离
    e[idx] = b,ne[idx] = h[a],w[idx] = c,h[a] = idx++;
}

// 堆优化dijkstra
int dijkstra(){
    // 1 初始距离
    memset(d,0x3f,sizeof d);
    d[1] = 0;
    // 2 利用最小堆来找不在确定集合中的最小距离
    priority_queue<PII,vector<PII>,greater<PII>> heap;
    heap.push({0,1});
    while(heap.size()){
        auto t = heap.top();
        heap.pop();
        int node = t.second,dis = t.first;
        // 2 按照第一个元素排序，可能存在冗余，直接判断就好
        if(st[node]) continue;
        // 3 将这个最小的距离加入确定集合
        st[node] = true;
        // 4 利用这个距离来更新他周围的点到起点的距离
        for(int i = h[node];i != -1;i = ne[i]){
            int j = e[i];
            if(d[j] > d[node] + w[i]){
                d[j] = d[node] + w[i];
                heap.push({d[j],j});
            }
        }
    }
    // 5 判断
    if(d[n] == 0x3f3f3f3f) return -1;
    return d[n];
}

int main()
{
    cin >> n >> m;
    memset(h,-1,sizeof h);
    // 邻接表的带权边
    while(m--){
        int x,y,z;
        cin >> x >> y >> z;
        add(x,y,z);
    }
    cout << dijkstra() << endl;
    return 0;
}