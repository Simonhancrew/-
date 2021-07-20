#include <vector>

using namespace std;

class Solution {
public:
    int minPairSum(vector<int>& nums) {
        int l = 0,r = nums.size() - 1;
        quicksort(nums,l,r);
        int mm = 0;
        for(int i = 0,j = nums.size() - 1;i < j;i++,j--){
            mm = max(mm,nums[i] + nums[j]);
        }
        return mm;
    }
    void quicksort(vector<int> &nums,int l,int r){
        if(l >= r) return;
        int i = l - 1,j = r + 1;
        int x = nums[l + r >> 1];
        while(i < j){
            do i++; while(nums[i] < x);
            do j--; while(nums[j] > x);
            if(i < j) swap(nums[i],nums[j]);
        }

        quicksort(nums,l,j);
        quicksort(nums,j + 1,r);
    }
};


class Solution {
public:
    int minPairSum(vector<int>& nums) {
        sort(nums.begin(),nums.end());
        int mm = 0;
        int l = 0,r = nums.size() - 1;
        while(l < r){
            mm = max(mm,nums[l++] + nums[r--]);
        }
        return mm;
    }   
};