#include <vector>

using namespace std;

// 差分区间

class Solution {
public:
    bool isCovered(vector<vector<int>>& ranges, int left, int right) {
        int dif[52];
        memset(dif,0,sizeof dif);
        for(auto &rg:ranges){
            insert(dif,rg[0],rg[1],1);
        }
        int cur = 0;
        for(int i = 1;i <= 50;i++){
            cur += dif[i];
            if(i <= right && i >= left && cur <= 0){
                return false;
            }
        }
        return true;
    }
    void insert(int a[],int l,int r,int val){
        a[l] += val;
        a[r+1] -= val; 
    }
};