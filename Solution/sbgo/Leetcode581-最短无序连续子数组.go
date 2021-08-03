package sbgo

import "sort"

func findUnsortedSubarray(nums []int) int {
	n := len(nums)
	cp := make([]int, n)
	copy(cp, nums)
	sort.Ints(nums)
	l, r := -1, 0
	for i := 0; i < n; i++ {
		if l == -1 && nums[i] != cp[i] {
			l = i
		} else if l != -1 && nums[i] != cp[i] {
			r = i
		}
	}
	if l == -1 {
		return 0
	}
	return r - l + 1
}
