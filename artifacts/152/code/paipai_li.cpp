#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
const int MAXN = 100001;
int head = 0, cnt = 0;
int key[MAXN], ls[MAXN], rs[MAXN], siz[MAXN]; double priority[MAXN];
bool rev[MAXN];
int ans[MAXN], ansi;
void up(int i) { siz[i] = siz[ls[i]] + siz[rs[i]] + 1; }
void down(int i) {
    if (rev[i]) { swap(ls[i], rs[i]); rev[ls[i]] ^= 1; rev[rs[i]] ^= 1; rev[i] = false; }
}
void split(int l, int r, int i, int rank) {
    if (i == 0) { rs[l] = ls[r] = 0; }
    else {
        down(i);
        if (siz[ls[i]] + 1 <= rank) { rs[l] = i; split(i, r, rs[i], rank - siz[ls[i]] - 1); }
        else { ls[r] = i; split(l, i, ls[i], rank); }
        up(i);
    }
}
int merge(int l, int r) {
    if (l == 0 || r == 0) return l + r;
    if (priority[l] >= priority[r]) { down(l); rs[l] = merge(rs[l], r); up(l); return l; }
    else { down(r); ls[r] = merge(l, ls[r]); up(r); return r; }
}
void inorder(int i) {
    if (i != 0) { down(i); inorder(ls[i]); ans[++ansi] = key[i]; inorder(rs[i]); }
}
int main(int argc, char** argv) {
    long long seed = argc>1 ? atoll(argv[1]) : 20261018;
    srand(seed);
    int ROUNDS = 200, STEPS = 60;
    long long fails = 0;
    for(int r=0;r<ROUNDS;r++){
        head=0; cnt=0;
        memset(key,0,sizeof(key)); memset(ls,0,sizeof(ls));
        memset(rs,0,sizeof(rs)); memset(siz,0,sizeof(siz)); memset(rev,0,sizeof(rev));
        int n = rand()%15+1;
        for (int i=1;i<=n;i++){ key[++cnt]=i; siz[cnt]=1; priority[cnt]=(double)rand()/RAND_MAX; head=merge(head,cnt); }
        vector<int> v(n);
        for (int i=0;i<n;i++) v[i]=i+1;
        for(int st=0;st<STEPS;st++){
            int x = rand()%n+1, y = rand()%n+1;
            if (x>y) swap(x,y);
            split(0,0,head,y); int lm=rs[0], r=ls[0];
            split(0,0,lm,x-1); int l=rs[0], m=ls[0];
            rev[m] ^= 1;
            head = merge(merge(l,m),r);
            reverse(v.begin()+x-1, v.begin()+y);
        }
        ansi=0; inorder(head);
        if (ansi != n) { printf("r%d size mismatch %d vs %d\n", r, ansi, n); fails++; break; }
        for (int i=1;i<=ansi;i++){
            if (ans[i] != v[i-1]) { printf("r%d pos %d course=%d brute=%d\n", r, i, ans[i], v[i-1]); fails++; break; }
        }
        if (fails>5) return 1;
    }
    printf("literary(P3391): fails=%lld\n", fails);
    return fails?1:0;
}
