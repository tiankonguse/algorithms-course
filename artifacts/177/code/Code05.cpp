#include <bits/stdc++.h>

using namespace std;

struct Query {
    int l, r, id;
};

const int MAXN = 200001;
const int MAXB = 501;
int n, m;
int arr[MAXN];
Query query[MAXN];

int blen, bnum;
int bi[MAXN];
int bl[MAXB];

int cnt[MAXN];
int mex;
int ans[MAXN];

bool QueryCmp(Query &a, Query &b) {
    if (bi[a.l] != bi[b.l]) {
        return bi[a.l] < bi[b.l];
    }
    return b.r < a.r;
}

void del(int num) {
    if (--cnt[num] == 0) {
        mex = min(mex, num);
    }
}

void add(int num) {
    cnt[num]++;
}

void compute() {
    for (int i = 1; i <= n; i++) {
        cnt[arr[i]]++;
    }
    mex = 0;
    while (cnt[mex] != 0) {
        mex++;
    }
    int winl = 1, winr = n;
    for (int block = 1, qi = 1; block <= bnum && qi <= m; block++) {
        while (winl < bl[block]) {
            del(arr[winl++]);
        }
        int beforeJob = mex;
        for (; qi <= m && bi[query[qi].l] == block; qi++) {
            int jobl = query[qi].l;
            int jobr = query[qi].r;
            int id = query[qi].id;
            while (winr > jobr) {
                del(arr[winr--]);
            }
            int backup = mex;
            while (winl < jobl) {
                del(arr[winl++]);
            }
            ans[id] = mex;
            mex = backup;
            while (winl > bl[block]) {
                add(arr[--winl]);
            }
        }
        while (winr < n) {
            add(arr[++winr]);
        }
        mex = beforeJob;
    }
}

void prepare() {
    blen = (int)sqrt(n);
    bnum = (n + blen - 1) / blen;
    for (int i = 1; i <= n; i++) {
        bi[i] = (i - 1) / blen + 1;
    }
    for (int i = 1; i <= bnum; i++) {
        bl[i] = (i - 1) * blen + 1;
    }
    sort(query + 1, query + m + 1, QueryCmp);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    for (int i = 1; i <= m; i++) {
        cin >> query[i].l >> query[i].r;
        query[i].id = i;
    }
    prepare();
    compute();
    for (int i = 1; i <= m; i++) {
        cout << ans[i] << '\n';
    }
    return 0;
}
