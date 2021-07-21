#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

// 仔细分析，发现这是一个环图的问题，每个点的入度和出度最终都会是1
// 直接建图就可以了，对于可能存在环，开一个state数组，避免死环
// 遍历每一个点，找到他的尾节点和头节点，如果没有连接起来，需要加入剩下的点
// 如果所有的环是连接的话，剩下的点就需要单独开一个环了。

const int N = 2e5 + 10;

int ne[N],pr[N],st[N];

int main(){
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        memset(st,0,sizeof st);
        memset(pr,0,sizeof pr);
        memset(ne,0,sizeof ne);
        for(int i = 1;i <= n;i++){
            cin >> ne[i];
            pr[ne[i]] = i;
        }
        
        bool flag = false;
        for(int i = 1;i <= n;i++){
            if(st[i] || !ne[i]) continue;
            st[i] = 1;
            // 找到首尾
            int x = i,y = i;
            while(ne[x] && !st[ne[x]]){
                x = ne[x];
                st[x] = 1;
            }
            while(pr[y] && !st[pr[y]]){
                y = pr[y];
                st[y] = 1;
            }
            if(ne[x] == y) continue;
            // 完全不在环里面的点
            if(!flag){
                flag = true;
                for(int j = 1;j <= n;j++){
                    if(!ne[j] && !pr[j]){
                        ne[x] = j;
                        x = j;
                        st[j] = 1;
                    }
                }
            }
            ne[x] = y;
        }
        if(!flag){
            int x = 0,y = 0;
            for(int i = 1;i <= n;i++){
                if(!ne[i]){
                    if(!x && !y) x = y = i;
                    else{
                        ne[x] = i;
                        x = i;
                    }
                }
            }
            ne[x] = y;
        }
        for(int i = 1;i <= n;i++) cout << ne[i] << ' ';
        puts("");
    }
}