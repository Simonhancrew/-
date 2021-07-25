from typing import List
from collections import defaultdict
class Solution:
    def restoreArray(self, adjacentPairs: List[List[int]]) -> List[int]:
        mp = defaultdict(list)
        for nei in adjacentPairs:
            mp[nei[0]].append(nei[1])
            mp[nei[1]].append(nei[0])
        
        n = len(adjacentPairs) + 1
        ret = [0] * n
        for k,v in mp.items():
            if len(v) == 1:
                ret[0] = k
                break

        ret[1] = mp[ret[0]][0]

        for i in range(2,n):
            nei = mp[ret[i - 1]]
            if ret[i - 2] == nei[0]:
                ret[i] = nei[1]
            else:
                ret[i] = nei[0]

        return ret