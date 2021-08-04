// 最直观的就是排序之后枚举
// 但是考虑数据范围肯定过不去，要优化，最内层可以考虑二分
#include <vector>
#include <algorithm>

using namespace std;

#if 0
class Solution {
public:
    int triangleNumber(vector<int>& nums) {
        sort(nums.begin(),nums.end());
        int n = nums.size(),res = 0;
        for(int i = 0;i < n - 2;i++){
            for(int j = i + 1;j < n - 1;j++){
                int l = j + 1,r = n - 1;
                while(l < r){
                    int mid = l + r + 1 >> 1;
                    if(nums[i] + nums[j] > nums[mid]) l = mid;
                    else r = mid - 1;
                }
                if(nums[i] + nums[j] <= nums[l]) continue;
                res += l - j;
            }
        }
        return res;
    }   
};
#endif

// 然后找到一个规律，固定a（第一个数），枚举b，适合的c肯定是越来越大的
// 其实是一个双指针，这里逆序找会比较好做

class Solution {
public:
    int triangleNumber(vector<int>& nums) {
        int n = nums.size(),res = 0;
        sort(nums.begin(),nums.end());
        for(int i = 0;i < n;i++){
            for(int j = i - 1, k = 0;j > 0 && k < j;j--){
                while(k < j && nums[k] <= nums[i] - nums[j]) k++;
                res += j - k;
            }
        }
        return res;
    }
};