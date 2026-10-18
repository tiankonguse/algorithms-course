#include <bits/stdc++.h>
using namespace std;
struct D { int f[20]; int F(int x){ return f[x]==x?x:f[x]=F(f[x]); } void ini(int n){ for(int i=1;i<=n;i++) f[i]=i; } };
int main(){
    int n, m; cin >> n >> m;
    vector<pair<int,int>> e(m+1);
    for(int i=1;i<=m;i++) cin >> e[i].first >> e[i].second;
    int q; cin >> q;
    while(q--){
        int x, y, z; cin >> x >> y >> z;
        int ans = 0;
        for(int lim = 0; lim <= m; lim++){
            D d; d.ini(n);
            for(int i=1;i<=lim;i++) d.f[d.F(e[i].first)] = d.F(e[i].second);
            int fx = d.F(x), fy = d.F(y);
            int cnt = 0;
            if(fx == fy){ for(int i=1;i<=n;i++) if(d.F(i)==fx) cnt++; }
            else { for(int i=1;i<=n;i++) if(d.F(i)==fx||d.F(i)==fy) cnt++; }
            if(cnt >= z){ ans = lim; break; }
        }
        cout << ans << "\n";
    }
    return 0;
}
