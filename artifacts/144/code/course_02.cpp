#include <bits/stdc++.h>
using namespace std;
const int MOD = 10007;
const int MAXK = 1001;
long long fac[MAXK+1], inv[MAXK+1];
long long power(long long x, int p) { long long ans=1; while(p>0){ if(p&1) ans=ans*x%MOD; x=x*x%MOD; p>>=1;} return ans; }
void build() {
  fac[0] = 1;  // 0! = 1
  fac[1] = 1;
  for(int i=2;i<=MAXK;i++) fac[i] = (long long)i*fac[i-1]%MOD;
  inv[MAXK]=power(fac[MAXK], MOD-2);
  for(int i=MAXK-1;i>=0;i--) inv[i] = (long long)(i+1)*inv[i+1]%MOD;
}
long long c(int n, int k){ return fac[n]*inv[k]%MOD*inv[n-k]%MOD; }
int main(){
  build();
  int a,b,k,n,m; cin>>a>>b>>k>>n>>m;
  cout << power(a,n)*power(b,m)%MOD*c(k,k-n)%MOD << "\n";
}
