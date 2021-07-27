class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

class Solution:
    def findSecondMinimumValue(self, root: TreeNode) -> int:
        ans = [-1]
        bg = root.val
        def dfs(root,ans):
            if not root:
                return 
            if root.val != bg:
                if -1 == ans[0]:
                    ans[0] = root.val
                else:
                    ans[0] = min(ans[0],root.val)
                return 
            dfs(root.left,ans)
            dfs(root.right,ans)
        dfs(root,ans)
        return ans[0]