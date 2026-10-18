#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cstring>
#include <map>
using namespace std;
const int MAXN = 100001;
int head = 0, cnt = 0;
int key[MAXN], key_count[MAXN], ls[MAXN], rs[MAXN], siz[MAXN];
double priority[MAXN];
void up(int i) { siz[i] = siz[ls[i]] + siz[rs[i]] + key_count[i]; }
void split(int l, int r, int i, int num) {
    if (i == 0) { rs[l] = ls[r] = 0; }
    else {
        if (key[i] <= num) { rs[l] = i; split(i, r, rs[i], num); }
        else { ls[r] = i; split(l, i, ls[i], num); }
        up(i);
    }
}
int merge(int l, int r) {
    if (l == 0 || r == 0) return l + r;
    if (priority[l] >= priority[r]) { rs[l] = merge(rs[l], r); up(l); return l; }
    else { ls[r] = merge(l, ls[r]); up(r); return r; }
}
int find(int i, int num) {
    if (i == 0) return 0;
    if (key[i] == num) return i;
    else if (key[i] > num) return find(ls[i], num);
    else return find(rs[i], num);
}
void changeCount(int i, int num, int change) {
    if (key[i] == num) key_count[i] += change;
    else if (key[i] > num) changeCount(ls[i], num, change);
    else changeCount(rs[i], num, change);
    up(i);
}
void add(int num) {
    if (find(head, num) != 0) changeCount(head, num, 1);
    else {
        split(0, 0, head, num);
        key[++cnt] = num; key_count[cnt] = siz[cnt] = 1;
        priority[cnt] = (double)rand() / RAND_MAX;
        head = merge(merge(rs[0], cnt), ls[0]);
    }
}
void remove(int num) {
    int i = find(head, num);
    if (i != 0) {
        if (key_count[i] > 1) changeCount(head, num, -1);
        else {
            split(0, 0, head, num);
            int lm = rs[0], r = ls[0];
            split(0, 0, lm, num - 1);
            int l = rs[0];
            head = merge(l, r);
        }
    }
}
int small(int i, int num) {
    if (i == 0) return 0;
    if (key[i] >= num) return small(ls[i], num);
    else return siz[ls[i]] + key_count[i] + small(rs[i], num);
}
int getRank(int num) { return small(head, num) + 1; }
int index(int i, int x) {
    if (siz[ls[i]] >= x) return index(ls[i], x);
    else if (siz[ls[i]] + key_count[i] < x) return index(rs[i], x - siz[ls[i]] - key_count[i]);
    return key[i];
}
int index(int x) { return index(head, x); }
int pre(int i, int num) {
    if (i == 0) return INT_MIN;
    if (key[i] >= num) return pre(ls[i], num);
    else return max(key[i], pre(rs[i], num));
}
int pre(int num) { return pre(head, num); }
int post(int i, int num) {
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
    int ROUNDS = 400, STEPS = 300;
    long long total = 0, fails = 0;
    for(int r=0;r<ROUNDS;r++){
        head=0; cnt=0; mp.clear(); msize=0;
        memset(key,0,sizeof(key)); memset(key_count,0,sizeof(key_count));
        memset(ls,0,sizeof(ls)); memset(rs,0,sizeof(rs)); memset(siz,0,sizeof(siz));
        for(int s=0;s<STEPS;s++){
            int op = rand()%6+1, x;
            if(op==4){ if(msize==0){ op=1; x=rand()%41-20; } else x = rand()%msize+1; }
            else if(op==2){ if (msize==0) { op=1; x=rand()%41-20; } else { auto it = mp.begin(); int k = rand()%mp.size(); while(k--) ++it; x = it->first; } }
            else { x = rand()%41-20; }
            total++;
            if(op==1){ add(x); madd(x); }
            else if(op==2){ remove(x); mdel(x); }
            else if(op==3){ int a=getRank(x), b=mrank(x); if(a!=b){ fails++; printf("r%d s%d op3 x=%d course=%d brute=%d\n",r,s,x,a,b); if(fails>5) return 1; } }
            else if(op==4){ int a=index(x), b=mindex(x); if(a!=b){ fails++; printf("r%d s%d op4 x=%d course=%d brute=%d\n",r,s,x,a,b); if(fails>5) return 1; } }
            else if(op==5){ int a=pre(x), b=mpre(x); if(a!=b){ fails++; printf("r%d s%d op5 x=%d course=%d brute=%d\n",r,s,x,a,b); if(fails>5) return 1; } }
            else { int a=post(x), b=mpost(x); if(a!=b){ fails++; printf("r%d s%d op6 x=%d course=%d brute=%d\n",r,s,x,a,b); if(fails>5) return 1; } }
        }
    }
    printf("withcount: total=%lld fails=%lld\n", total, fails);
    return fails?1:0;
}
