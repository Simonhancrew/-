#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>

using namespace std;

int n;

void update(char& m){
    if(m == 'W') m = 'B';
    else m = 'W';
}

bool check(string s,char c){
    vector<int> res;
    for(int i = 0;i + 1 < n;i++){
        if(s[i] != c){
            update(s[i]);
            update(s[i + 1]);
            res.push_back(i);
        }
    }
    if(s.back() != s[0]) return false;

    cout << res.size() << endl;
    if(res.size() > 0) {
        for(auto val : res) cout << val + 1 << ' ';
        cout << endl;
    }
    return true;
}


int main()
{
    int t;
    cin >> t;
    while(t--){
        string s;
        cin >> n >> s;
        if(!check(s,'W') && !check(s,'B')) puts("-1");
    }
}