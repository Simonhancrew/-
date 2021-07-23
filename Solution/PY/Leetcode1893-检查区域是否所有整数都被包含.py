from typing import List
class Solution:
    def isCovered(self, ranges: List[List[int]], left: int, right: int) -> bool:
        df = [0] * 52
        for l,r in ranges:
            df[l] += 1
            df[r + 1] -= 1

        cur = 0
        for i in range(1,51):
            cur += df[i]
            # print(cur)
            if left <= i <= right and cur <= 0:
                return False    
        return True 