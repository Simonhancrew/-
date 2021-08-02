#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

//最近难的的送福利的题目

const int N = 110;
const double pi = 3.14159265358979;

int a[N],n;


int main()
{
    cin >> n;
    for(int i = 0;i < n;i++){
        cin >> a[i];
    }
    sort(a,a + n);
    double res = 0;
    for(int i = n - 1;i >= 1;i -= 2){
        res += pi * (a[i] - a[i - 1]) * (a[i] + a[i - 1]);
    }
    if(n & 1){
        res += pi * a[0] * a[0];
    }
    printf("%.6f\n",res);
    return 0;
}
