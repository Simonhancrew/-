#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

// 找规律，要最大最小，尽量均匀分配。但是1的时候还是无法均分的，所以在第一次无法均分的时候
// 记录tag，之后每一次都把更多的数分给之前吃亏的数（无论后面怎么分，前面的那个不均匀分配的数因为在高位，无论如何都会更大的）

int n;
string x;

void solve(){
    string a,b;
    bool flag = false;
    
    for(int i = 0;i < n;i++){
        if(x[i] == '2'){
            if(flag){
                a += '0';
                b += '2';
            }else{
                a += '1';
                b += '1';
            }
        }else if (x[i] == '1'){
            if(flag){
                a += '0';
                b += '1';
            }else{
                a += '1';
                b += '0';
                flag = true;
            }
        }else{
            a += '0';
            b += '0';
        }
    }
    cout << a << endl;
    cout << b << endl;
}

int main()
{
    int t;
    cin >> t;
    while(t--){
        cin >> n;
        cin >> x;
        solve();
    }
    return 0;
}