#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<int> kWeakestRows(vector<vector<int>>& mat, int k) {
        // 二分 + 排序
        unordered_map<int,int> mp;
        int n  = mat.size();
        for(int i = 0;i < n;i++){
            int l = 0,r = mat[i].size() - 1;
            while(l < r) {
                int mid = l + r >>  1;
                if(mat[i][mid] == 0) r = mid;
                else l = mid + 1; 
            }
            mp[i] = l;
            if(mat[i][l] == 1) mp[i]++;
        }
        vector<pair<int,int>> arr;
        for(auto pii:mp){
            arr.push_back(pii);
        }
        sort(arr.begin(),arr.end(),[](auto& lhs,auto& rhs){
            return lhs.second < rhs.second || (lhs.second == rhs.second && lhs.first < rhs.first);
        });
        vector<int> res;
        for(int i = 0;i < k;i++){
            res.push_back(arr[i].first);
        }
        return res;
    }
};