#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
int n, m;
int num[MAXN];
int ls[MAXN];
int rs[MAXN];
int dist[MAXN];
int fa[MAXN];

void prepare() {
    dist[0] = -1;
    for(int i = 1; i <= n; i++) {
        ls[i] = rs[i] = dist[i] = 0;
        fa[i] = i;
    }
}

int find(int i) {
    fa[i] = fa[i] == i ? i : find(fa[i]);
    return fa[i];
}

int merge(int i, int j) {
    if (i == 0 || j == 0) {
        return i + j;
    }
    if (num[i] < num[j]) {
        swap(i, j);
    }
    rs[i] = merge(rs[i], j);
    if (dist[ls[i]] < dist[rs[i]]) {
        swap(ls[i], rs[i]);
    }
    dist[i] = dist[rs[i]] + 1;
    fa[ls[i]] = fa[rs[i]] = i;
    return i;
}

int pop(int i) {
    fa[ls[i]] = ls[i];
    fa[rs[i]] = rs[i];
    fa[i] = merge(ls[i], rs[i]);
    ls[i] = rs[i] = dist[i] = 0;
    return fa[i];
}

int fight(int x, int y) {
    int a = find(x);
    int b = find(y);
    if (a == b) {
        return -1;
    }
    int l = pop(a);
    int r = pop(b);
    num[a] /= 2;
    num[b] /= 2;
    fa[a] = fa[b] = fa[l] = fa[r] = merge(merge(l, a), merge(r, b));
    return num[fa[a]];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    while (cin >> n) {
        prepare();
        for (int i = 1; i <= n; i++) {
            cin >> num[i];
        }
        cin >> m;
        for (int i = 1; i <= m; i++) {
            int x, y;
            cin >> x >> y;
            cout << fight(x, y) << endl;
        }
    }
    return 0;
}
