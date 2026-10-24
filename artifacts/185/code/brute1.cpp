#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<vector<int>> g;
vector<int> w;
long long d[15];
void dfs(int u, int fa, long long cur){
    d[u] = cur;
    for(int v: g[u]) if(v!=fa) dfs(v,u,cur+1);
}
int main(){
    cin >> n >> m;
    g.assign(n+1, {});
    w.assign(n+1, 0);
    for(int i=1;i<=n;i++) cin >> w[i];
    for(int i=1;i<n;i++){int u,v;cin>>u>>v;g[u].push_back(v);g[v].push_back(u);}
    int lastAns = 0;
    for(int i=0;i<m;i++){
        int op, x, y; cin >> op >> x >> y;
        x ^= lastAns; y ^= lastAns;
        if(op == 0){
            dfs(x, 0, 0);
            long long s = 0;
            for(int u=1;u<=n;u++) if(d[u] <= y) s += w[u];
            cout << s << '\n';
            lastAns = s;
        } else {
            w[x] = y;
        }
    }
    return 0;
}
