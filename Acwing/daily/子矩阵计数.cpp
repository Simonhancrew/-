#include <iostream>
#include <unordered_map>

using namespace std;
using ll = long long;

int n,m,k;
int a;

/*
统计一个每个数组连续1的数量，然后看看能不能被k整除，这个整除之后的长度还要在另一个数组的范围内。
然后单纯的乘出结果就行了
*/

int main()
{
    cin >> n >> m >> k;  
    int cnt = 0;
    unordered_map<int,int> mpa,mpb;
    for(int i = 0;i < n;i++){
        scanf("%d", &a);
        if(a){
            cnt ++;
        } 
        else{
            for(int j = 1;j <= cnt;j++) mpa[j] += (cnt - j + 1);
            cnt = 0;
        }
    } 
    for(int i = 1;i <= cnt;i++) mpa[i] += (cnt - i + 1);
    
    cnt = 0;
    for(int i = 0;i < m;i++) {
        scanf("%d",&a);
        if(a){
            cnt++;
        }else{
            for(int j = 1;j <= cnt;j++) mpb[j] += (cnt - j + 1);
            cnt = 0;
        }
    }
    for(int i = 1;i <= cnt ;i++) mpb[i] += (cnt - i + 1);
    
    
    ll ans = 0;
    for(auto &pii:mpa){
        int l = pii.first,num = pii.second;
        if(k % l == 0 && k / l <= m){
            ans += num * mpb[k / l]; 
        }
    }
    printf("%lld\n",ans);
    return 0;
}