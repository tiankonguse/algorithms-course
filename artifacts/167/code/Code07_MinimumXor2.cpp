#include <bits/stdc++.h>

using namespace std;

const int MAXN = 10000001;
const int BIT = 29;
const int INF = 1 << 30;

int fa[MAXN];
int tree[MAXN][2];
int pass[MAXN];
int cnt = 1;

int mineor[MAXN];
int only[MAXN];

int change(int x, int changeCnt) {
    int cur = 1;
    pass[cur] += changeCnt;
    for (int b = BIT, path; b >= 0; b--) {
        path = (x >> b) & 1;
        if (tree[cur][path] == 0) {
            tree[cur][path] = ++cnt;
            fa[tree[cur][path]] = cur;
        }
        cur = tree[cur][path];
        pass[cur] += changeCnt;
    }
    return cur;
}

void compute(int x, int changeCnt) {
    int bottom = change(x, changeCnt);
    mineor[bottom] = pass[bottom] >= 2 ? 0 : INF;
    only[bottom] = pass[bottom] == 1 ? x : 0;
    for (int i = fa[bottom], l, r; i > 0; i = fa[i]) {
        l = tree[i][0];
        r = tree[i][1];
        if (pass[i] < 2) {
            mineor[i] = INF;
        } else if (pass[l] == 1 && pass[r] == 1) {
            mineor[i] = only[l] ^ only[r];
        } else if (pass[l] == 0 ^ pass[r] == 0) {
            mineor[i] = pass[l] == 0 ? mineor[r] : mineor[l];
        } else {
            mineor[i] = min(mineor[l], mineor[r]);
        }
        if (pass[l] + pass[r] == 1) {
        	only[i] = pass[l] == 1 ? only[l] : only[r];
        } else {
        	only[i] = 0;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int q;
    cin >> q;
    for (int i = 1, op, x; i <= q; i++) {
        cin >> op;
        if (op == 3) {
            cout << mineor[1] << '\n';
        } else {
            cin >> x;
            if (op == 1) {
                compute(x, 1);
            } else {
                compute(x, -1);
            }
        }
    }
    return 0;
}
