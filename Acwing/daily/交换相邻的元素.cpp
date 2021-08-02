#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 2e5 + 10;

int n;
int a[N];
string s;

bool check(){
    int l = 0,r = 0;
    while(r < n - 1){
        while(s[r] == '1') r++;
        // cout << l << ' ' << r << endl;
        if(l != r) sort(a + l,a + r + 1);
        // for(int i = l;i <= r + 1;i ++) cout << a[i] << ' ';
        // cout << endl;
        l = ++r;
    }
    
    for(int i = 0;i < n;i++){
        if(a[i] != i + 1) return false;
    }
    return true;
}

int main()
{
    cin >> n;
    for(int i = 0;i < n;i++) cin >> a[i];
    cin >> s;
    if(check()) puts("YES");
    else puts("NO");
}   


// 看看一个区间里面的值是不是应有的
#if 0
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 2e5 + 10;

int a[N],n;
char s[N];


int main()
{
    cin >> n;
    for(int i = 1;i <= n;i++) cin >> a[i];
    scanf("%s",s + 1);
    for(int i = 1,mm = 0;i <= n;i++){
        mm = max(mm,a[i]);
        if(s[i] == '0' && i != mm){
            puts("NO");
            return 0;
        }
    }
    puts("YES");
}
#endif