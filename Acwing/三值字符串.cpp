#include <iostream>
#include <cstring>
#include <algorithm>

//双指针算法，找到单调性。枚举端点，用数组或者哈希记录需要的信息。

using namespace std;

int cnt[3];
int res;

int work(const string &s){
    memset(cnt,0,sizeof cnt);
    int n = s.size();
    res = n + 1;
    for(int i = 0,j = 0;i < n;i++){
        cnt[s[i] - '1']++;
        while(cnt[s[j] - '1'] > 1) cnt[s[j++] - '1']--;
        if(cnt[0] && cnt[1] && cnt[2]){
            res = min(res,i - j + 1);
        }
    }
    return res==n + 1?0:res;
}

int main()
{
    int t;
    cin >> t;
    while(t--){
        string s;
        cin >> s;
        cout << work(s) << endl;
    }
    return 0;
}