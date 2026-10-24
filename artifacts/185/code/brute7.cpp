#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<vector<int>> g;
long long d[15];
void dfs(int u, int fa, long long cur){
    d[u] = cur;
    for(int v: g[u]) if(v!=fa) dfs(v,u,cur+1);
}
int main(){
    cin >> n;
    g.assign(n+1, {});
    for(int i=1;i<n;i++){int u,v;cin>>u>>v;g[u].push_back(v);g[v].push_back(u);}
    cin >> m;
    vector<char> black(n+1, 1);
    for(int i=0;i<m;i++){
        char op; cin >> op;
        if(op == 'C'){ int x; cin >> x; black[x] ^= 1; }
        else {
            vector<int> bs;
            for(int u=1;u<=n;u++) if(black[u]) bs.push_back(u);
            if(bs.empty()){ cout << -1 << '\n'; continue; }
            if(bs.size() == 1){ cout << 0 << '\n'; continue; }
            long long mx = 0;
            for(int a: bs) for(int b: bs){
                if(a == b) continue;
                dfs(a, 0, 0);
                mx = max(mx, d[b]);
            }
            cout << mx << '\n';
        }
    }
    return 0;
}
