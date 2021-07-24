#include <iostream>

//很有趣的一道题，打印一个菱形。模拟的时候逐行打印，分析空格和实际情况.

using namespace std;

int n;

void print(int mid){
    int sp = (n - mid) * 2;
    for(int i = 0;i < sp;i++) cout << ' ';
    for(int i = 0;i <= mid;i++) cout << i << ' ';
    for(int i = mid - 1;i >= 0;i--){
        cout << i;
        if(i != 0) cout << ' ';
    }
    cout << endl;
}

int main()
{
    cin >> n;
    //逐行打印
    for(int i = 0;i <= n;i++) print(i);
    for(int i = n - 1;i >= 0;i--) print(i);
    return 0;
}