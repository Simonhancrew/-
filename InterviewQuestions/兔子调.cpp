// 智力题，因为二维可以随便走，只要欧式距离满足，所以可以分情况讨论

#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

int main()
{
    int t;
    cin >> t;
    while(t--){
        int n,x;
        cin >> n >> x;
        int mm = 0;
        bool flag = false;
        while(n--){
            int tmp;
            cin >> tmp;
            if(tmp == x) flag = true;// dont break here,cause all the data need to be read
            mm = max(mm,tmp);
        }
        if(flag) cout << 1 << endl;
        else if(mm > x) cout << 2 << endl;
        else cout << (mm + x - 1) / mm << endl;
    }
}