#include <iostream>
#include <string>

// 其实是一个经典的双指针，我这里偷了。

using namespace std;

int n;

int main()
{
    string s;
    cin >> n;
    cin >> s;
    int cnt = 0,res = 0;
    for(auto& ch:s){
        if(ch == 'x'){
            cnt++;
            if(cnt == 3){
                res ++;
                cnt--;
            }
        }
        else{
            cnt = 0;
        }
    }    
    cout << res << endl;
}