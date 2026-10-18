#include <bits/stdc++.h>
using namespace std;
struct Node { int v, x; };
int main(){
    int n; cin >> n;
    vector<Node> a(n+1);
    for(int i=1;i<=n;i++) cin >> a[i].v >> a[i].x;
    long long ans = 0;
    for(int i=1;i<=n;i++) for(int j=i+1;j<=n;j++)
        ans += 1LL * max(a[i].v, a[j].v) * abs(a[i].x - a[j].x);
    cout << ans << "\n";
    return 0;
}
