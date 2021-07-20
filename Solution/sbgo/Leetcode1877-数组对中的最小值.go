package sbgo

import "sort"

func minPairSum(nums []int) int {
	mm, l, r := 0, 0, len(nums)-1
	sort.Ints(nums)
	for l < r {
		mm = max(mm, nums[l]+nums[r])
		l++
		r--
	}
	return mm
}
