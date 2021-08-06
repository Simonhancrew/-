#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int shortestPathLength(vector<vector<int>>& graph) {
        typedef pair<int,int> PII;
        int n = graph.size(),INF = 1e9;
        vector<vector<int>> dp(1<<n,vector<int>(n,INF));
        queue<PII> q;
        for(int i = 0;i < n;i++){
            dp[1 << i][i] = 0;
            q.push({1 << i,i});
        }
        while(q.size()){
            auto t = q.front();
            q.pop();
            for(auto node:graph[t.second]){
                int a = t.first | 1 << node,b = node;
                if(dp[a][b] > dp[t.first][t.second] + 1){
                    dp[a][b] = dp[t.first][t.second] + 1;
                    q.push({a,b});
                }
            }
        }
        int res = INF;
        for(int i = 0;i < n;i++){
            res = min(res,dp[(1 << n) - 1][i]);
        }
        return res; 
    }
};