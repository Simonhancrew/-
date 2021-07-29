#include <vector>
#include <algorithm>

using namespace std;

// 偶数行转换成正常排列
 
class Solution {
public:
    int getrv(int row,int label){
        return (1 << row) + (1 << row - 1) - 1 - label;
    }
    vector<int> pathInZigZagTree(int label) {
        int row = 1;
        while((1 << row) <=label ){
            row++;
        }
        if(!(row & 1)){
            label = getrv(row,label);
        }
        vector<int> res;
        while(row > 0){
            if(row & 1){
                res.push_back(label);
            }else{
                res.push_back(getrv(row,label));
            }
            row--,label >>= 1;
        }
        reverse(res.begin(),res.end());
        return res;
    }
};