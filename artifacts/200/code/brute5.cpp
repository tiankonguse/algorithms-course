// 仙人掌直径（暴力）：路径格式输入 → 建图 → Floyd
#include <bits/stdc++.h>
using namespace std;
int main(){
    string prob; cin>>prob; int pid; cin>>pid;
    int n,m; cin>>n>>m;
    vector<vector<int>> d(n+1, vector<int>(n+1, 10000));
    for(int i=1;i<=n;i++) d[i][i]=0;
    for(int i=0;i<m;i++){
        int k,x; cin>>k>>x;
        for(int j=2;j<=k;j++){int y;cin>>y; d[x][y]=d[y][x]=1; x=y;}
    }
    for(int k=1;k<=n;k++) for(int i=1;i<=n;i++) for(int j=1;j<=n;j++)
        if(d[i][j]>d[i][k]+d[k][j]) d[i][j]=d[i][k]+d[k][j];
    int ans=0;
    for(int i=1;i<=n;i++) for(int j=1;j<=n;j++) ans=max(ans,d[i][j]);
    cout<<ans<<"\n";
    return 0;
}
