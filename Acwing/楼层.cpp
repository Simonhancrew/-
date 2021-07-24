#include <iostream>
#include <cstring>
#include <algorithm>

//上取整的应用，c = （a + b - 1） / b

using namespace std;

int main()
{
    int t;
    cin >> t;
    while(t --){
        int n,x;
        cin >> n >> x;
        if(n <= 2) cout << 1 << endl;
        else cout << (n - 3 + x)/x + 1 << endl;
    }
    return 0;
}