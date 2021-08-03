#ifndef __DATA_TYPE
#define __DATA_TYPE
#include <stdlib.h>

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};
#endif