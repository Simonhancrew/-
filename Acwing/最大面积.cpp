#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

// 字节 presum + 单调栈 + 预处理

const int N = 2010;

int n,m;
char matrix[N][N];
int l[N],r[N],q[N];
int U[N], D[N], L[N], R[N];
int s[N][N];//记录上方或者下方有多少个连续的1

int query(int x,int y){
    int res = 0;
    res = max(max(U[x - 1], D[x + 1]), max(L[y - 1], R[y + 1]));
    return res;
}

int calc(int h[],int u){
    h[0] = h[u + 1] = -1;
    int tt = 0;
    q[0] = 0;
    for(int i = 1;i <= u;i++){
        while(h[q[tt]] >= h[i]) tt--;
        l[i] = q[tt];
        q[++tt] = i;
    }
    
    tt = 0;
    q[0] = u + 1;
    //下标从1开始的
    for(int i = u;i >= 1;i--){
        while(h[q[tt]] >= h[i]) tt--;
        r[i] = q[tt];
        q[++tt] = i;
    }
    int res = 0;
    for(int i = 1;i <= u;i++){
        res = max(res,h[i] * (r[i] - l[i] - 1));
    }
    return res;
}


void init()
{
    for (int i = 1; i <= n; i ++ )
    {
        for (int j = 1; j <= m; j ++ )
            if (matrix[i][j] == '1') s[i][j] = s[i - 1][j] + 1;
            else s[i][j] = 0;
        U[i] = max(U[i - 1], calc(s[i], m));
    }

    memset(s, 0, sizeof s);
    for (int i = n; i; i -- )
    {
        for (int j = 1; j <= m; j ++ )
            if (matrix[i][j] == '1') s[i][j] = s[i + 1][j] + 1;
            else s[i][j] = 0;
        D[i] = max(D[i + 1], calc(s[i], m));
    }

    memset(s, 0, sizeof s);
    for (int i = 1; i <= m; i ++ )
    {
        for (int j = 1; j <= n; j ++ )
            if (matrix[j][i] == '1') s[i][j] = s[i - 1][j] + 1;
            else s[i][j] = 0;
        L[i] = max(L[i - 1], calc(s[i], n));
    }

    memset(s, 0, sizeof s);
    for (int i = m; i >= 0; i -- )
    {
        for (int j = 1; j <= n; j ++ )
            if (matrix[j][i] == '1') s[i][j] = s[i + 1][j] + 1;
            else s[i][j] = 0;
        R[i] = max(R[i + 1], calc(s[i], n));
    }
}



int main(){
    cin >> n >> m;
    for(int i = 1;i <= n;i++) scanf("%s",matrix[i] + 1);
    
    init();
    int Q;
    cin >> Q;
    for(int i = 0;i < Q;i++){
        int x,y;
        cin >> x >> y;
        x++,y++;//从1开始
        int res = query(x,y);
        cout << res << endl;
    }
    return 0;
}