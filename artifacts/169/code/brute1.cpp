#include <bits/stdc++.h>
using namespace std;
struct J { int d, p, l; };
int main(){
    int n, m; cin >> n >> m;
    vector<J> j(n);
    for(auto &x : j) cin >> x.d >> x.p >> x.l;
    sort(j.begin(), j.end(), [](J a, J b){ return a.d > b.d; });
    for(int i=0;i<m;i++){
        long long money, least; cin >> money >> least;
        int ans = -1;
        for(int k=1; k<=n; k++){               // 找最小可行 k
            vector<J> sub(j.begin(), j.begin()+k);
            sort(sub.begin(), sub.end(), [](J a, J b){ return a.p < b.p; });
            long long need = least, cost = 0;
            for(auto &x : sub){
                long long take = min((long long)x.l, need);
                cost += take * x.p;
                need -= take;
                if(need == 0) break;
            }
            if(need == 0 && cost <= money){ ans = j[k-1].d; break; }
        }
        cout << ans << "\n";
    }
    return 0;
}
