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
    if (num[i] > num[j] || (num[i] == num[j] && i > j)) {
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    prepare();
    for (int i = 1; i <= n; i++) {
        cin >> num[i];
    }
    for (int i = 1; i <= m; i++) {
        int op;
        cin >> op;
        if (op == 1) {
            int x, y;
            cin >> x >> y;
            if (num[x] != -1 && num[y] != -1) {
                int l = find(x);
                int r = find(y);
                if (l != r) {
                    merge(l, r);
                }
            }
        } else {
            int x;
            cin >> x;
            if (num[x] == -1) {
                cout << -1 << "\n";
            } else {
                int ans = find(x);
                cout << num[ans] << "\n";
                pop(ans);
                num[ans] = -1;
            }
        }
    }
    return 0;
}
