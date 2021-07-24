#include <iostream>
#include <cstring>
#include <algorithm>

// 大模拟，和前面的数不同，最后一个数还要和第一个数不同

using namespace std;

const int N = 110;
int a[N],b[N],c[N];
int n;

void solve(){
    int last = -1;
    for(int i = 1;i < n;i++){
        if(last != a[i]) {
            cout << a[i] << ' ';
            last = a[i];
        }
        else {
            cout << b[i] << ' ';
            last = b[i];
        }
    }
    if(a[n] != last && a[n] != a[1]) cout << a[n] << endl;
    else if(b[n] != last && b[n] != a[1]) cout << b[n] << endl;
    else cout << c[n] << endl;
}

int main()
{
    int t;
    cin >> t;
    while(t --){;
        cin >> n;
        for(int i = 1;i <= n;i++) cin >> a[i];
        for(int i = 1;i <= n;i++) cin >> b[i];
        for(int i = 1;i <= n;i++) cin >> c[i];
        solve();
    }
    return 0;
}