#include <bits/stdc++.h>
using namespace std;
int n, A[10], B[10];
vector<pair<int,int>> g[10];
vector<int> path;
int best;
void dfs_path(int u, int fa, int t){
    if(u == t){
        int len = path.size();
        int ans = 0;
        for(int mask = 0; mask < (1 << len); mask++){
            vector<int> vis(n+1, 0);
            int cnt = 0;
            for(int i = 0; i < len; i++){
                int p = path[i];
                int pick = (mask >> i) & 1 ? A[p] : B[p];
                if(!vis[pick]){ vis[pick] = 1; cnt++; }
            }
            ans = max(ans, cnt);
        }
        best = max(best, ans);
        return;
    }
    for(auto e : g[u]) if(e.first != fa){
        path.push_back(e.first);
        dfs_path(e.first, u, t);
        path.pop_back();
    }
}
int main(){
    cin >> n;
    for(int i = 1; i <= n; i++) cin >> A[i] >> B[i];
    for(int i = 1; i < n; i++){
        int u, v; cin >> u >> v;
        g[u].push_back({v,0}); g[v].push_back({u,0});
    }
    for(int t = 2; t <= n; t++){
        best = 0; path.clear(); path.push_back(1);
        dfs_path(1, 0, t);
        if(t < n) cout << best << " "; else cout << best << "\n";
    }
    return 0;
}
