#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int findUnsortedSubarray(vector<int>& nums) {
        vector<int> cp = nums;
        int n = nums.size();
        sort(nums.begin(),nums.end());
        int l = -1,r = 0;
        for(int i = 0;i < n;i++){
            if(l == -1 && nums[i] != cp[i]) l = i;
            else if(l != -1 && nums[i] != cp[i]) r = i;
        }
        if(l == -1) return 0;
        return r - l + 1;
    }
};