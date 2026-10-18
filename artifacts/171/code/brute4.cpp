#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin >> n;
    vector<int> h(n+1), v(n+1);
    for(int i=1;i<=n;i++) cin >> h[i] >> v[i];
    vector<int> len1(n+1,1), len2(n+1,1);
    vector<double> cnt1(n+1,1.0), cnt2(n+1,1.0);
    for(int i=1;i<=n;i++){
        int best = 1; double ways = 1.0;
        for(int j=1;j<i;j++) if(h[j]>=h[i] && v[j]>=v[i]){
            int cur = len1[j] + 1;
            if(cur > best){ best = cur; ways = cnt1[j]; }
            else if(cur == best) ways += cnt1[j];
        }
        len1[i] = best; cnt1[i] = ways;
    }
    for(int i=n;i>=1;i--){
        int best = 1; double ways = 1.0;
        for(int j=i+1;j<=n;j++) if(h[j]<=h[i] && v[j]<=v[i]){
            int cur = len2[j] + 1;
            if(cur > best){ best = cur; ways = cnt2[j]; }
            else if(cur == best) ways += cnt2[j];
        }
        len2[i] = best; cnt2[i] = ways;
    }
    int len = 0; for(int i=1;i<=n;i++) len = max(len, len1[i]);
    double cnt = 0.0; for(int i=1;i<=n;i++) if(len1[i]==len) cnt += cnt1[i];
    printf("%d\n", len);
    for(int i=1;i<=n;i++){
        if(len1[i] + len2[i] - 1 < len) printf("0 ");
        else printf("%.5f ", cnt1[i] * cnt2[i] / cnt);
    }
    printf("\n");
    return 0;
}
