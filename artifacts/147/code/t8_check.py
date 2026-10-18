import math, subprocess
# C++ 输出 n=1..500 的全部值
prog = r'''
#include <bits/stdc++.h>
using namespace std;
int minpf[2000001], prime[2000001], cnt, counts5[2000001];
void euler(int n){memset(minpf,0,sizeof(minpf));cnt=0;for(int i=2;i<=n;i++){if(minpf[i]==0)prime[cnt++]=i;for(int j=0;j<cnt;j++){if((long long)i*prime[j]>n)break;minpf[i*prime[j]]=prime[j];if(i%prime[j]==0)break;}}}
struct Big{vector<int>d;Big(int v=1){while(v){d.push_back(v%10);v/=10;}}void mul(int x){long long c=0;for(int i=0;i<(int)d.size();i++){long long cur=(long long)d[i]*x+c;d[i]=cur%10;c=cur/10;}while(c){d.push_back(c%10);c/=10;}}string str(){string s;for(int i=(int)d.size()-1;i>=0;i--)s+=char('0'+d[i]);return s;}};
string ladder(int n){
    euler(2*n);
    for(int i=2;i<=n;i++)counts5[i]=-1;
    for(int i=n+2;i<=2*n;i++)counts5[i]=1;
    counts5[n+1]=0;
    for(int i=2*n;i>=2;i--)if(minpf[i]!=0){counts5[minpf[i]]+=counts5[i];counts5[i/minpf[i]]+=counts5[i];counts5[i]=0;}
    Big ans(1);
    for(int i=2;i<=2*n;i++)for(int j=0;j<counts5[i];j++)ans.mul(i);
    return ans.str();
}
int main(){for(int n=1;n<=500;n++)printf("%d %s\n",n,ladder(n).c_str());}
'''
open('t8_main.cpp','w').write(prog)
subprocess.run(['clang++','-O2','-std=c++11','-I/tmp/cpp147/inc','t8_main.cpp','-o','t8_main'],check=True)
out = subprocess.check_output(['./t8_main']).decode().strip().split('\n')
bad = 0
for line in out:
    n, s = line.split()
    n = int(n)
    want = math.comb(2*n, n)//(n+1)
    if str(want) != s:
        bad += 1
        print('MISMATCH n=%d' % n)
        if bad > 5: break
print('T8 全量 1..500 对比', 'ALL OK' if bad==0 else f'{bad} 处不一致', '（位数检查：C500 =', len(out[-1].split()[1]), '位）')
