#include <iostream>
#include <cstring>
#include <algorithm>


// 高精度加法

using namespace std;

int t;
string s;

string aplus(int x){
    string res;
    int t = x;
    for(int i = s.size() - 1;i >= 0;i --)
    {
        t += s[i] - '0';
        res.push_back((t % 2) + '0');
        t /= 2;
    }
    if(t) res.push_back((t % 2) + '0');
    reverse(res.begin(), res.end());
    return res;
}

int main()
{
    cin >> t;
    for(int i = 0;i < t;i++){
        cin >> s;
        cout << aplus(1) << endl;
        cout << aplus(3) << endl;
    }
    return 0;
}