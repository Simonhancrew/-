#include <cstring>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

#define SPFA

#ifdef DIJ_SIMPLE
// 朴素dij
const int N = 110,INF = 0x3f3f3f3f;
int g[N][N];
int d[N];
bool st[N];

class Solution {
public:
    int n;
    void dijkstra(int start){
        memset(d,0x3f,sizeof d);
        memset(st,0,sizeof st);
        d[start] = 0;
        for(int i = 0;i < n - 1;i++){
            int t = 0;
            for(int j = 1;j <= n;j++){
                cout << "juge" << (!st[j] && (t == 0 || d[t] > d[j])) <<endl;
                if(!st[j] && (t == 0 || d[t] > d[j])){
                    t = j;
                }
            }
            cout << t << endl;
            st[t] = 1;
            for(int j = 1;j <= n;j++){
                d[j] = min(d[j],d[t] + g[t][j]);
            }
        }
    }
    int networkDelayTime(vector<vector<int>>& times, int _n, int k) {
        n = _n;
        memset(g,0x3f,sizeof g);
        for(auto& e:times){
            int a = e[0],b = e[1],c = e[2];
            g[a][b] = c;
        }
        dijkstra(k);
        int res = 1;
        for(int i = 1;i <= n;i++) res = max(res,d[i]);
        if(res == INF) return -1;
        return res;
    }
};
#endif

#ifdef DIJ_HEAP
//堆优化dij

const int N = 110,M = 6010,INF = 0x3f3f3f3f;
int e[M],ne[M],h[N],w[M],idx;
int d[N];
bool st[N];

class Solution {
public:
    void dijkstra(int start){
        memset(d,0x3f,sizeof d);
        memset(st,0,sizeof st);
        d[start] = 0;
        typedef pair<int,int> PII;
        priority_queue<PII,vector<PII>,greater<PII>> q;
        q.push({0,start});
        while(q.size()){
            auto t = q.top();
            q.pop();
            int node = t.second,dis = t.first;
            if(st[node]) continue;
            st[node] = true;
            for(int i = h[node];~i;i = ne[i]){
                int j = e[i];
                if(d[j] > d[node] + w[i]){
                    d[j] = d[node] + w[i];
                    q.push({d[j],j});
                }
            }
        }
    }
    void add(int a,int b,int c){
        e[idx] = b,ne[idx] = h[a],w[idx] = c,h[a] = idx++;
    }
    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        idx = 0;
        memset(h,-1,sizeof h);
        for(auto& e:times){
            int a = e[0],b = e[1],c = e[2];
            add(a,b,c);
        }
        dijkstra(k);
        int res = 1;
        for(int i = 1;i <= n;i++) res = max(res,d[i]);
        if(res == INF) return -1;
        return res;
    }
};
#endif

#ifdef FLOYD
//floyd
const int N = 110,INF = 0x3f3f3f3f;
int dp[N][N];
class Solution {
public:
    int n;
    void floyd(){
        for(int k = 1;k <= n;k++){
            for(int i = 1;i <= n;i++){
                for(int j = 1;j <= n;j++){
                    dp[i][j] = min(dp[i][j],dp[i][k] + dp[k][j]);
                }
            }
        }
    }
    int networkDelayTime(vector<vector<int>>& times, int _n, int k) {
        n = _n;
        for(int i = 1;i <= n;i++){
            for(int j = 1;j <= n;j++){
                if(i == j) dp[i][j] = 0;
                else dp[i][j] = INF;
            }
        }
        for(auto& e:times){
            int a = e[0],b = e[1],c = e[2];
            dp[a][b] = c;
        }
        floyd();
        int res = 0;
        for(int i = 1;i <= n;i++) res = max(res,dp[k][i]);
        if(res == INF) return -1;
        return res;
    }
};
#endif

#ifdef SPFA
//spfa

// 初始邻接表和判断数据以及距离
const int N = 110,M = 6010,INF = 0x3f3f3f3f;
int d[N];
int e[M],ne[M],h[N],w[M],idx;
bool st[N];

class Solution {
public:
    void add(int a,int b,int c){
        e[idx] = b,ne[idx] = h[a],w[idx] = c,h[a] = idx++;
    }

    void spfa(int start){
        memset(d,0x3f,sizeof d);
        d[start] = 0;
        queue<int> q;
        q.push(start);
        while(q.size()){
            int t = q.front();
            q.pop();
            st[t] = false;
            for(int i = h[t];~i;i = ne[i]){
                int j = e[i];
                if(d[j] > d[t] + w[i]){
                    d[j] = d[t] + w[i];
                    if(!st[j]){
                        st[j] = true;
                        q.push(j);
                    }
                }
            }
        }
    }

    int networkDelayTime(vector<vector<int>>& times, int n, int k) {
        memset(h,-1,sizeof h);
        idx = 0;
        for(auto& e:times){
            int a = e[0],b = e[1],c = e[2];
            add(a,b,c);
        }
        spfa(k);
        int res = 1;
        for(int i = 1;i <= n;i++) res = max(res,d[i]);
        if(res == INF) return-1;
        return res;
    }
};
#endif

#ifdef BELLMAN
//bellman_ford(可以过)
const int N = 6010,INF = 0x3f3f3f3f;
struct Edge{
    int a,b,c;
}edge[N];

int d[110],cp[110];

class Solution {
public:
    void bellman_ford(int start){
        memset(d,0x3f,sizeof d);
        d[start] = 0;
        for(int i = 0;i < n;i++){
            memcpy(cp,d,sizeof cp);
            for(int j = 0;j < m;j++){
                int a = edge[j].a,b = edge[j].b,c = edge[j].c;
                d[b] = min(d[b],cp[a] + c);
            }
        }
    }
    int n,m;
    int networkDelayTime(vector<vector<int>>& times, int _n, int k) {
        n = _n,m = times.size();
        int idx = 0;
        for(auto &e:times){
            int a = e[0],b = e[1],c = e[2];
            edge[idx++] = {a,b,c};
        }
        bellman_ford(k);
        int res = 1;
        for(int i = 1;i <= n;i++) res = max(res,d[i]);
        if(res >= INF / 2) return -1;
        return res;
    }
};
#endif