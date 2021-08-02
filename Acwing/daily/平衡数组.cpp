#include <iostream>
#include <cstring>
#include <algorithm>

// 样例比较有迷惑性，其实选中某个数，其余的数 +i 等价于选中的数-i

using namespace std;

int n;

int main()
{
    int t;
    cin >> t;
    while(t--){
        cin >> n;
        cout << n << endl;
        for(int i = 1;i <= n;i++) cout << i << ' ';
        cout << endl;
    }
}