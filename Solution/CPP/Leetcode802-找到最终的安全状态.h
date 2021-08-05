// 就是一个topsort，入度变出度
#include <vector>
#include <queue>

using namespace std;

class Solution {
public:
    vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
        int n = graph.size();
        vector<vector<int>> rev(n);
        vector<int> d(n);
        for(int i = 0;i < n;i++){
            for(auto b:graph[i]){
                int a = i;
                rev[b].push_back(a);
                d[a]++;
            }
        }
        queue<int> q;
        for(int i = 0;i < n;i++){
            if(!d[i]) q.push(i);
        }
        while(q.size()){
            int t = q.front();
            q.pop();
            for(auto a:rev[t]){
                if(--d[a] == 0){
                    q.push(a);
                }
            }
        }
        vector<int> res;
        for(int i = 0;i < n;i++){
            if(d[i] == 0) res.push_back(i);
        }
        return res;
    }
};