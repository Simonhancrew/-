#include "data.h"
// 快慢指针
class Solution {
public:
    ListNode *entryNodeOfLoop(ListNode *head) {
        if(!head || !head -> next) return head;
        auto fast = head,slow = head;
        while(fast && slow){
            fast = fast->next;
            slow = slow->next;
            if(fast) fast = fast->next;
            else return 0;
            if(fast == slow){
                slow = head;
                while(slow != fast){
                    slow = slow ->next;
                    fast = fast->next;
                }
                return slow;
            }
        }
        return 0;
    }
};