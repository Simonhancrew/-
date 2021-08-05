from typing import List

class Solution:
    def eventualSafeNodes(self, graph: List[List[int]]) -> List[int]:
        n = len(graph)
        rev = [[] for _ in range(n)]
        d = [0] * n
        for i in range(n):
            for b in graph[i]:
                a = i
                rev[b].append(a)
                d[a] += 1
        que = []
        for i in range(n):
            if d[i] == 0:
                que.append(i)

        while len(que) > 0:
            t = que[0]
            que.pop(0)
            for b in rev[t]:
                d[b] -= 1
                if d[b] == 0:
                    que.append(b)
        res = []
        for i in range(n):
            if d[i] == 0:
                res.append(i)
        return res
            