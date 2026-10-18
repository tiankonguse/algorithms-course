#include <bits/stdc++.h>
using namespace std;
int n, m;
int main(){
    cin >> n >> m;
    set<pair<int,int>> active;
    int fa[10];
    auto find = [&](int x){ while(fa[x]!=x) x=fa[x]; return x; };
    for(int i=1; i<=m; i++){
        int t, x, y; cin >> t >> x >> y;
        if(t==0) active.insert({min(x,y), max(x,y)});
        else if(t==1) active.erase({min(x,y), max(x,y)});
        else {
            for(int j=1;j<=n;j++) fa[j]=j;
            for(auto e : active){
                int fx=find(e.first), fy=find(e.second);
                if(fx!=fy) fa[fx]=fy;
            }
            cout << (find(x)==find(y) ? "Y" : "N") << "\n";
        }
    }
    return 0;
}
