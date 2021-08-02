#include <stdlib.h>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
public:
    ListNode* getTail(ListNode* head){
        while(head->next) head= head->next;
        return head;
    }
    ListNode* quickSortList(ListNode* head) {
        // 1 特判和递归结束
        if(!head || !head->next) return head;
        // 2 partition的部分
        int val = head->val;
        auto left = new ListNode(-1),mid = new ListNode(-1),right = new ListNode(-1);
        auto ltail = left,mtail = mid,rtail = right;
        for(auto p = head;p;p = p->next){
            if(p->val == val) mtail = mtail->next = p;
            else if(p -> val > val) rtail = rtail -> next = p;
            else ltail = ltail->next = p;
        }
        // 3 断开原来的连接
        mtail->next = ltail -> next = rtail->next = NULL;
        // 4 递归
        left->next = quickSortList(left->next),right->next = quickSortList(right -> next);
        // 5 重建链表
        getTail(left)->next = mid -> next;
        getTail(left)->next = right->next;
        
        auto p = left -> next;
        delete left;
        delete mid;
        delete right;
        return p;
    }
};