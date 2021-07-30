class Solution:
    def titleToNumber(self, columnTitle: str) -> int:
        n = len(columnTitle)
        ans = 0
        cnt = 1
        for i in range(n):
            ans = ans *  26 + (ord(columnTitle[i]) - ord('A') + 1)
        return ans