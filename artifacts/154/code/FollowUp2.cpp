#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1000001;
int n, m;

int num[MAXN];
int up[MAXN];
int ls[MAXN];
int rs[MAXN];
int dist[MAXN];
int father[MAXN];

int root[MAXN];

void prepare() {
    dist[0] = -1;
    for (int i = 1; i <= n; i++) {
        up[i] = ls[i] = rs[i] = dist[i] = 0;
        father[i] = i;
        root[i] = i;
    }
}

int find(int i) {
    father[i] = father[i] == i ? i : find(father[i]);
    return father[i];
}

int merge(int i, int j) {
    if (i == 0 || j == 0) {
        return i + j;
    }
    if (num[i] > num[j] || (num[i] == num[j] && i > j)) {
        swap(i, j);
    }
    rs[i] = merge(rs[i], j);
    up[rs[i]] = i;
    if (dist[ls[i]] < dist[rs[i]]) {
        swap(ls[i], rs[i]);
    }
    dist[i] = dist[rs[i]] + 1;
    father[ls[i]] = father[rs[i]] = i;
    return i;
}

int remove(int i) {
    int h = find(i);
    father[ls[i]] = ls[i];
    father[rs[i]] = rs[i];
    int s = merge(ls[i], rs[i]);
    int f = up[i];
    father[i] = s;
    up[s] = f;
    if (h != i) {
        father[s] = h;
        if (ls[f] == i) {
            ls[f] = s;
        } else {
            rs[f] = s;
        }
        for (int d = dist[s]; dist[f] > d + 1; f = up[f], d++) {
            dist[f] = d + 1;
            if (dist[ls[f]] < dist[rs[f]]) {
                swap(ls[f], rs[f]);
            }
        }
    }
    up[i] = ls[i] = rs[i] = dist[i] = 0;
    return father[s];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    prepare();
    for (int i = 1; i <= n; i++) {
        cin >> num[i];
    }
    for (int i = 1, op, x, y, z; i <= m; i++) {
        cin >> op;
        if (op == 0) {
            cin >> x >> y;
            root[x] = remove(y);
            if (root[x] != 0) {
                father[root[x]] = root[x];
                up[root[x]] = 0;
            }
        } else if (op == 1) {
            cin >> x;
            cout << num[root[x]] << "\n";
        } else if (op == 2) {
            cin >> x >> y;
            root[x] = merge(root[x], root[y]);
            if (root[x] != 0) {
                father[root[x]] = root[x];
                up[root[x]] = 0;
            }
        } else {
            cin >> x >> y >> z;
            int h = remove(y);
            num[y] = z;
            father[y] = y;
            root[x] = merge(h, y);
            if (root[x] != 0) {
                father[root[x]] = root[x];
                up[root[x]] = 0;
            }
        }
    }
    return 0;
}
