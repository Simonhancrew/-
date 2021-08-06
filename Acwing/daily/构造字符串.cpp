#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

int main()
{
    int n;
    cin >> n;
    // aabb
    for(int i = 0;i < n;i++){
        if(i % 4 == 0 || i % 4 == 1) cout << 'a';
        else cout << 'b';
    }
}