#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<vector<pair<int,int>>> g;
long long dist[20][20];
void dfs(int s, int u, int fa, long long cur){
    dist[s][u] = cur;
    for(auto [v,w]: g[u]) if(v!=fa) dfs(s,v,u,cur+w);
}
int main(){
    cin >> n;
    g.assign(n+1, {});
    for(int i=1;i<n;i++){int u,v,w;cin>>u>>v>>w;g[u].push_back({v,w});g[v].push_back({u,w});}
    memset(dist, 0x3f, sizeof dist);
    for(int s=1;s<=n;s++) dfs(s,s,0,0);
    cin >> m;
    for(int i=0;i<m;i++){
        int x,y; cin >> x >> y;
        if(x==y){ cout << -1 << '\n'; continue; }
        long long best = LLONG_MAX;
        for(int a=x;a<=y;a++) for(int b=a+1;b<=y;b++) best = min(best, dist[a][b]);
        cout << best << '\n';
    }
    return 0;
}
