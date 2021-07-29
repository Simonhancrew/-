#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>

//bellmanford算法的一个优化，和dijsktra很像
//优化点在于不一定每次迭代的时候每条边都可以用作更新
//宽搜做一个优化，队列里储存所有更新过的点
//一个点只有被更新的更小了才能去更新他的后继

using namespace std;

const int N = 1e5 + 10;

//邻接表
int h[N],e[N],ne[N],w[N],idx;
int n,m;
int d[N];
//某个点是否存在于队列中
bool st[N];

void add(int a,int b,int c){
    e[idx] = b,ne[idx] = h[a],w[idx] = c,h[a] = idx++;
}

int spfa(){
    queue<int> q;
    q.push(1),st[1] = true;
    memset(d,0x3f,sizeof d);
    d[1] = 0;
    //对于在队列中的每一个点，拿出来看看能不能更新他连接的点
    //如果可以的话就把更新的点放进队列
    while(q.size()){
        int t = q.front();
        q.pop();
        st[t] = false;
        for(int i = h[t];i != -1;i = ne[i]){
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
    if(d[n] == 0x3f3f3f3f) return -1;
    else return d[n];
    
}



int main(){
    cin >> n >> m;
    
    memset(h,-1,sizeof h);
    
    while(m--){
        int a,b,c;
        cin >> a >> b >> c;
        add(a,b,c);
    }
    
    int t = spfa();
    
    if(t == -1) puts("impossible");
    else cout << t << endl;
    
    return 0;
}


#if 0
#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>

using namespace std;

const int N = 1e5 + 10;

int n,m;
int d[N],h[N],e[N],ne[N],w[N],idx;
bool st[N];

void add(int a,int b,int c){
    e[idx] = b,ne[idx] = h[a],w[idx] = c,h[a] = idx++;
}


int spfa(){
    // 1 所有距离置0x3f,起点置零
    memset(d,0x3f,sizeof d);
    d[1] = 0;
    // 2 将更新了的最短距离放进队列中,重复的放没有错，但是效率会下降
    queue<int> q;
    q.push(1);
    st[1] = true; // 这里为了防止节点重复放入
    // 3 利用队列中的节点更新周围节点
    while(q.size()){
        int t = q.front();
        q.pop();
        st[t] = false;
        // 周围节点
        for(int i = h[t];i != -1;i = ne[i]){
            int j = e[i];
            // 确定距离是否变小了
            if(d[j] > d[t] + w[i]){
                d[j] = d[t] + w[i];
                // 确定在不在集合里，在集合里的都是变小了的，没必要重复加入
                if(!st[j]) q.push(j),st[j] = true;
            }
        }
    }
    if(d[n] == 0x3f3f3f3f) return -1;
    return d[n];
}

int main()
{
    cin >> n >> m;
    memset(h,-1,sizeof h);
    while(m--){
        int x,y,z;
        cin >> x >> y >> z ;
        add(x,y,z);
    }

    int t = spfa();
    if(t == -1) puts("impossible");
    else cout << t << endl;
    return 0;
}
#endif