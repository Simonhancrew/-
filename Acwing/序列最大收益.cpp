#include <iostream>
#include <cstring>
#include <algorithm>

//中兴2021校招笔试题，dp，最长上升子序列的变化

using namespace std;


//数据保证wij = wji,wii = 0

const int N = 210;
int n,k,m;
int a[N],w[N][N];
int dp[N][N];//前i的序列，删除了j个数，且第i个数是保留的

int main()
{
    cin >> n >> k >> m;
    for(int i = 1;i <= m;i++) cin >> a[i];
    for(int i = 1;i <= n;i++){
        for(int j = 1;j <= n;j++){
            cin >> w[i][j];
        }
    }
    //初始状态一定会包含开头,反证，不包含开头的话，如果加入开头，会引入一段>=0的区间，矛盾
    dp[1][0] = 0;
    for(int i = 2;i <= m;i++){
        for(int j = 0;j <= k;j++){
            //倒数第二个数是现在序列的第t个数
            for(int t = 1;t < i;t++){
                if(j >= i - t - 1)//t -> i 区间中删除了 i - t - 1个数
                    //之前的区间取max， + Wti
                    dp[i][j] = max(dp[i][j],dp[t][j - (i - t -1)] + w[a[t]][a[i]]);
            }       
        }
    }
    int res = 0;
    for(int i = 0;i <= k;i++) res = max(res,dp[m][i]);
    cout << res << endl;
}

/*
    ### 首先考虑最后一步，最后一个元素选不选呢？
    答案是一定要选的，因为保证了matrix里面的元素都是大于0的，容易反证这个区间如果加上的话肯定是增加的。同理，第一个元素也肯定是要选的。
    ### 其次，怎么知道删除了几个呢？
    状态转移方程需要增加一维状态，标识删除了多少个。
    倒退一步，之前的元素是那个？可以是1 -> i-1这其中的选，记为t,t->i这中间删除了i-t-1个数。
    要之前的子状态最大，既是dp[t][j - (i - t - 1)]最大
    所以容易推出状态转移方程，dp[i][j],前i个数，删除j个，且要保留最后一个i

*/