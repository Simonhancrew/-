#include <iostream>
#include <cstring>
#include <algorithm>


//lcs和lis的互相转化

using namespace std;

/*
    1 第一个数组中的数字都不重复，所以满足lcs转化为lis的问题
    2 第一个数组是a，第二个数组是b。构建一个数组c，该数组的每个元素是b中出现的元素，且在a中出现，c[b[i]] = k,k是这个b[i]出现在a中的下标。
    求数组c的lis，此时a中出现的数的下标递增，等价于a中的数是从左到右排列的
*/

const int N = 1e6 + 10;

int id[N],h[N];

int main(){
    int n;
    cin >> n;
    memset(id,-1,sizeof id);
    for(int i = 0;i < n;i++){
        int x;
        scanf("%d", &x);
        id[x] = i;
    }
    int len = 0;
    for(int i = 0;i < n;i++){
        int x;
        scanf("%d", &x);
        int k = id[x];
        if(k == -1) continue;
        int l = 0,r = len;
        while(l < r){
            int mid = l + r  + 1>> 1;
            if(h[mid] < k) l = mid;
            else r = mid - 1;
        }
        h[r + 1] = k;
        len = max(r + 1,len);
    }
    cout << len << endl;
}