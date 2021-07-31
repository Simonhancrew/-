#include <vector>
#include <algorithm>
#include <map>
#include "ADT.hpp"

using namespace std;

class Solution {
public:
    map<int,vector<vector<int>>> mp;
    void dfs(TreeNode* node,int x,int y){
        if(!node) return;
        mp[y].push_back({x,node->val});
        dfs(node->left,x + 1,y - 1);
        dfs(node->right,x + 1,y + 1);
    }
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        dfs(root,0,0);
        vector<vector<int>> res; 
        for(auto& [k,v]:mp){
            sort(v.begin(),v.end());
            vector<int> tmp;
            for(auto n:v) tmp.push_back(n[1]);
            res.push_back(tmp);
        }
        return res;
    }
};