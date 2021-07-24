#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
using ll = long long;

const int N = 2e5 + 10;

int pos[N];//前i位置有多少个正数
int nag[N];
int n;

//mul(l,r) = mul(0,r) / mul(0,l - 1);枚举每一个右端点，看前面有多少和当前符号是相同的就有多少个正数对
//因为只用到了i-1一个状态，所以可以不用开数组。

int main()
{
    cin >> n;
    pos[0] = 1;
    int s = 1;
    ll nagnums = 0,posnums = 0;//strore the ans
    for(int i = 1;i <= n;i++){
        int x;
        scanf("%d",&x);
        if(x < 0) s *= -1;
        if(s > 0) {//(0,i)位置的mul > 0，正数对找前面i-1是正数的个数的就可以
            posnums += pos[i - 1],nagnums += nag[i - 1];
            pos[i] = pos[i - 1] + 1,nag[i] = nag[i - 1];
        }else{//当前i位置的mul < 0
            nagnums += pos[i - 1],posnums += nag[i - 1];
            pos[i] = pos[i - 1],nag[i] = nag[i - 1] + 1;
        }
    }
    cout << nagnums << ' ' << posnums << endl;
}