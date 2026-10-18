#include <bits/stdc++.h>
using namespace std;
int main(){
    int n, m; cin >> n >> m;
    vector<vector<int>> fa(m+1), siz(m+1);
    fa[0].resize(n+1); siz[0].resize(n+1);
    for(int i=1;i<=n;i++){ fa[0][i]=i; siz[0][i]=1; }
    for(int v=1; v<=m; v++){
        int op; cin >> op;
        if(op==1){
            int x,y; cin >> x >> y;
            fa[v]=fa[v-1]; siz[v]=siz[v-1];
            // find (无压缩)
            int fx=x; while(fa[v][fx]!=fx) fx=fa[v][fx];
            int fy=y; while(fa[v][fy]!=fy) fy=fa[v][fy];
            if(fx!=fy){
                if(siz[v][fx]>=siz[v][fy]){ fa[v][fy]=fx; siz[v][fx]+=siz[v][fy]; }
                else { fa[v][fx]=fy; siz[v][fy]+=siz[v][fx]; }
            }
        } else if(op==2){
            int x; cin >> x;
            fa[v]=fa[x]; siz[v]=siz[x];
        } else {
            int x,y; cin >> x >> y;
            fa[v]=fa[v-1]; siz[v]=siz[v-1];
            int fx=x; while(fa[v][fx]!=fx) fx=fa[v][fx];
            int fy=y; while(fa[v][fy]!=fy) fy=fa[v][fy];
            cout << (fx==fy ? 1 : 0) << "\n";
        }
    }
    return 0;
}
