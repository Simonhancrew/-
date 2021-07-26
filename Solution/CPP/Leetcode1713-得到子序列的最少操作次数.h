#include <algorithm>
#include <vector>
#include <unordered_map>

using namespace std;

// 最长公共子序列转下标不减的最长上升子序列

class Solution {
public:
    int minOperations(vector<int>& target, vector<int>& arr) {
        // target中下标唯一，则可以映射成下标数组
        // 在arr中出现的最长上升子序列
        unordered_map<int,int> pos;
        for(int i = 0;i < target.size();i++) pos[target[i]] = i;
        vector<int> t;
        for(auto val:arr){
            if(pos.count(val)){
                int idx = pos[val];
                auto it = lower_bound(t.begin(),t.end(),idx);
                if(it == t.end()) t.push_back(idx);
                else *it = idx;
            }
        } 
        return target.size() - t.size();
    }
};