#include <iostream>

using namespace std;


int m,n;

int main(){
    int t;
    cin >> t;
    while(t--){
        cin >> n >> m;
        int na = 0;
        int sub = INT_MAX;
        int sum = 0;
        for(int i = 0;i < n;i++){
            for(int j = 0;j < m;j++){
                int x;
                cin >> x;
                sum += abs(x);
                sub = min(sub,abs(x));
                if(x < 0) na++; 
            }
        }
        if(na & 1) sum -= sub * 2;
        cout << sum << endl;
    }
}

/*
python3
    t = int(input())

    for _ in range(t):
        n,m = map(int,input().split())
        res = 0
        sub = 110
        cnt = 0
        for _ in range(n):
            tmp = list(map(int,input().split()))
            for _,val in enumerate(tmp):
                sub = min(sub,abs(val))
                res += abs(val)
                if val < 0:
                    cnt += 1
        if cnt & 1 == 1:
            res -= (2 * sub)
        print(res)

golang
    package main

    import ."fmt"

    func min(i,j int) int{
        if i < j{
            return i
        }
        return j
    }

    func main(){
        var t int
        Scanf("%d",&t)
        for t > 0{
            var m,n int
            Scanf("%d%d",&n,&m)
            var sum,sub,cnt int
            sub = 110
            for i := 0;i < n;i++{
                for j := 0;j < m;j++{
                    var x int
                    Scanf("%d",&x)
                    if(x >= 0){
                        sum += x
                        sub = min(sub,x)
                    }else if x < 0 {
                        cnt++
                        sum += -x
                        sub = min(sub,-x)
                    }
                }
            }        
            if cnt & 1 == 1{
                sum -= 2 * sub
                // Printf("sub is %d",sub)
            }
            Println(sum)
            t--
        }
    }
*/