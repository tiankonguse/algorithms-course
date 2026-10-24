// 仙人掌最长路线（暴力）：枚举起点，DFS 不重边走最大 trail 到 1
#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<pair<int,int>> es;
vector<vector<int>> g;
int best;
void dfs(int u,int mask,int dep){
    if(u==1) best=max(best,dep);
    for(auto&e:es){
        int a=e.first,b=e.second;
        // 边 e 已用？
        // 用边 id：需要从 g 推断。改用全局 edges 数组按 id。
    }
}
int main(){
    string prob; cin>>prob; int pid; cin>>pid;
    cin>>n>>m;
    es.resize(m);
    g.assign(n+1,{});
    for(int i=0;i<m;i++){cin>>es[i].first>>es[i].second;g[es[i].first].push_back(es[i].second);g[es[i].second].push_back(es[i].first);}
    best=0;
    // 从任意起点出发；枚举起点 s，DFS 状态 (cur, mask)
    function<void(int,int,int)> run=[&](int s,int mask,int dep){
        if(s==1) best=max(best,dep);
        for(int i=0;i<m;i++){
            if(mask>>i&1) continue;
            int a=es[i].first,b=es[i].second;
            int nx = (a==s)?b:((b==s)?a:-1);
            if(nx!=-1) run(nx, mask|(1<<i), dep+1);
        }
    };
    for(int s=1;s<=n;s++) run(s,0,0);
    cout<<best<<"\n";
    return 0;
}
