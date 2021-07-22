
class Node:
    def __init__(self, x: int, next: 'Node' = None, random: 'Node' = None):
        self.val = int(x)
        self.next = next
        self.random = random

class Solution:
    def copyRandomList(self, head: 'Node') -> 'Node':
        p = head
        while p:
            q = Node(p.val)
            q.next = p.next
            p.next = q
            p = p.next.next
        
        p = head
        while p:
            if p.random:
                p.next.random = p.random.next
            p = p.next.next
        
        dummy = Node(-1)
        cur = dummy
        p = head
        while p:
            q = p.next
            cur.next = q
            cur = cur.next
            p.next = q.next
            p = p.next
        return dummy.next