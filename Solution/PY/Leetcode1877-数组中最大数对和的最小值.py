from typing import List

class Solution:
    def minPairSum(self, nums: List[int]) -> int:
        nums.sort()
        l,r = 0,len(nums) - 1
        mm = 0
        while l < r:
            mm = max(nums[l] + nums[r],mm)
            l += 1
            r -= 1
        return mm