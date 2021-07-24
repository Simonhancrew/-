#include <iostream>
#include <cstring>
#include <algorithm>
#include <unordered_map>

// 周赛题，类似于lc的两数之和

using namespace std;

const int N = 2e5 + 10;

typedef pair<int, int> PII;

#define x first
#define y second

int a[N];

int main()
{
    int k;
    cin >> k;
    unordered_map<int,PII> mp;
    for(int i = 1;i <= k;i++){
        int sum = 0,n;
        cin >> n;
        for(int j = 1;j <= n;j++){
            cin >> a[j];
            sum += a[j];
        }
        
        for(int j = 1;j <= n;j++){
            int t = sum - a[j];
            if(mp.count(t) && mp[t].x != i){
                puts("YES");
                cout << mp[t].x << ' ' << mp[t].y << endl;
                cout << i << ' ' << j << endl;
                return 0;
            }
            mp[t] = {i,j};
        }
    }
    puts("NO");
    return 0;
}