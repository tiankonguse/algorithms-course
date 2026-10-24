// 仙人掌最短路（暴力）：Floyd
#include <bits/stdc++.h>
using namespace std;
int main(){
    string prob; cin>>prob; int pid; cin>>pid;
    int n,m,q; cin>>n>>m>>q;
    vector<vector<long long>> d(n+1, vector<long long>(n+1, 1e18));
    for(int i=1;i<=n;i++) d[i][i]=0;
    for(int i=0;i<m;i++){int u,v,w;cin>>u>>v>>w;d[u][v]=d[v][u]=w;}
    for(int k=1;k<=n;k++) for(int i=1;i<=n;i++) for(int j=1;j<=n;j++)
        if(d[i][j]>d[i][k]+d[k][j]) d[i][j]=d[i][k]+d[k][j];
    while(q--){int x,y;cin>>x>>y;cout<<d[x][y]<<"\n";}
    return 0;
}
