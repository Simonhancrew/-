from typing import List
class Solution:
    def pathInZigZagTree(self, label: int) -> List[int]:
        row = 1
        def getrv(row,label):
            return 2 ** (row) + 2 ** (row - 1) - 1 - label
        while 2 ** row <= label:
            row += 1
        if row % 2 == 0:
            label = getrv(row,label)
        res = []
        while row > 0:
            if row % 2 == 0:
                res.append(getrv(row,label))
            else:
                res.append(label)
            row -= 1
            label >>= 1
        res.reverse()
        return res