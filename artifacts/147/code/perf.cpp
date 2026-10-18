#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int minpf[2000001], prime[2000001], cnt, counts5[2000001];
void euler(int n){memset(minpf,0,sizeof(minpf));cnt=0;for(int i=2;i<=n;i++){if(minpf[i]==0)prime[cnt++]=i;for(int j=0;j<cnt;j++){if((ll)i*prime[j]>n)break;minpf[i*prime[j]]=prime[j];if(i%prime[j]==0)break;}}}
ll power5(ll x,ll p,int mod){ll ans=1;while(p){if(p&1)ans=ans*x%mod;x=x*x%mod;p>>=1;}return ans;}
int funny(int n,int mod){euler(2*n);for(int i=2;i<=n;i++)counts5[i]=-1;for(int i=n+2;i<=2*n;i++)counts5[i]=1;counts5[n+1]=0;for(int i=2*n;i>=2;i--)if(minpf[i]!=0){counts5[minpf[i]]+=counts5[i];counts5[i/minpf[i]]+=counts5[i];counts5[i]=0;}ll ans=1;for(int i=2;i<=2*n;i++)if(counts5[i]!=0)ans=ans*power5(i,counts5[i],mod)%mod;return (int)ans;}
const int MOD6=20100403;
ll fac6[2000001],inv6[2000001];
ll power6(ll x,ll p){ll ans=1;while(p){if(p&1)ans=ans*x%MOD6;x=x*x%MOD6;p>>=1;}return ans;}
void build6(int n){fac6[0]=inv6[0]=1;fac6[1]=1;for(int i=2;i<=n;i++)fac6[i]=(ll)i*fac6[i-1]%MOD6;inv6[n]=power6(fac6[n],MOD6-2);for(int i=n-1;i>=1;i--)inv6[i]=(ll)(i+1)*inv6[i+1]%MOD6;}
ll c6(int n,int k){return (((fac6[n]*inv6[k])%MOD6)*inv6[n-k])%MOD6;}
ll genstr(int n,int m){build6(n+m);return (c6(n+m,m)-c6(n+m,m-1)+MOD6)%MOD6;}
int main(){
  clock_t s=clock();
  int a=funny(1000000,1000000007);
  printf("T5 n=1e6 p=1e9+7 -> %d (%.2fs)\n",a,(double)(clock()-s)/CLOCKS_PER_SEC);
  s=clock();
  ll b=genstr(1000000,1000000);
  printf("T6 n=m=1e6 -> %lld (%.2fs)\n",b,(double)(clock()-s)/CLOCKS_PER_SEC);
  s=clock();
  int c=funny(1000000,20100403); // 非质数 mod
  printf("T5 n=1e6 p=20100403(合数? 20100403=?) -> %d (%.2fs)\n",c,(double)(clock()-s)/CLOCKS_PER_SEC);
  return 0;
}
