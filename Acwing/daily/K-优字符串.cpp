#include <iostream>

//谷歌的一道题

using namespace std;
const int N =  2e5 + 10;

char s[N];
int T,n,k;

int main()
{
    cin >> T;
    for(int i = 1;i <= T;i++)
    {
        int n,k;
        scanf("%d%d",&n,&k);
        scanf("%s",s + 1);
        int cnt = 0;
        for(int i = 1;i <= n / 2;i++){
            if(s[i] != s[n - i + 1]) cnt ++;
        }
        printf("Case #%d: %d\n",i,abs(cnt - k));
    }
    return 0;
}