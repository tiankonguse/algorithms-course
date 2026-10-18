#include <bits/stdc++.h>
using namespace std;
int n, m, k;
int main(){
    cin >> n >> m >> k;
    vector<tuple<int,int,int,int>> edges;
    for(int i=0;i<m;i++){
        int x,y,l,r; cin>>x>>y>>l>>r;
        edges.push_back({x,y,l,r});
    }
    for(int t=1; t<=k; t++){
        int fa[20];
        auto find = [&](int x){ while(fa[x]!=x) x=fa[x]; return x; };
        for(int j=1;j<=2*n;j++) fa[j]=j;
        bool ok = true;
        for(auto [x,y,l,r] : edges){
            if(l+1 <= t && t <= r){
                int fx = find(x), fy = find(y);
                if(fx == fy){ ok = false; break; }
                fa[fx] = find(y+n);
                fa[find(y)] = find(x+n);
            }
        }
        cout << (ok ? "Yes" : "No") << "\n";
    }
    return 0;
}
