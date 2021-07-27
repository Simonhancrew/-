package sbgo

func findSecondMinimumValue(root *TreeNode) int {
	ans, bg := -1, root.Val
	var dfs func(*TreeNode)
	dfs = func(root *TreeNode) {
		if root == nil {
			return
		}
		if root.Val != bg {
			if ans == -1 {
				ans = root.Val
			} else {
				ans = min(ans, root.Val)
			}
			return
		}
		dfs(root.Left)
		dfs(root.Right)
	}
	dfs(root)
	return ans
}
