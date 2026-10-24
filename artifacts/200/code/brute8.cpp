// 仙人掌路径翻转（暴力）：真实图操作。最短/最长路唯一（环长奇数）。
// 子仙人掌 = 删掉根到 x 的最短路与最长路（两条互补弧）的边后，含 x 的连通块。
#include <bits/stdc++.h>
using namespace std;
int n, m, q;
vector<pair<int,int>> es;
vector<vector<int>> g;
vector<int> color;  // 1=黑
vector<int> dep;
// 找 x 到 1 的最短路径（唯一）：BFS 深度 + 回溯
vector<int> pathShort(int x){
    vector<int> p;
    if(x==1) return {1}; // 根：路径含自己（课程翻转含根）
    // 最短路从 x 沿 dep 递减走
    int cur=x;
    p.push_back(x);
    while(cur!=1){
        for(int v:g[cur]) if(dep[v]==dep[cur]-1){cur=v;break;}
        p.push_back(cur);
    }
    return p;
}
// 最长简单路径 x -> 1（唯一）
vector<int> bestPath;
void dfsLong(int u,int fa,vector<int>& path){
    path.push_back(u);
    if(u==1){ if(path.size()>bestPath.size()) bestPath=path; }
    for(int v:g[u]) if(v!=fa&&find(path.begin(),path.end(),v)==path.end()) dfsLong(v,u,path);
    path.pop_back();
}
vector<int> pathLong(int x){
    bestPath.clear();
    vector<int> path;
    dfsLong(x,0,path);
    return bestPath;
}
set<pair<int,int>> edgeSet(const vector<int>& p){
    set<pair<int,int>> s;
    for(int i=0;i+1<(int)p.size();i++){
        int a=p[i],b=p[i+1];
        s.insert({min(a,b),max(a,b)});
    }
    return s;
}
int countBlack(int x){
    auto s1=edgeSet(pathShort(x));
    auto s2=edgeSet(pathLong(x));
    set<pair<int,int>> del;
    for(auto&e:s1) del.insert(e);
    for(auto&e:s2) del.insert(e);
    // BFS from x
    set<pair<int,int>> valid;
    for(auto&e:es) if(!del.count({min(e.first,e.second),max(e.first,e.second)})) valid.insert({min(e.first,e.second),max(e.first,e.second)});
    vector<int> vis(n+1);
    queue<int> qq; qq.push(x); vis[x]=1;
    int ans=0;
    while(!qq.empty()){
        int u=qq.front(); qq.pop();
        if(color[u]) ans++;
        for(int v:g[u]){
            if(!vis[v]&&valid.count({min(u,v),max(u,v)})){vis[v]=1;qq.push(v);}
        }
    }
    return ans;
}
int main(){
    string prob; cin>>prob; int pid; cin>>pid;
    cin>>n>>m>>q;
    es.resize(m);
    g.assign(n+1,{});
    for(int i=0;i<m;i++){cin>>es[i].first>>es[i].second;g[es[i].first].push_back(es[i].second);g[es[i].second].push_back(es[i].first);}
    color.assign(n+1,1);
    // BFS 深度
    dep.assign(n+1,-1);
    queue<int> qq; qq.push(1); dep[1]=0;
    while(!qq.empty()){
        int u=qq.front(); qq.pop();
        for(int v:g[u]) if(dep[v]==-1){dep[v]=dep[u]+1;qq.push(v);}
    }
    while(q--){
        int op,x; cin>>op>>x;
        if(op==1){
            auto p=pathShort(x);
            for(int u:p) color[u]^=1;
        } else if(op==2){
            auto p=pathLong(x);
            for(int u:p) color[u]^=1;
        } else {
            cout<<countBlack(x)<<"\n";
        }
    }
    return 0;
}
