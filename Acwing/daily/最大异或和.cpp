#include <iostream>
#include <algorithm>

using namespace std;

//美团面试题
//首先，前缀和处理成数组的xor和。之后就可以转换成异或对的处理。
//异或对处理，看做二进制的数，尽量每一位都选择和自己不一样的数，这样异或的值是最大的，这个结构可以选择trie树
//最后需要考虑的是范围m的限制。只要考虑移除之前的i - m - 1的数就可以了,给每个节点一个cnt值，后续处理，0代表这个点没经过，1代表经过了

const int N = (1e5 + 10) * 31, M = 100010;

int n, m;
int s[M];
int son[N][2], cnt[N], idx;

//注意insert的操作，这里没有真的删除这个值，利用cnt数组做一个判断
void insert(int x, int v)
{
    int p = 0;
    for (int i = 30; i >= 0; i -- )
    {
        int u = x >> i & 1;
        if (!son[p][u]) son[p][u] = ++ idx;
        p = son[p][u];
        cnt[p] += v;
    }
}

int query(int x)
{
    int res = 0, p = 0;
    for (int i = 30; i >= 0; i -- )
    {
        int u = x >> i & 1;
        if (cnt[son[p][!u]]) p = son[p][!u], res = res * 2 + !u;
        else p = son[p][u], res = res * 2 + u;
    }
    return res;
}

int main()
{
    cin >> n >> m;
    for (int i = 1; i <= n; i ++ )
    {
        int x;
        cin >> x;
        s[i] = s[i - 1] ^ x;
    }

    int res = 0;
    insert(s[0], 1);

    for (int i = 1; i <= n; i ++ )
    {
        if (i > m) insert(s[i - m - 1], -1);
        res = max(res, query(s[i]) ^ s[i]);
        insert(s[i], 1);
    }

   cout << res << endl;
}