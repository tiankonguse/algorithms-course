// 仙人掌加边方案数（暴力）：枚举可加边的子集，子集加入后仍仙人掌则计数
#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<pair<int,int>> es;
bool isCactus(const vector<pair<int,int>>& eds){
    int mm=eds.size();
    vector<vector<int>> g(n+1);
    for(auto&e:eds){g[e.first].push_back(e.second);g[e.second].push_back(e.first);}
    for(int i=0;i<mm;i++){
        int a=eds[i].first,b=eds[i].second;
        // 删掉 e 后 a 到 b 的简单路径数
        long long cnt=0;
        vector<int> vis(n+1,0);
        function<void(int)> dfs=[&](int u){
            if(cnt>=2) return;
            if(u==b){cnt++;return;}
            vis[u]=1;
            for(int v:g[u]){
                if(vis[v]) continue;
                if((u==a&&v==b)||(u==b&&v==a)) continue;
                dfs(v);
            }
            vis[u]=0;
        };
        dfs(a);
        if(cnt>=2) return false;
    }
    return true;
}
int main(){
    string prob; cin>>prob; int pid; cin>>pid;
    int t; cin>>t;
    while(t--){
        cin>>n>>m;
        es.resize(m);
        set<pair<int,int>> have;
        for(int i=0;i<m;i++){cin>>es[i].first>>es[i].second;have.insert({es[i].first,es[i].second});}
        if(!isCactus(es)){cout<<0<<"\n";continue;}
        vector<pair<int,int>> cand;
        for(int i=1;i<=n;i++) for(int j=i+1;j<=n;j++)
            if(!have.count({i,j})&&!have.count({j,i})) cand.push_back({i,j});
        long long ans=0;
        for(int mask=0;mask<(1<<cand.size());mask++){
            vector<pair<int,int>> eds=es;
            for(int i=0;i<(int)cand.size();i++) if(mask>>i&1) eds.push_back(cand[i]);
            if(isCactus(eds)) ans++;
        }
        cout<<ans%998244353<<"\n";
    }
    return 0;
}
