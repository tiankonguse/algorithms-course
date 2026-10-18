#include <bits/stdc++.h>
using namespace std;
int main(){
    int n, m; cin >> n >> m;
    vector<int> a(n+1);
    for(int i=1;i<=n;i++) cin >> a[i];
    for(int i=0;i<m;i++){
        char t; cin >> t;
        if(t == 'C'){ int x, y; cin >> x >> y; a[x] = y; }
        else { int x, y, v; cin >> x >> y >> v; vector<int> s(a.begin()+x, a.begin()+y+1); sort(s.begin(), s.end()); cout << s[v-1] << "\n"; }
    }
    return 0;
}
