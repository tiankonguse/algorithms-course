// 最短路最大值（暴力）：Floyd + 每查询两两取 max
#include <bits/stdc++.h>
using namespace std;
int main(){
    string prob; cin>>prob; int pid; cin>>pid;
    int n,m; cin>>n>>m;
    vector<vector<long long>> d(n+1, vector<long long>(n+1, 1e18));
    for(int i=1;i<=n;i++) d[i][i]=0;
    for(int i=0;i<m;i++){int u,v,w;cin>>u>>v>>w;d[u][v]=d[v][u]=w;}
    for(int k=1;k<=n;k++) for(int i=1;i<=n;i++) for(int j=1;j<=n;j++)
        if(d[i][j]>d[i][k]+d[k][j]) d[i][j]=d[i][k]+d[k][j];
    int q; cin>>q;
    while(q--){
        int k; cin>>k;
        vector<int> a(k);
        for(int i=0;i<k;i++) cin>>a[i];
        long long ans=0;
        for(int i=0;i<k;i++) for(int j=0;j<k;j++) ans=max(ans,d[a[i]][a[j]]);
        cout<<ans<<"\n";
    }
    return 0;
}
