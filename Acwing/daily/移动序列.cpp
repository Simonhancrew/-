#include <iostream>

using namespace std;

const int N = 100;
int a[N];

int main(){
	int t;
	cin >> t;
	while(t --){
		int n;
		cin >> n;
		for(int i = 1;i <= n;i++) cin >> a[i];
		int l,r;
		for(l = 1;l <= n;l++){
			if(a[l] == 1) break;
		}
		for(r = n;r >= 0;r--){
			if(a[r] == 1) break;
		}
		int ans = 0;
		for(int i = l;i <= r;i++){
			if(a[i] == 0) ans++;
		}
		cout << ans << endl;
	}
}