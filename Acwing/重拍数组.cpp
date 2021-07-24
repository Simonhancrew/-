#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

const int N = 110;
int a[N];

//逆序排列数组就可以了

int main()
{
    int t;
    cin >> t;
    while(t--){
        int n;
        cin >> n;
        for(int i = 0;i < n;i++){
            cin >> a[i];
        }
        sort(a,a + n,greater<int>());//sort(a,a + n,[](int lhs,int rhs){return lhs > rhs;});
        for(int i = 0;i < n;i++) cout << a[i] << ' ';
        cout << endl;
    }
    return 0;
}