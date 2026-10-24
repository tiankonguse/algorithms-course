#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<vector<int>> g;
vector<int> color;
void dfs(int u, int fa, int l, int r, set<int>& col){
    if(u < l || u > r) return;
    col.insert(color[u]);
    for(int v: g[u]) if(v!=fa) dfs(v,u,l,r,col);
}
int main(){
    cin >> n >> m;
    g.assign(n+1, {});
    color.assign(n+1, 0);
    for(int i=1;i<=n;i++) cin >> color[i];
    for(int i=1;i<n;i++){int u,v;cin>>u>>v;g[u].push_back(v);g[v].push_back(u);}
    for(int i=0;i<m;i++){
        int l, r, x; cin >> l >> r >> x;
        set<int> col;
        dfs(x, 0, l, r, col);
        cout << col.size() << '\n';
    }
    return 0;
}
