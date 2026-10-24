#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<vector<pair<int,int>>> g;
vector<long long> d;
void dfs(int u, int fa, long long cur){
    d[u] = cur;
    for(auto [v,w]: g[u]) if(v!=fa) dfs(v,u,cur+w);
}
int main(){
    cin >> n >> m;
    g.assign(n+1, {});
    for(int i=1;i<n;i++){int u,v,w;cin>>u>>v>>w;g[u].push_back({v,w});g[v].push_back({u,w});}
    vector<pair<int,int>> pr(m);
    for(int i=0;i<m;i++) cin >> pr[i].first >> pr[i].second;
    long long best = LLONG_MAX;
    for(int x=1;x<=n;x++){
        d.assign(n+1,0); dfs(x,0,0);
        long long mx = 0;
        for(auto [a,b]: pr) mx = max(mx, d[a]+d[b]);
        best = min(best, mx);
    }
    cout << best << '\n';
    return 0;
}
