#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
const int MAXN = 500001;
const int MAXM = MAXN * 50;
int cnt = 0;
int head[MAXN];
int key[MAXM], ls[MAXM], rs[MAXM], siz[MAXM];
double priority[MAXM];
int copy(int i) { ++cnt; key[cnt]=key[i]; ls[cnt]=ls[i]; rs[cnt]=rs[i]; siz[cnt]=siz[i]; priority[cnt]=priority[i]; return cnt; }
void up(int i) { siz[i] = siz[ls[i]] + siz[rs[i]] + 1; }
void split(int l, int r, int i, int num) {
    if (i == 0) { rs[l] = ls[r] = 0; }
    else {
        i = copy(i);
        if (key[i] <= num) { rs[l] = i; split(i, r, rs[i], num); }
        else { ls[r] = i; split(l, i, ls[i], num); }
        up(i);
    }
}
int merge(int l, int r) {
    if (l == 0 || r == 0) return l + r;
    if (priority[l] >= priority[r]) { l = copy(l); rs[l] = merge(rs[l], r); up(l); return l; }
    else { r = copy(r); ls[r] = merge(l, ls[r]); up(r); return r; }
}
void add(int v, int i, int num) {
    split(0, 0, i, num);
    int l = rs[0], r = ls[0];
    ls[0] = rs[0] = 0;
    ++cnt; key[cnt] = num; siz[cnt] = 1; priority[cnt] = (double)rand() / RAND_MAX;
    head[v] = merge(merge(l, cnt), r);
}
void remove(int v, int i, int num) {
    split(0, 0, i, num);
    int lm = rs[0], r = ls[0];
    split(0, 0, lm, num - 1);
    int l = rs[0], m = ls[0];
    ls[0] = rs[0] = 0;
    head[v] = merge(merge(l, merge(ls[m], rs[m])), r);
}
int small(int i, int num) {
    if (i == 0) return 0;
    if (key[i] >= num) return small(ls[i], num);
    else return siz[ls[i]] + 1 + small(rs[i], num);
}
int index(int i, int x) {
    if (siz[ls[i]] >= x) return index(ls[i], x);
    else if (siz[ls[i]] + 1 < x) return index(rs[i], x - siz[ls[i]] - 1);
    else return key[i];
}
int pre(int i, int num) {
    if (i == 0) return INT_MIN + 1;
    if (key[i] >= num) return pre(ls[i], num);
    else return max(key[i], pre(rs[i], num));
}
int post(int i, int num) {
    if (i == 0) return INT_MAX;
    if (key[i] <= num) return post(rs[i], num);
    else return min(key[i], post(ls[i], num));
}
int main(int argc, char** argv) {
    long long seed = argc>1 ? atoll(argv[1]) : 20261018;
    srand(seed);
    int ROUNDS = 40, STEPS = 80;
    long long fails = 0, total = 0;
    for(int r=0;r<ROUNDS;r++){
        cnt = 0; head[0] = 0;
        memset(key,0,sizeof(key)); memset(ls,0,sizeof(ls));
        memset(rs,0,sizeof(rs)); memset(siz,0,sizeof(siz));
        vector<map<int,int>> snap(1); // 版本0空
        for(int i=1;i<=STEPS;i++){
            int version = rand()%i;
            int op = rand()%6+1;
            int x;
            map<int,int>& sm = snap[version];
            int sz = 0; for(auto& p:sm) sz += p.second;
            if(op==4){ if(sz==0){ op=1; x = rand()%41-20; } else x = rand()%sz+1; }
            else if(op==2){ if(sz==0){ op=1; x = rand()%41-20; } else { auto it=sm.begin(); int k=rand()%(int)sm.size(); while(k--) ++it; x = it->first; } }
            else x = rand()%41-20;
            // 暴力版本快照
            map<int,int> ns = sm;
            if(op==1) ns[x]++;
            else if(op==2){ if(ns[x]>0){ ns[x]--; if(ns[x]==0) ns.erase(x); } }
            // 课程端
            if(op==1) add(i, head[version], x);
            else if(op==2) remove(i, head[version], x);
            else {
                head[i] = head[version];
                if(op==3){ int a=small(head[i],x)+1, b=0; for(auto& p:sm){ if(p.first<x) b+=p.second; else break; } b+=1; total++; if(a!=b){ fails++; printf("r%d v%d op3 x=%d course=%d brute=%d\n",r,i,x,a,b); if(fails>5) return 1; } }
                else if(op==4){ int a=index(head[i],x), b=0; int k=x; for(auto& p:ns){ if(k<=p.second){ b=p.first; break; } k-=p.second; } total++; if(a!=b){ fails++; printf("r%d v%d op4 x=%d course=%d brute=%d\n",r,i,x,a,b); if(fails>5) return 1; } }
                else if(op==5){ int a=pre(head[i],x), b=INT_MIN+1; auto it=sm.lower_bound(x); if(it!=sm.begin()){ --it; b=it->first; } total++; if(a!=b){ fails++; printf("r%d v%d op5 x=%d course=%d brute=%d\n",r,i,x,a,b); if(fails>5) return 1; } }
                else { int a=post(head[i],x), b=INT_MAX; auto it=sm.upper_bound(x); if(it!=sm.end()) b=it->first; total++; if(a!=b){ fails++; printf("r%d v%d op6 x=%d course=%d brute=%d\n",r,i,x,a,b); if(fails>5) return 1; } }
            }
            snap.push_back(ns);
        }
        if(fails>5) return 1;
    }
    printf("persistent(P3835): total=%lld fails=%lld\n", total, fails);
    return fails?1:0;
}
