#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
const int MAXN = 200001;
const int MAXM = MAXN * 100;
int cnt = 0;
int head[MAXN];
int key[MAXM], ls[MAXM], rs[MAXM], siz[MAXM];
bool rev[MAXM];
long long sum[MAXM];
double priority[MAXM];
int copy(int i) { key[++cnt]=key[i]; ls[cnt]=ls[i]; rs[cnt]=rs[i]; siz[cnt]=siz[i]; rev[cnt]=rev[i]; sum[cnt]=sum[i]; priority[cnt]=priority[i]; return cnt; }
void up(int i) { siz[i]=siz[ls[i]]+siz[rs[i]]+1; sum[i]=sum[ls[i]]+sum[rs[i]]+key[i]; }
void down(int i) {
    if (rev[i]) {
        if (ls[i]!=0){ ls[i]=copy(ls[i]); rev[ls[i]]^=1; }
        if (rs[i]!=0){ rs[i]=copy(rs[i]); rev[rs[i]]^=1; }
        swap(ls[i], rs[i]); rev[i]=false;
    }
}
void split(int l, int r, int i, int rank) {
    if (i == 0) { rs[l] = ls[r] = 0; }
    else {
        i = copy(i); down(i);
        if (siz[ls[i]] + 1 <= rank) { rs[l] = i; split(i, r, rs[i], rank - siz[ls[i]] - 1); }
        else { ls[r] = i; split(l, i, ls[i], rank); }
        up(i);
    }
}
int merge(int l, int r) {
    if (l == 0 || r == 0) return l + r;
    if (priority[l] >= priority[r]) { l = copy(l); down(l); rs[l] = merge(rs[l], r); up(l); return l; }
    else { r = copy(r); down(r); ls[r] = merge(l, ls[r]); up(r); return r; }
}
int main(int argc, char** argv) {
    long long seed = argc>1 ? atoll(argv[1]) : 20261018;
    srand(seed);
    int ROUNDS = 40, STEPS = 80;
    long long fails = 0, total = 0;
    for(int r=0;r<ROUNDS;r++){
        cnt = 0; head[0] = 0;
        memset(key,0,sizeof(key)); memset(ls,0,sizeof(ls));
        memset(rs,0,sizeof(rs)); memset(siz,0,sizeof(siz)); memset(rev,0,sizeof(rev));
        vector<vector<int>> snap(1); // 版本0空序列
        long long lastAns = 0;
        for(int i=1;i<=STEPS;i++){
            int version = rand()%i;
            int op = rand()%4+1;
            vector<int>& sm = snap[version];
            int sz = (int)sm.size();
            long long x, y = 0;
            if(op==1){ x = rand()%(sz+1); y = rand()%41-20; }
            else if(op==2){ if(sz==0){ op=1; x=rand()%1; y=rand()%41-20; } else { x = rand()%sz+1; } }
            else if(op==3){ if(sz==0){ op=1; x=rand()%1; y=rand()%41-20; } else { int a=rand()%sz+1, b=rand()%sz+1; if(a>b) swap(a,b); x=a; y=b; } }
            else { if(sz==0){ op=1; x=rand()%1; y=rand()%41-20; } else { int a=rand()%sz+1, b=rand()%sz+1; if(a>b) swap(a,b); x=a; y=b; } }
            // 加密输入（模拟题目：输入参数异或 lastAns）
            long long xenc = x ^ lastAns, yenc = y ^ lastAns;
            // 课程端读入解码
            long long xd = xenc ^ lastAns, yd = yenc ^ lastAns;
            vector<int> ns = sm;
            int l, m, lm2, r2;
            if(op==1){
                split(0,0,head[version], (int)xd); l=rs[0]; r2=ls[0]; ls[0]=rs[0]=0;
                key[++cnt]=(int)yd; siz[cnt]=1; sum[cnt]=yd; priority[cnt]=(double)rand()/RAND_MAX;
                head[i]=merge(merge(l,cnt),r2);
                ns.insert(ns.begin()+xd, (int)yd);
            }
            else if(op==2){
                split(0,0,head[version], (int)xd); lm2=rs[0]; r2=ls[0];
                split(0,0,lm2,(int)xd-1); l=rs[0]; m=ls[0]; ls[0]=rs[0]=0;
                head[i]=merge(l,r2);
                ns.erase(ns.begin()+xd-1);
            }
            else if(op==3){
                split(0,0,head[version], (int)yd); lm2=rs[0]; r2=ls[0];
                split(0,0,lm2,(int)xd-1); l=rs[0]; m=ls[0]; ls[0]=rs[0]=0;
                rev[m]^=1;
                head[i]=merge(merge(l,m),r2);
                reverse(ns.begin()+xd-1, ns.begin()+yd);
            }
            else {
                split(0,0,head[version], (int)yd); lm2=rs[0]; r2=ls[0];
                split(0,0,lm2,(int)xd-1); l=rs[0]; m=ls[0]; ls[0]=rs[0]=0;
                lastAns = sum[m];
                head[i]=merge(merge(l,m),r2);
                long long b=0; for(int k=xd-1;k<yd;k++) b+=ns[k];
                total++;
                if(lastAns!=b){ fails++; printf("r%d v%d op4 x=%d y=%d course=%lld brute=%lld\n",r,i,(int)xd,(int)yd,lastAns,b); if(fails>5) return 1; }
            }
            snap.push_back(ns);
        }
        if(fails>5) return 1;
    }
    printf("persistentliterary(P5055): total=%lld fails=%lld\n", total, fails);
    return fails?1:0;
}
