#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;
using ll = long long;

const int N = 1e5 + 10;

int a[N];

int main()
{
    int t;
    cin >> t;
    while(t --){
        int n,x;
        cin >> n >> x;
        // cnt代表能有多少完全展开次数，
        // psum是最后一次停止的时候前面的展开和
        ll sum = 0,pusm = 0;
        int cnt = N;
        for(int i = 0;i < n;i++){
            cin >> a[i];
        }
        for(int i = 0;i < n;i++){
            sum += a[i];
            int c = 0;
            for(int j = a[i];j % x == 0; j /= x){
                c++;
            }
            if(c < cnt){
                cnt = c;
                pusm = sum - a[i];
            }
        }
        cout << sum * (cnt + 1) + pusm << endl;
    }
}