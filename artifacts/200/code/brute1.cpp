// 仙人掌删边方案数（暴力）：判仙人掌（每边至多一环）+ 枚举删边子集数有效删法（删后连通）
#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<pair<int,int>> es;
bool conn(int mask){
    vector<vector<int>> g(n+1);
    for(int i=0;i<m;i++) if(!(mask>>i&1)){
        g[es[i].first].push_back(es[i].second);
        g[es[i].second].push_back(es[i].first);
    }
    vector<int> vis(n+1);
    queue<int> q; q.push(1); vis[1]=1;
    while(!q.empty()){
        int u=q.front();q.pop();
        for(int v:g[u]) if(!vis[v]){vis[v]=1;q.push(v);}
    }
    for(int i=1;i<=n;i++) if(!vis[i]) return false;
    return true;
}
// 判定仙人掌：每条边删掉后，端点间简单路径数 <= 1（0=割边, 1=恰一环, >=2 非仙人掌）
long long pathCnt(int a,int b,int skip,vector<vector<int>>& g,int forb,vector<int>& vis){
    if(a==b) return 1;
    vis[a]=1; long long s=0;
    for(int v:g[a]) if(v!=forb&&!vis[v]) s+=pathCnt(v,b,skip,g,forb,vis);
    vis[a]=0; return s;
}
bool isCactus(){
    vector<vector<int>> g(n+1);
    for(auto&e:es){g[e.first].push_back(e.second);g[e.second].push_back(e.first);}
    for(int i=0;i<m;i++){
        int a=es[i].first,b=es[i].second;
        vector<int> vis(n+1);
        long long c=0;
        function<void(int)> dfs=[&](int u){
            if(c>=2) return;
            if(u==b){c++;return;}
            vis[u]=1;
            for(int v:g[u]){
                if(vis[v]) continue;
                if((u==a&&v==b)||(u==b&&v==a)) continue;
                dfs(v);
            }
            vis[u]=0;
        };
        dfs(a);
        if(c>=2) return false;
    }
    return true;
}
int main(){
    string prob; cin>>prob; int pid; cin>>pid;
    cin>>n>>m;
    es.clear();
    for(int i=0;i<m;i++){int k,x;cin>>k>>x;for(int j=2;j<=k;j++){int y;cin>>y;es.push_back({x,y});x=y;}}
    m=(int)es.size();
    if(!isCactus()){cout<<0<<"\n";return 0;}
    long long ans=0;
    for(int mask=0;mask<(1<<m);mask++) if(conn(mask)) ans++;
    cout<<ans<<"\n";
    return 0;
}
