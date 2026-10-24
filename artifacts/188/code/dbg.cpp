#include <bits/stdc++.h>
using namespace std;
int n, m, kind; vector<pair<int,int>> es; long long cnt=0;
void dfs(vector<int>& verts, int used, int cur) {
    cnt++;
    if (cnt > 5000000) { cout << "OVERFLOW\n"; exit(0); }
    if (used == (1<<m)-1) return;
    for (int e=0;e<m;e++) if(!(used>>e&1)){
        int u=es[e].first,v=es[e].second;
        if(kind==0){ if(u==cur){verts.push_back(v);dfs(verts,used|1<<e,v);verts.pop_back();} }
        else { if(u==cur){verts.push_back(v);dfs(verts,used|1<<e,v);verts.pop_back();}
               if(v==cur){verts.push_back(u);dfs(verts,used|1<<e,u);verts.pop_back();} }
    }
}
int main(int argc,char**argv){ios::sync_with_stdio(false);cin.tie(nullptr);
 kind=(argv[1][0]=='d'?0:1); cin>>n>>m; es.resize(m);
 for(int i=0;i<m;i++)cin>>es[i].first>>es[i].second;
 for(int s=1;s<=n;s++){vector<int> v{s};dfs(v,0,s);}
 cout<<"nodes="<<cnt<<"\n";}
