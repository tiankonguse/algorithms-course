#include <bits/stdc++.h>
using namespace std;
int main(){
    int n, m; cin >> n >> m;
    vector<int> a(n+1);
    for(int i=1;i<=n;i++) cin >> a[i];
    for(int t=0;t<m;t++){
        int op,l,r; cin >> op >> l >> r;
        if(op==1){
            int x,y; cin >> x >> y;
            for(int i=l;i<=r;i++) if(a[i]==x) a[i]=y;
        } else {
            int k; cin >> k;
            vector<int> b;
            for(int i=l;i<=r;i++) b.push_back(a[i]);
            sort(b.begin(), b.end());
            cout << b[k-1] << '\n';
        }
    }
    return 0;
}
