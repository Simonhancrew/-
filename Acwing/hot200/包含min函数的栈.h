// 一个简单的设计题，双栈
class MinStack {
public:
    /** initialize your data structure here. */
    MinStack() {
        
    }
    
    void push(int x) {
        stk.push(x);
        if(mini.size()) x = min(x,mini.top());
        mini.push(x);
    }
    
    void pop() {
        stk.pop();
        mini.pop();
    }
    
    int top() {
        return stk.top();
    }
    
    int getMin() {
        return mini.top();
    }
private:
    stack<int> stk,mini;
};
