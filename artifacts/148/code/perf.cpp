// 148 性能验证：AVL 有序表 1e5 次操作
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 100001;
int cnt = 0, head = 0;
int key[MAXN], height[MAXN], ls[MAXN], rs[MAXN], key_count[MAXN], siz[MAXN];
void up(int i) { siz[i] = siz[ls[i]] + siz[rs[i]] + key_count[i]; height[i] = max(height[ls[i]], height[rs[i]]) + 1; }
int leftRotate(int i) { int r = rs[i]; rs[i] = ls[r]; ls[r] = i; up(i); up(r); return r; }
int rightRotate(int i) { int l = ls[i]; ls[i] = rs[l]; rs[l] = i; up(i); up(l); return l; }
int maintain(int i) {
    int lh = height[ls[i]], rh = height[rs[i]];
    if (lh - rh > 1) {
        if (height[ls[ls[i]]] >= height[rs[ls[i]]]) i = rightRotate(i);
        else { ls[i] = leftRotate(ls[i]); i = rightRotate(i); }
    } else if (rh - lh > 1) {
        if (height[rs[rs[i]]] >= height[ls[rs[i]]]) i = leftRotate(i);
        else { rs[i] = rightRotate(rs[i]); i = leftRotate(i); }
    }
    return i;
}
int add(int i, int num) {
    if (i == 0) { key[++cnt] = num; key_count[cnt] = siz[cnt] = height[cnt] = 1; return cnt; }
    if (key[i] == num) key_count[i]++;
    else if (key[i] > num) ls[i] = add(ls[i], num);
    else rs[i] = add(rs[i], num);
    up(i); return maintain(i);
}
void add(int num) { head = add(head, num); }
int getRank(int i, int num) {
    if (i == 0) return 0;
    if (key[i] >= num) return getRank(ls[i], num);
    return siz[ls[i]] + key_count[i] + getRank(rs[i], num);
}
int getRank(int num) { return getRank(head, num) + 1; }
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n = 100000;
    auto t0 = chrono::high_resolution_clock::now();
    for (int i = 1; i <= n; i++) {
        int x = (i * 1000003LL) % 20000001 - 10000000;
        add(x);
    }
    long long sum = 0;
    for (int i = 1; i <= n; i++) {
        int x = (i * 100003LL) % 20000001 - 10000000;
        sum += getRank(x);
    }
    auto t1 = chrono::high_resolution_clock::now();
    double sec = chrono::duration<double>(t1 - t0).count();
    printf("n=%d 插入+排名共 %d 次操作，%.3f s，sum=%lld，树高=%d\n", n, 2 * n, sec, sum, height[head]);
    return 0;
}
