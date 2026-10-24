#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<char> val;
vector<vector<int>> g;
string s;
bool found;
void dfsPath(int u, int fa, int tgt, vector<int>& path){
    if(found) return;
    path.push_back(u);
    if(u==tgt){ found = true; return; }
    for(int v: g[u]) if(v!=fa) dfsPath(v,u,tgt,path);
    if(!found) path.pop_back();
}
bool ok(const vector<int>& path){
    if((int)path.size() % m != 0) return false;
    int reps = path.size() / m;
    string built;
    for(int x: path) built += val[x];
    for(int r=0;r<reps;r++)
        if(built.substr(r*m, m) != s) return false;
    return true;
}
int main(){
    int t; cin >> t;
    while(t--){
        cin >> n >> m;
        val.assign(n+1, 0);
        for(int i=1;i<=n;i++) cin >> val[i];
        g.assign(n+1, {});
        for(int i=1;i<n;i++){int a,b;cin>>a>>b;g[a].push_back(b);g[b].push_back(a);}
        s.clear();
        for(int i=1;i<=m;i++){char c;cin>>c;s+=c;}
        long long ans = 0;
        for(int u=1;u<=n;u++) for(int v=1;v<=n;v++){
            if(u==v) continue;
            vector<int> path; found = false;
            dfsPath(u,0,v,path);
            if(ok(path)) ans++;
        }
        cout << ans << '\n';
    }
    return 0;
}
