#include "data.h"
#if 1
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* pre = NULL,*cur = head;
        while(cur){
            auto t = cur->next;
            cur->next = pre;
            pre = cur;
            cur = t;
        }
        return pre;
    }
};
#endif


#if 0 
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if(!head || !head->next) return head;
        auto p = head ->next;
        head ->next = NULL;
        auto newhead = reverseList(p);
        auto end = newhead;
        while(end->next) end = end->next;
        end ->next = head;
        return newhead;
    }
};
#endif

#if 0 
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if(!head || !head->next) return head;
        auto p = head ->next;
        head ->next = NULL;
        auto newhead = reverseList(p);
        auto end = newhead;
        while(end->next) end = end->next;
        end ->next = head;
        return newhead;
    }
};
#endif