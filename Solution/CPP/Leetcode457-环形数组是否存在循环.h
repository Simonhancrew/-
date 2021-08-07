#include <vector>

using namespace std;


// 就是判断图有无环的问题，需要注意的是只能往一个方向走，自环不算环。

class Solution {
public:
    bool circularArrayLoop(vector<int>& nums) {
        int n = nums.size();
        vector<bool> st(n,0);
        for(int i = 0;i < n;i++){
            if(st[i]) continue;
            vector<bool> visit(n,0);
            int start = i,d = nums[start] > 0;
            int last = -1;
            do{
                int ne = ((start + nums[start])% n + n) % n;
                last = nums[start];
                visit[start] = true,st[start] = true;
                start = ne;
            }while(start != i && !visit[start] && (d ^ (nums[start] > 0) == 0));
            // cout << last << endl;
            if(last % n && visit[start]) return true;
        }
        return false;
    }
};