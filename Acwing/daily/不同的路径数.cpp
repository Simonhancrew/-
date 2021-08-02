#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_set>

//贝壳找房，bfs + unordered_set去重

using namespace std;

const int N = 10;

int n,m,k;
int matrix[N][N];
int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
unordered_set<int> st;

void dfs(int x,int y,int t,int cur){
    if(t == k) {
        st.insert(cur);
        return;
    }
    for(int i = 0;i < 4;i++){
        int nx = x + dx[i],ny = y + dy[i];
        if(nx < n && nx >= 0 && ny >= 0 && ny < m) {
            dfs(nx,ny,t + 1,cur * 10 + matrix[nx][ny]);
        }
    }
}

int main(){
    cin >> n >> m >> k;
    for(int i = 0;i < n;i++){
        for(int j = 0;j < m;j++){
            cin >> matrix[i][j];
        }
    }    
    for(int i = 0;i < n;i++){
        for(int j = 0;j < m;j++){
            dfs(i,j,0,matrix[i][j]);
        }
    }
    cout << st.size() << endl;
    return 0;
}