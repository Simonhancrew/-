#include <iostream>
#include <algorithm>

using namespace std;

int x[7][3] = {{0, 0, 1}, {0, 1, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 1}, {1, 0, 1}, {1, 1, 1}};

int main(){
    int n;
    cin >> n;
    while(n --){
        int a,b,c;
        cin >> a >> b >> c;
        int res = 0;
        for(int i = 0;i < (1 << 7);i++){
            int sa = 0,sb = 0,sc = 0,cnt = 0;
            for(int j = 0;j <= 6;j++){
                if((i >> j) & 1){
                    cnt++;
                    sa += x[j][0];
                    sb += x[j][1];
                    sc += x[j][2];
                }
            }
            if(sa <= a && sb <= b && sc <= c) res = max(res,cnt);
        }
        cout << res << endl;
    }
    return 0;
}