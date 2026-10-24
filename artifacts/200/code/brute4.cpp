// 仙人掌最大独立集（暴力）：2^n 枚举
#include <bits/stdc++.h>
using namespace std;
int n,m;
vector<int> adj[20];
int main(){
    string prob; cin>>prob; int pid; cin>>pid;
    cin>>n>>m;
    for(int i=0;i<m;i++){int u,v;cin>>u>>v;adj[u].push_back(v);adj[v].push_back(u);}
    vector<int> w(n+1);
    for(int i=1;i<=n;i++) cin>>w[i];
    int best=0;
    for(int mask=0;mask<(1<<n);mask++){
        bool ok=true;
        for(int i=1;i<=n&&ok;i++) if(mask>>(i-1)&1)
            for(int v:adj[i]) if(mask>>(v-1)&1){ok=false;break;}
        if(!ok) continue;
        int s=0;
        for(int i=1;i<=n;i++) if(mask>>(i-1)&1) s+=w[i];
        best=max(best,s);
    }
    cout<<best<<"\n";
    return 0;
}
