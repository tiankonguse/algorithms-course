#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1000001;
const int MAXM = 500001;
const int MAXV = 100002;
int n, m;
int blen, bnum;

int arr[MAXN];
int op[MAXM];
int ql[MAXM];
int qr[MAXM];
int qx[MAXM];

int maxv, lazy;
int fa[MAXV];
int pre0[MAXN];
int cntv[MAXV];

int ans[MAXM];

int find(int x) {
    if (x != fa[x]) {
        fa[x] = find(fa[x]);
    }
    return fa[x];
}

void change(int x, int y) {
    fa[x] = y;
}

void down(int l, int r) {
    for (int i = l; i <= r; i++) {
        arr[i] = find(arr[i]);
    }
}

void update(int qi, int l, int r) {
    int jobl = ql[qi], jobr = qr[qi], jobx = qx[qi];
    if (jobx >= maxv - lazy || jobl > r || jobr < l) {
        return;
    }
    if (jobl <= l && r <= jobr) {
        if ((jobx << 1) <= maxv - lazy) {
            for (int v = lazy + 1; v <= lazy + jobx; v++) {
                cntv[v + jobx] += cntv[v];
                cntv[v] = 0;
                change(v, v + jobx);
            }
            lazy += jobx;
        } else {
            for (int v = lazy + jobx + 1; v <= maxv; v++) {
                cntv[v - jobx] += cntv[v];
                cntv[v] = 0;
                change(v, v - jobx);
            }
            for (int v = maxv; v >= 0; v--) {
                if (cntv[v] != 0) {
                    maxv = v;
                    break;
                }
            }
        }
    } else {
        down(l, r);
        for (int i = max(l, jobl); i <= min(r, jobr); i++) {
            if (arr[i] - lazy > jobx) {
                cntv[arr[i]]--;
                arr[i] -= jobx;
                cntv[arr[i]]++;
            }
        }
        for (int v = maxv; v >= 0; v--) {
            if (cntv[v] != 0) {
                maxv = v;
                break;
            }
        }
    }
}

void query(int qi, int l, int r) {
    int jobl = ql[qi], jobr = qr[qi], jobx = qx[qi];
    if (jobx == 0 || jobx > maxv - lazy || jobl > r || jobr < l) {
        return;
    }
    if (jobl <= l && r <= jobr) {
        ans[qi] += cntv[jobx + lazy];
    } else {
        down(l, r);
        for (int i = max(l, jobl); i <= min(r, jobr); i++) {
            if (arr[i] - lazy == jobx) {
                ans[qi]++;
            }
        }
    }
}

void compute(int l, int r) {
    memset(cntv, 0, sizeof(int) * MAXV);
    maxv = lazy = 0;
    for (int i = l; i <= r; i++) {
        maxv = max(maxv, arr[i]);
        cntv[arr[i]]++;
    }
    for (int v = 0; v <= maxv; v++) {
        fa[v] = v;
    }
    for (int i = 1; i <= m; i++) {
        if (op[i] == 1) {
            update(i, l, r);
        } else {
            query(i, l, r);
        }
    }
}

void prepare() {
    blen = (int)sqrt(n);
    bnum = (n + blen - 1) / blen;
    for (int i = 1; i <= n; i++) {
        pre0[i] = pre0[i - 1] + (arr[i] == 0 ? 1 : 0);
    }
    for (int i = 1; i <= m; i++) {
        if (op[i] == 2 && qx[i] == 0) {
            ans[i] = pre0[qr[i]] - pre0[ql[i] - 1];
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    for (int i = 1; i <= m; i++) {
        cin >> op[i] >> ql[i] >> qr[i] >> qx[i];
    }
    prepare();
    for (int i = 1, l, r; i <= bnum; i++) {
        l = (i - 1) * blen + 1;
        r = min(i * blen, n);
        compute(l, r);
    }
    for (int i = 1; i <= m; i++) {
        if (op[i] == 2) {
            cout << ans[i] << '\n';
        }
    }
    return 0;
}
