#include <bits/stdc++.h>
using namespace std;
int main(){
    int n, m; cin >> n >> m;
    vector<int> fa(n+1, 0);
    for(int i=2;i<=n;i++) cin >> fa[i];
    for(int t=0;t<m;t++){
        int op; cin >> op;
        if(op==1){
            int x,y,z; cin >> x >> y >> z;
            for(int i=x;i<=y;i++) fa[i] = max(1, fa[i]-z);
        } else {
            int x,y; cin >> x >> y;
            set<int> s;
            for(int p=x;;p=fa[p]){ s.insert(p); if(p==1||p==0) break; }
            int ans = 1;
            for(int p=y;;p=fa[p]){ if(s.count(p)){ ans=p; break; } if(p==1||p==0) break; }
            cout << ans << '\n';
        }
    }
    return 0;
}
