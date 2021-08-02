#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;


// 分析一下，一样的值，无收益，不要管他。a更大，正收益，b更大，负收益
// 要让这个p最大，所以要均分。这里不要仅仅考了不等式的小于，要小于等于之后去上取整

const int N = 110;

int a[N],b[N];
int n;

int main()
{
    cin >> n;
    for(int i = 0;i < n;i++) cin >> a[i];
    for(int i = 0;i < n;i++) cin >> b[i];
    int x = 0,y = 0;
    for(int i = 0;i < n;i++){
        if(a[i] > b[i]) x++;
        if(a[i] < b[i]) y++;
    }
    if(!x) cout << -1 << endl;
    else cout << (x + y) / x << endl;
}