#include <iostream>
#include <string>

//实际没有必要把双端队列实现出来，可以考虑哈希表双映射，或者这种卡数据的中间作为起点
//再或者因为数据都不一样，可以就记录坐标

using namespace std;

const int N = 4e5 + 10;
string s;
char op[2];
int q;

int hh = 2e5,tt = 2e5 - 1;

int dq[N],p[N];

int main()
{
    cin >> q;
    for(int i = 0;i < q;i++){
        int x;
        scanf("%s%d",op,&x);
        if(op[0] == 'L'){
            dq[--hh] = x;
            p[x] = hh;
        }else if(op[0] == 'R'){
            dq[++tt] = x;
            p[x] = tt;
        }else{
            cout << min(tt - p[x],p[x] - hh) << endl;
        }
    }
}

/*
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 200010;

int n;
int p[N];

int main()
{
    cin >> n;
    int l = 0, r = -1;
    while (n -- )
    {
        char c;
        int x;
        cin >> c >> x;
        if (c == 'L') p[x] = -- l;
        else if (c == 'R') p[x] = ++ r;
        else cout << min(r - p[x], p[x] - l) << endl;
    }

    return 0;
}
*/