from typing import List
from collections import defaultdict
class Solution:
    def minOperations(self, target: List[int], arr: List[int]) -> int:
        mp = defaultdict(int)
        n = len(target)
        for i in range(n):
            mp[target[i]] = i
        t = []
        rhs = 0
        for val in arr:
            if val in mp:
                idx = mp[val]
                if not t or t[-1] < idx:
                    t.append(idx)
                else:
                    l,r = 0,len(t) - 1
                    # 二分下界
                    while l < r:
                        mid = (l + r) // 2
                        if t[mid] >= idx:
                            r = mid
                        else:
                            l = mid + 1
                    t[l] = idx
        return n - len(t)