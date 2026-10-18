#include <bits/stdc++.h>
using namespace std;
const int MAXN = 100001;
int cnt = 0, head = 0;
int key[MAXN], key_count[MAXN], ls[MAXN], rs[MAXN], siz[MAXN];
double priority[MAXN];
void up(int i) { siz[i] = siz[ls[i]] + siz[rs[i]] + key_count[i]; }
int leftRotate(int i) { int r = rs[i]; rs[i] = ls[r]; ls[r] = i; up(i); up(r); return r; }
int rightRotate(int i) { int l = ls[i]; ls[i] = rs[l]; rs[l] = i; up(i); up(l); return l; }
int add(int i, int num) {
    if (i == 0) { key[++cnt] = num; key_count[cnt] = siz[cnt] = 1; priority[cnt] = (double)rand() / RAND_MAX; return cnt; }
    if (key[i] == num) key_count[i]++;
    else if (key[i] > num) ls[i] = add(ls[i], num);
    else rs[i] = add(rs[i], num);
    up(i);
    if (ls[i] != 0 && priority[ls[i]] > priority[i]) return rightRotate(i);
    if (rs[i] != 0 && priority[rs[i]] > priority[i]) return leftRotate(i);
    return i;
}
void add(int num) { head = add(head, num); }
int small(int i, int num) {
    if (i == 0) return 0;
    if (key[i] >= num) return small(ls[i], num);
    else return siz[ls[i]] + key_count[i] + small(rs[i], num);
}
int getRank(int num) { return small(head, num) + 1; }
int main() {
    srand(2026);
    int N = 100000;
    auto t0 = chrono::steady_clock::now();
    for (int i = 0; i < N; i++) add(rand() % 100000000);
    for (int i = 0; i < N; i++) getRank(rand() % 100000000);
    auto t1 = chrono::steady_clock::now();
    printf("treap 20万操作(10万插+10万查): %.3fs 节点=%d\n", chrono::duration<double>(t1-t0).count(), cnt);
    // 树深
    int maxd = 0;
    for (int i = 1; i <= cnt; i++) {
        int cur = i, dep = 0;
        while (ls[cur] || rs[cur]) { cur = ls[cur] ? ls[cur] : rs[cur]; dep++; }
        maxd = max(maxd, dep);
    }
    printf("treap max depth: %d\n", maxd);
    return 0;
}
