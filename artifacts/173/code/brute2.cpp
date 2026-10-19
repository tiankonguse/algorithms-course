#include <bits/stdc++.h>
using namespace std;
int main(){
    int n, m; cin >> n >> m;
    vector<int> arr(n+1);
    for(int i=1;i<=n;i++) cin >> arr[i];
    m++;
    vector<vector<long long>> val(m+2, vector<long long>(n+1, 0));
    for(int j=1;j<=m;j++) for(int x=1;x<=n;x++) val[j][x] = arr[x];
    for(int t=2;t<=m;t++){
        int op; cin >> op;
        if(op==1){
            int l,r,v; cin >> l >> r >> v;
            for(int j=t;j<=m;j++) for(int x=l;x<=r;x++) val[j][x] += v;
        } else {
            int x,v; cin >> x >> v;
            int cnt = 0;
            for(int j=1;j<=t-1;j++) if(val[j][x] >= v) cnt++;
            cout << cnt << '\n';
        }
    }
    return 0;
}
