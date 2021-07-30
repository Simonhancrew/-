#include <vector>

using namespace std;

class Solution {
public:
    vector<int> singleNumber(vector<int>& nums) {
        int cur = 0;
        for(auto num:nums){
            cur ^= num;
        }
        int dif = 1;
        while((dif & cur) == 0){
            dif <<= 1;
        }
        int a = 0,b = 0;
        for(auto num:nums){
            if(dif & num){ 
                a ^= num;
            }else{
                b ^= num;
            }
        }
        return vector<int> {a,b};
    }
};