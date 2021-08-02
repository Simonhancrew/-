#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long LL;
vector<LL> num;


// dfs出现过的数
void dfs(int u,LL x){ 
    num.push_back(x);
    if(u == 10) return;
    dfs(u + 1,x * 10 + 4);
    dfs(u + 1,x * 10 + 7);
}


int main()
{
    LL l,r;
    cin >> l >> r;
    dfs(0,0);
    sort(num.begin(),num.end());
    LL res = 0;
    for(int i = 1;i < num.size();i++){
        LL a = num[i - 1] + 1,b = num[i];
        res += max(0ll,min(b,r) - max(a,l) + 1) * num[i]; // 一个求交集的实现
    }
    cout << res << endl;
}