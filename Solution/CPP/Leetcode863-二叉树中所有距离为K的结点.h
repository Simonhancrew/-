#include "ADT.hpp"
#include <unordered_map>


using namespace std;


// 建图 + dfs，主要不往回走。

class Solution {
public:
    vector<int> res;
    unordered_map<TreeNode*,vector<TreeNode*>> mp;//无向图->有向图
    void buildGrph(TreeNode* node){
        if(node->left){
            mp[node].push_back(node->left);
            mp[node->left].push_back(node);
            buildGrph(node->left);
        }
        if(node->right){
            mp[node].push_back(node->right);
            mp[node->right].push_back(node);
            buildGrph(node->right);
        }
    }
    // 不往回走
    void dfs(TreeNode* root,TreeNode* fa,int k){
        if(!k) res.push_back(root->val);
        for(auto node:mp[root]){
            if(node != fa){
                dfs(node,root,k - 1);
            }
        }
    }
    vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
        buildGrph(root);
        dfs(target,NULL,k);
        return res;
    }
};