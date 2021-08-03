class Solution:
    def findUnsortedSubarray(self, nums: List[int]) -> int:
        cp = nums[:]
        nums.sort()
        n = len(nums)
        l,r = -1,0
        for i in range(n):
            if l==-1 and nums[i] != cp[i]:
                l = i
            elif l != -1 and nums[i] != cp[i]:
                r = i

        if l == -1:
            return 0
        return r - l + 1