#include "ADT.hpp"


class Solution {
public:
    int ans = -1;
    int findSecondMinimumValue(TreeNode* root) {
        dfs(root,root->val);
        return ans;
    }
    void dfs(TreeNode* node,int val){
        if(!node) return;
        if(node->val != val){
            if(ans == -1) ans = node->val;
            else ans = min(ans,node->val);
            return; 
        }
        dfs(node->left,val);
        dfs(node->right,val);
    }
};