#include <iostream>
#include <cstring>
#include <queue>

//在spfa算法上稍加改动，加一个cnt[x]，表示到x的最小距离的时候经过了多少条边
//根据抽屉原理，如果cnt[x] 大于等于n的话，说明经过了n+1个点
//此时图中应该存在负环

using namespace std;

const int N = 2010, M = 10010;

int h[N],e[M],ne[M],w[M],idx;
int n,m;
int d[N],cnt[N];
bool st[N];

void add(int a,int b,int c){
    e[idx] = b,ne[idx] = h[a],w[idx] = c,h[a] = idx++;

}

bool spfa(){
    //不需要初始化，此时不需要求距离的绝对值，只需要判断负环
    queue<int> q;
    //压入所有的点，有些点可能从1开始去不了
    for(int i = 1;i <= n;i++){
        q.push(i);
        st[i] = true;
    }
    while(q.size()){
        int t = q.front();
        q.pop();
        st[t] = false;
        for (int i = h[t];i != -1;i = ne[i]){
            int j = e[i];
            //不需要初始化d，有负环的话一定会循环无数次
            //有些点的距离一定是负无穷的
            //也可以理解为有一个虚拟源点，开始到所有的点的距离都是0
            //然后执行了一次更新，所有的入队列
            if(d[j] > d[t] + w[i]){
                d[j] = d[t] + w[i];
                cnt[j] = cnt[t] + 1;
                if(cnt[j] >= n) return true;
                if(!st[j]){
                    q.push(j);
                    st[j] = true;
                }
            }
        }
    }
    return false;
}


int main(){
    cin >> n >> m;
    memset(h,-1,sizeof h);
    while(m--){
        int a,b,c;
        scanf("%d%d%d",&a,&b,&c);
        add(a,b,c);
    }
    if(spfa()) puts("Yes");
    else puts("No");
    return 0;  
}

/*

在原图的基础上新建一个虚拟源点，从该点向其他所有点连一条权值为0的有向边。
那么原图有负环等价于新图有负环。此时在新图上做spfa，将虚拟源点加入队列中。
然后进行spfa的第一次迭代，这时会将所有点的距离更新并将所有点插入队列中。
执行到这一步，就等价于代码中的做法了。
上面的算法可以找到负环，等价于这次spfa可以找到负环，等价于新图有负环，等价于原图有负环。得证

*/

#if 0
#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>

using namespace std;

const int N = 1e4 + 10;

int n,m;
int d[N],cnt[N],h[N],e[N],ne[N],w[N],idx;
bool st[N];

void add(int a,int b,int c){
    e[idx] = b,ne[idx] = h[a],w[idx] = c,h[a] = idx++;
}

bool spfa(){
    // 不需要知道距离，只需要知道走的边有没有超过n
    // 1 把所有的节点入队
    queue<int> q;
    for(int i = 1;i <= n;i++){
        st[i] = true;
        q.push(i);
    } 
    // 2 出队遍历，状态置否
    while(q.size()){
        int t = q.front();
        q.pop();
        st[t] = false;
        // 3 周围节点能更新就更新，边数增加
        for(int i = h[t];i != -1;i = ne[i]){
            int j = e[i];
            if(d[j] > d[t] + w[i]){
                d[j] = d[t] + w[i];
                cnt[j] = cnt[t] + 1;
                // 4 走的边多了，存在负环
                if(cnt[j] >= n) return true;
                if(!st[j]){
                    q.push(j);
                    st[j] = true;
                }
            }
        }
    }
    return false;
}

int main()
{
    cin >> n >> m;
    memset(h,-1,sizeof h);
    while(m--){
        int a,b,c;
        cin >> a >> b >> c;
        add(a,b,c);
    }
    if(spfa()) puts("Yes");
    else puts("No");
}

#endif