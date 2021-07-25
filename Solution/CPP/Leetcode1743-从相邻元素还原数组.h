#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
public:
    typedef vector<int> VI;

    vector<int> restoreArray(vector<vector<int>>& adjacentPairs) {
        unordered_map<int,VI> mp;
        for(auto nei:adjacentPairs){
            mp[nei[0]].push_back(nei[1]);
            mp[nei[1]].push_back(nei[0]);
        }
        int n = adjacentPairs.size() + 1;
        vector<int> res(n);
        // 头节点只有一个相邻
        for(auto [k,v]:mp){
            if(v.size() == 1){
                res[0] = k;
                break;
            }
        }
        res[1] = mp[res[0]][0];
        for(int i = 2;i < n;i++){
            auto nei = mp[res[i - 1]];
            res[i] = res[i - 2] == nei[0]?nei[1]:nei[0];
        }
        return res;
    }
};