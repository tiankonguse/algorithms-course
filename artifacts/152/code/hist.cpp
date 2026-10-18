#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <map>
using namespace std;
const int MAXN = 100001;
int head = 0, cnt = 0;
int key[MAXN], ls[MAXN], rs[MAXN], siz[MAXN];
double priority[MAXN];
int DEPG = 0, DSTEP = 0;
void up(int i) { siz[i] = siz[ls[i]] + siz[rs[i]] + 1; }
void split(int l, int r, int i, int num, int dep) {
    if (dep > 500) { printf("SPLIT LOOP l=%d r=%d i=%d num=%d (step %d)\n", l, r, i, num, DSTEP); DEPG = 1; return; }
    if (i == 0) { rs[l] = ls[r] = 0; }
    else {
        if (key[i] <= num) { rs[l] = i; split(i, r, rs[i], num, dep+1); }
        else { ls[r] = i; split(l, i, ls[i], num, dep+1); }
        up(i);
    }
}
int merge(int l, int r, int dep) {
    if (dep > 500) { printf("MERGE LOOP l=%d r=%d (step %d)\n", l, r, DSTEP); DEPG = 1; return 0; }
    if (l == 0 || r == 0) return l + r;
    if (priority[l] >= priority[r]) { rs[l] = merge(rs[l], r, dep+1); up(l); return l; }
    else { ls[r] = merge(l, ls[r], dep+1); up(r); return r; }
}
void add(int num) {
    split(0, 0, head, num, 0);
    key[++cnt] = num; siz[cnt] = 1;
    priority[cnt] = (double)rand() / RAND_MAX;
    head = merge(merge(rs[0], cnt, 0), ls[0], 0);
}
void remove(int num) {
    printf("   [remove %d] head=%d\n", num, head);
    split(0, 0, head, num, 0);
    printf("   after split1: rs0=%d ls0=%d\n", rs[0], ls[0]);
    int lm = rs[0], r = ls[0];
    split(0, 0, lm, num - 1, 0);
    printf("   after split2: rs0=%d ls0=%d  l=%d m=%d ls[m]=%d rs[m]=%d\n", rs[0], ls[0], rs[0], ls[0], ls[ls[0]], rs[ls[0]]);
    int l = rs[0], m = ls[0];
    printf("   m=%d ls[m]=%d rs[m]=%d\n", m, ls[m], rs[m]);
    head = merge(merge(l, merge(ls[m], rs[m], 0), 0), r, 0);
    printf("   after merge: head=%d rs[2]=%d\n", head, rs[2]);
}
int getRank(int num) {
    split(0, 0, head, num - 1, 0);
    int ans = siz[rs[0]] + 1;
    head = merge(rs[0], ls[0], 0);
    return ans;
}
void dumpTree() {
    // 先序打印 每条边 (父->子)
    printf("   tree: head=%d", head);
    for (int i = 1; i <= cnt; i++) {
        if (ls[i]) printf(" %d->L%d", i, ls[i]);
        if (rs[i]) printf(" %d->R%d", i, rs[i]);
    }
    printf("  keys:");
    for (int i = 1; i <= cnt; i++) printf(" %d:%d", i, key[i]);
    printf("\n");
}
int main(int argc, char** argv) {
    long long seed = argc>1 ? atoll(argv[1]) : 20261018;
    srand(seed);
    for (int s = 0; s < 30 && !DEPG; s++) {
        int op = rand()%6+1, x;
        if(op==4){ op=1; x=rand()%41-20; }
        else if(op==2){ x = rand()%41-20; }
        else { x = rand()%41-20; }
        DSTEP = s;
        printf("s%d: op%d x=%d", s, op, x);
        if(op==1){ add(x); }
        else if(op==2){ remove(x); }
        else { int a=getRank(x); printf(" ->rank%d", a); }
        printf("\n");
        dumpTree();
        if (DEPG) break;
    }
    return 0;
}
