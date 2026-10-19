#include <bits/stdc++.h>

using namespace std;

struct Query {
    int l, r, id;
};

const int MAXN = 100002;
const int MAXV = 1 << 14;
int n, m, k;
int arr[MAXN];
int bi[MAXN];
int kOneArr[MAXV];
int cntk;

Query query[MAXN];

int headl[MAXN];
int headr[MAXN];
int nextq[MAXN << 1];
int ql[MAXN << 1];
int qr[MAXN << 1];
int qop[MAXN << 1];
int qid[MAXN << 1];
int cntq;

int cnt[MAXV];
long long pre[MAXN];
long long suf[MAXN];

long long ans[MAXN];

bool QueryCmp(Query &a, Query &b) {
    if (bi[a.l] != bi[b.l]) {
        return bi[a.l] < bi[b.l];
    }
    return a.r < b.r;
}

int lowbit(int x) {
    return x & -x;
}

int countOne(int num) {
    int ret = 0;
    while (num > 0) {
        ret++;
        num -= lowbit(num);
    }
    return ret;
}

void addLeftOffline(int x, int l, int r, int op, int id) {
    nextq[++cntq] = headl[x];
    headl[x] = cntq;
    ql[cntq] = l;
    qr[cntq] = r;
    qop[cntq] = op;
    qid[cntq] = id;
}

void addRightOffline(int x, int l, int r, int op, int id) {
    nextq[++cntq] = headr[x];
    headr[x] = cntq;
    ql[cntq] = l;
    qr[cntq] = r;
    qop[cntq] = op;
    qid[cntq] = id;
}

void prepare() {
    int blen = (int)sqrt(n);
    for (int i = 1; i <= n; i++) {
        bi[i] = (i - 1) / blen + 1;
    }
    sort(query + 1, query + m + 1, QueryCmp);
    for (int v = 0; v < MAXV; v++) {
        if (countOne(v) == k) {
            kOneArr[++cntk] = v;
        }
    }
}

void compute() {
    for (int i = 1; i <= n; i++) {
        pre[i] = pre[i - 1] + cnt[arr[i]];
        for (int j = 1; j <= cntk; j++) {
            cnt[arr[i] ^ kOneArr[j]]++;
        }
    }
    memset(cnt, 0, sizeof(cnt));
    for (int i = n; i >= 1; i--) {
        suf[i] = suf[i + 1] + cnt[arr[i]];
        for (int j = 1; j <= cntk; j++) {
            cnt[arr[i] ^ kOneArr[j]]++;
        }
    }
    int winl = 1, winr = 0;
    for (int i = 1; i <= m; i++) {
        int jobl = query[i].l;
        int jobr = query[i].r;
        int id = query[i].id;
        if (winr < jobr) {
            addLeftOffline(winl - 1, winr + 1, jobr, -1, id);
            ans[id] += pre[jobr] - pre[winr];
        }
        if (winr > jobr) {
            addLeftOffline(winl - 1, jobr + 1, winr, 1, id);
            ans[id] -= pre[winr] - pre[jobr];
        }
        winr = jobr;
        if (winl > jobl) {
            addRightOffline(winr + 1, jobl, winl - 1, -1, id);
            ans[id] += suf[jobl] - suf[winl];
        }
        if (winl < jobl) {
            addRightOffline(winr + 1, winl, jobl - 1, 1, id);
            ans[id] -= suf[winl] - suf[jobl];
        }
        winl = jobl;
    }
    memset(cnt, 0, sizeof(cnt));
    for (int x = 0; x <= n; x++) {
        if (x >= 1) {
            for (int j = 1; j <= cntk; j++) {
                cnt[arr[x] ^ kOneArr[j]]++;
            }
        }
        for (int q = headl[x]; q > 0; q = nextq[q]) {
            int l = ql[q], r = qr[q], op = qop[q], id = qid[q];
            for (int j = l; j <= r; j++) {
                ans[id] += 1LL * op * cnt[arr[j]];
            }
        }
    }
    memset(cnt, 0, sizeof(cnt));
    for (int x = n + 1; x >= 1; x--) {
        if (x <= n) {
            for (int j = 1; j <= cntk; j++) {
                cnt[arr[x] ^ kOneArr[j]]++;
            }
        }
        for (int q = headr[x]; q > 0; q = nextq[q]) {
            int l = ql[q], r = qr[q], op = qop[q], id = qid[q];
            for (int j = l; j <= r; j++) {
                ans[id] += 1LL * op * cnt[arr[j]];
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m >> k;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    for (int i = 1; i <= m; i++) {
        cin >> query[i].l >> query[i].r;
        query[i].id = i;
    }
    prepare();
    compute();
    for (int i = 2; i <= m; i++) {
        ans[query[i].id] += ans[query[i - 1].id];
    }
    for (int i = 1; i <= m; i++) {
        cout << ans[i] << '\n';
    }
    return 0;
}
