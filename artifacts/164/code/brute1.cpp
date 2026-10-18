#include <bits/stdc++.h>
using namespace std;
int main(){
    int n, m; cin >> n >> m;
    vector<vector<int>> d(n+1, vector<int>(n+1, INT_MAX/2));
    for(int i=1;i<=n;i++) d[i][i]=0;
    for(int i=0,u,v,w;i<m;i++){ cin>>u>>v>>w; d[u][v]=d[v][u]=min(d[u][v],w); }
    for(int k=1;k<=n;k++) for(int i=1;i<=n;i++) for(int j=1;j<=n;j++)
        d[i][j]=min(d[i][j], max(d[i][k], d[k][j]));
    int q; cin >> q;
    while(q--){ int x,y; cin>>x>>y; if(d[x][y]>=INT_MAX/2) cout<<"impossible\n"; else cout<<d[x][y]<<"\n"; }
    return 0;
}
