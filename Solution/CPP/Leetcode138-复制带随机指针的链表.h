/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* next;
    Node* random;
    
    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};
*/
//有random的链表深拷贝
#include <unordered_map>
#include <map>

using namespace std;

class Node {
public:
    int val;
    Node* next;
    Node* random;
    
    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};
class Solution {
public:
    Node* copyRandomList(Node* head) {
        std::map<Node*,int> node_map;
        std::vector<Node*> node_vec;
        Node* ptr = head;
        int i = 0;
        while(ptr){
            node_vec.push_back(new Node(ptr->val));
            node_map[ptr] = i;
            ptr = ptr->next;
            i++;
        }
        node_vec.push_back(0);//记得处理vec中的尾节点，面的扣边界了
        ptr = head;
        i = 0;
        while (ptr){
            node_vec[i]->next = node_vec[i+1];
            if (ptr->random){
                int id = node_map[ptr->random];
                node_vec[i]->random = node_vec[id];
            }
            ptr = ptr->next;
            i++;
        }
        return node_vec[0];
    }
};


// 先创建全部的节点，再映射一下
class Solution {
public:
    Node* copyRandomList(Node* head) {
        auto dummy = new Node(-1);
        auto nh = dummy;
        auto tmp = head;
        unordered_map<Node*,Node*> mp;
        while(tmp){
            nh ->next = new Node(tmp->val);
            nh = nh->next;
            mp[tmp] = nh;
            tmp = tmp->next;
        }
        nh = dummy->next;
        tmp = head;
        while(tmp){
            if(tmp->random){
                nh->random = mp[tmp->random];
            }
            tmp = tmp->next;
            nh = nh->next;
        }
        return dummy->next;
    }
};

// 原位构建小弟，无额外空间
class Solution {
public:
    Node* copyRandomList(Node* head) {
        auto dummy = new Node(-1);
        // 构建节点的尾后新节点
        for(auto p = head;p;p = p->next->next){
            auto q = new Node(p->val);
            q->next = p->next;
            p->next = q;
        }
        // 构建random边
        for(auto p = head;p;p = p->next->next){
            if(p->random){
                p->next->random = p->random->next;
            }
        }
        // 拆分链表
        auto cur = dummy;
        for(auto p = head;p;p = p->next){
            auto q = p->next;
            cur->next = q;
            cur = cur->next;
            p->next = q->next;
        }
        return dummy->next;
    }
};