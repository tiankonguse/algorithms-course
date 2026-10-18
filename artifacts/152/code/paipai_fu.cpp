#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <map>
using namespace std;
const int MAXN = 100001;
int head = 0, cnt = 0;
int key[MAXN], ls[MAXN], rs[MAXN], siz[MAXN];
double priority[MAXN];
int DEPG = 0, DROUND = 0, DSTEP = 0;
void up(int i) { siz[i] = siz[ls[i]] + siz[rs[i]] + 1; }
void split(int l, int r, int i, int num, int dep=0) {
    if (dep > 500) { printf("SPLIT LOOP r%d s%d l=%d r=%d i=%d num=%d\n", DROUND, DSTEP, l, r, i, num); DEPG = 1; return; }
    if (i == 0) { rs[l] = ls[r] = 0; }
    else {
        if (key[i] <= num) { rs[l] = i; split(i, r, rs[i], num, dep+1); }
        else { ls[r] = i; split(l, i, ls[i], num, dep+1); }
        up(i);
    }
}
int merge(int l, int r, int dep=0) {
    if (dep > 500) { printf("MERGE LOOP r%d s%d l=%d r=%d\n", DROUND, DSTEP, l, r); DEPG = 1; return 0; }
    if (l == 0 || r == 0) return l + r;
    if (priority[l] >= priority[r]) { rs[l] = merge(rs[l], r, dep+1); up(l); return l; }
    else { ls[r] = merge(l, ls[r], dep+1); up(r); return r; }
}
void add(int num) {
    split(0, 0, head, num);
    key[++cnt] = num; siz[cnt] = 1;
    priority[cnt] = (double)rand() / RAND_MAX;
    head = merge(merge(rs[0], cnt), ls[0]);
}
void remove(int num) {
    split(0, 0, head, num);
    int lm = rs[0], r = ls[0];
    split(0, 0, lm, num - 1);
    int l = rs[0], m = ls[0];
    head = merge(merge(l, merge(ls[m], rs[m])), r);
}
int getRank(int num) {
    split(0, 0, head, num - 1);
    int ans = siz[rs[0]] + 1;
    head = merge(rs[0], ls[0]);
    return ans;
}
int index(int i, int x) {
    if (DEPG) return 0;
    if (siz[ls[i]] >= x) return index(ls[i], x);
    else if (siz[ls[i]] + 1 < x) return index(rs[i], x - siz[ls[i]] - 1);
    else return key[i];
}
int index(int x) { return index(head, x); }
int pre(int i, int num) {
    if (DEPG) return 0;
    if (i == 0) return INT_MIN;
    if (key[i] >= num) return pre(ls[i], num);
    else return max(key[i], pre(rs[i], num));
}
int pre(int num) { return pre(head, num); }
int post(int i, int num) {
    if (DEPG) return 0;
    if (i == 0) return INT_MAX;
    if (key[i] <= num) return post(rs[i], num);
    else return min(key[i], post(ls[i], num));
}
int post(int num) { return post(head, num); }
map<int,int> mp; int msize = 0;
void madd(int x){ mp[x]++; msize++; }
void mdel(int x){ if(mp[x]>0){ mp[x]--; if(mp[x]==0) mp.erase(x); msize--; } }
int mrank(int x){ int c=0; for(auto& p:mp){ if(p.first<x) c+=p.second; else break; } return c+1; }
int mindex(int k){ for(auto& p:mp){ if(k<=p.second) return p.first; k-=p.second; } return -1; }
int mpre(int x){ auto it=mp.lower_bound(x); if(it==mp.begin()) return INT_MIN; --it; return it->first; }
int mpost(int x){ auto it=mp.upper_bound(x); if(it==mp.end()) return INT_MAX; return it->first; }
int main(int argc, char** argv) {
    long long seed = argc>1 ? atoll(argv[1]) : 20261018;
    srand(seed);
    int ROUNDS = 300, STEPS = 300;
    long long total = 0, fails = 0;
    for(int r=0;r<ROUNDS && !DEPG;r++){
        head=0; cnt=0; mp.clear(); msize=0;
        memset(key,0,sizeof(key)); memset(ls,0,sizeof(ls));
        memset(rs,0,sizeof(rs)); memset(siz,0,sizeof(siz));
        int n = rand()%20;
        for (int j=0;j<n;j++){ int v = rand()%41-20; add(v); madd(v); }
        int lastAns = 0; long long ansC = 0, ansB = 0;
        for(int s=0;s<STEPS && !DEPG;s++){
            DROUND = r; DSTEP = s;
            int op = rand()%6+1;
            int v; // 真实值
            if(op==4){ if(msize==0){ op=1; v = rand()%41-20; } else v = rand()%msize+1; }
            else if(op==2){ if (msize==0) { op=1; v = rand()%41-20; } else { auto it = mp.begin(); int k = rand()%mp.size(); while(k--) ++it; v = it->first; } }
            else v = rand()%41-20;
            int x = v ^ lastAns;   // 加密输入（模拟题目输入）
            // 课程端：读入 x 后解码
            int xd = x ^ lastAns;
            total++;
            if(op==1){ add(xd); madd(xd); }
            else if(op==2){ remove(xd); mdel(xd); }
            else if(op==3){ int b=getRank(xd); if(b!=mrank(xd)){ fails++; printf("r%d s%d op3 v=%d course=%d brute=%d\n",r,s,xd,b,mrank(xd)); if(fails>5) return 1; } lastAns=b; ansC^=b; ansB^=b; }
            else if(op==4){ int b=index(xd); if(b!=mindex(xd)){ fails++; printf("r%d s%d op4 v=%d course=%d brute=%d\n",r,s,xd,b,mindex(xd)); if(fails>5) return 1; } lastAns=b; ansC^=b; ansB^=b; }
            else if(op==5){ int b=pre(xd); if(b!=mpre(xd)){ fails++; printf("r%d s%d op5 v=%d course=%d brute=%d\n",r,s,xd,b,mpre(xd)); if(fails>5) return 1; } lastAns=b; ansC^=b; ansB^=b; }
            else { int b=post(xd); if(b!=mpost(xd)){ fails++; printf("r%d s%d op6 v=%d course=%d brute=%d\n",r,s,xd,b,mpost(xd)); if(fails>5) return 1; } lastAns=b; ansC^=b; ansB^=b; }
            if (DEPG) { printf("TRIGGER r%d s%d\n", r, s); break; }
        }
        if (ansC != ansB) { fails++; printf("r%d ans mismatch %lld vs %lld\n", r, ansC, ansB); if(fails>5) return 1; }
        if(r%50==0) fprintf(stderr, "round %d done\n", r);
    }
    printf("followup(P6136): total=%lld fails=%lld\n", total, fails);
    return fails?1:0;
}
