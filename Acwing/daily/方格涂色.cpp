#include <iostream>
#include <cstring>
using namespace std;

//二进制枚举四个角，中间的部分和其余的行列无关的

int n;
int a[4];
int st[4];


bool check(int state){
    int ul = state >> 0 & 1,ur = state >> 1 & 1;
    int dl = state >> 2 & 1,dr = state >> 3 & 1;
    if(ul + ur > a[0] || ul + ur + n - 2 < a[0]) return false;
    if(ur + dr > a[1] || ur + dr + n - 2 < a[1]) return false;
    if(dl + dr > a[2] || dl + dr + n - 2 < a[2]) return false;
    if(dl + ul > a[3] || dl + ul + n - 2 < a[3]) return false;
    return true;
}

int main()
{
    int t;
    cin >> t;
    while(t --){
        
        cin >> n;
        for(int i = 0;i < 4;i++) cin >> a[i];
        bool flag = false;
        for(int i = 0;i < 1 << 4;i++){
            if(check(i)){
                flag = true;
                break;
            }
        }
        if(flag) cout << "YES" << endl;
        else cout << "NO" << endl;
    }
}