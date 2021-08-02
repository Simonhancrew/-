#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

int n,t,k;

// b只有两个，直接枚举b的位置就可以了。注意枚举的手法

int main()
{
    cin >> t;
    while(t--){
        cin >> n >> k;
        for(int i = 1;;i++){
            if(k <= i){
                string res(n,'a');
                res[n - i - 1] = 'b';
                res[n - k] = 'b'; 
                cout << res << endl;
                break;
            }
            k -= i;
        }
    }
    return 0;
}