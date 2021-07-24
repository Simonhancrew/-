#include <iostream>
#include <cstring>
#include <algorithm>
#include <map>
#include <unordered_map>

// map排序
// up记录位置

using namespace std;


int main()
{
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        map<int,int> cnt;
        unordered_map<int,int> shw;
        for(int i = 1;i <= n;i++) {
            int x;
            cin >> x;
            cnt[x]++;
            shw[x] = i;
        }
        bool flag = false;
        for(auto& [k,v]:cnt){
            // cout << v << endl;
            if(v == 1){
                flag = true;
                cout << shw[k] << endl;
                break;
            }
        }
        if(!flag) cout << -1 << endl;
    }
    return 0;
}