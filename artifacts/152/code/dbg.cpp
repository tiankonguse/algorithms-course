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

void up(int i) { siz[i] = siz[ls[i]] + siz[rs[i]] + 1; }
void split(int l, int r, int i, int num, int dep) {
    if (dep > 50) { printf("SPLIT LOOP l=%d r=%d i=%d num=%d\n", l, r, i, num); exit(1); }
    if (i == 0) { rs[l] = ls[r] = 0; }
    else {
        if (key[i] <= num) { rs[l] = i; split(i, r, rs[i], num, dep+1); }
        else { ls[r] = i; split(l, i, ls[i], num, dep+1); }
        up(i);
    }
}
int merge(int l, int r, int dep) {
    if (dep > 50) { printf("MERGE LOOP l=%d r=%d\n", l, r); exit(1); }
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
    split(0, 0, head, num, 0);
    int lm = rs[0], r = ls[0];
    split(0, 0, lm, num - 1, 0);
    int l = rs[0], m = ls[0];
    head = merge(merge(l, merge(ls[m], rs[m], 0), 0), r, 0);
}
int getRank(int num) {
    split(0, 0, head, num - 1, 0);
    int ans = siz[rs[0]] + 1;
    head = merge(rs[0], ls[0], 0);
    return ans;
}
// 环检测：DFS 从 head 出发，访问过的节点再访问即环
void check() {
    static int vis[MAXN], stamp = 0;
    stamp++;
    int st[100000], top = 0; st[top++] = head;
    while (top > 0) {
        int i = st[--top];
        if (i == 0) continue;
        if (vis[i] == stamp) { printf("CYCLE at %d\n", i); exit(1); }
        vis[i] = stamp;
        st[top++] = ls[i]; st[top++] = rs[i];
    }
}
int main(int argc, char** argv) {
    srand(atoi(argv[1]));
    int n = atoi(argv[2]);
    for (int s = 0; s < n; s++) {
        int op = rand() % 5 + 1;
        int x = rand() % 9 + 1;
        if (op == 1) { printf("add %d\n", x); add(x); }
        else if (op == 2) { printf("remove %d\n", x); remove(x); }
        else if (op == 3) { printf("rank %d -> %d\n", x, getRank(x)); }
        else if (op == 4) { printf("idx %d\n", x); }
        else { printf("pre %d\n", x); }
        check();
    }
    printf("done\n");
    return 0;
}
