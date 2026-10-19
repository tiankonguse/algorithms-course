#include <bits/stdc++.h>
using namespace std;
int main(){
    int n, m; cin >> n >> m;
    vector<int> a(n+1);
    for(int i=1;i<=n;i++) cin >> a[i];
    for(int t=0;t<m;t++){
        int op,l,r,x; cin >> op >> l >> r >> x;
        if(op==1){
            for(int i=l;i<=r;i++) if(a[i] > x) a[i] -= x;
        } else {
            int cnt = 0;
            for(int i=l;i<=r;i++) if(a[i] == x) cnt++;
            cout << cnt << '\n';
        }
    }
    return 0;
}
