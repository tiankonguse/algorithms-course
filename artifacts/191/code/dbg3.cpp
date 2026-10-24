#include <bits/stdc++.h>
using namespace std;
int n, m;
vector<pair<int,int>> es;
vector<vector<pair<int,int>>> g;
void build(){
    g.assign(n+1,{});
    for(int i=0;i<m;i++){int u=es[i].first,v=es[i].second;g[u].push_back({v,i*2});g[v].push_back({u,i*2+1});}
}
bool connected_without(int eid){
    int s=es[eid].first,t=es[eid].second;
    if(s==t)return true;
    vector<int> vis(n+1,0); queue<int> q; q.push(s); vis[s]=1;
    while(!q.empty()){int u=q.front();q.pop();for(auto&pr:g[u]){int v=pr.first,id=pr.second;if(id/2==eid)continue;if(!vis[v]){vis[v]=1;q.push(v);}}}
    return vis[t];
}
int main(){
    cin>>n>>m;
    int k,l; cin>>k>>l;
    vector<int> acnt(n+1,0),bcnt(n+1,0);
    for(int i=0;i<k;i++){int x;cin>>x;acnt[x]=1;}
    for(int i=0;i<l;i++){int x;cin>>x;bcnt[x]=1;}
    es.resize(m);
    for(int i=0;i<m;i++)cin>>es[i].first>>es[i].second;
    build();
    for(int i=0;i<m;i++){
        cerr<<"边"<<(i+1)<<": "<<es[i].first<<"-"<<es[i].second<<" is_bridge="<<(!connected_without(i));
    }
    cerr<<"k="<<k<<" l="<<l<<" A={";for(int x=1;x<=n;x++)if(acnt[x])cerr<<x<<",";cerr<<"} B={";for(int x=1;x<=n;x++)if(bcnt[x])cerr<<x<<",";cerr<<"}\n";
    return 0;
}
