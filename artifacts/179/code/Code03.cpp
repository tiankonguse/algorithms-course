#include <bits/stdc++.h>

using namespace std;

struct Query {
    int l, r, id;
};

const int MAXN = 3000001;
const int MAXM = 100001;
int n, m, k;
int len, cntq, cntv;
char str[MAXN];
Query query[MAXM];

int base = 499;
long long basePower[MAXN];
long long hashValue[MAXN];

long long val[MAXN];
long long sorted[MAXN];
int arr[MAXN];
int bi[MAXN];

int cnt[MAXN];
long long curAns;
long long ans[MAXM];

int kth(long long num) {
    int left = 1, right = cntv, ret = 0;
    while (left <= right) {
        int mid = (left + right) >> 1;
        if (sorted[mid] <= num) {
            ret = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return ret;
}

bool QueryCmp(Query &a, Query &b) {
    if (bi[a.l] != bi[b.l]) {
        return bi[a.l] < bi[b.l];
    }
    if (bi[a.l] & 1) {
        return a.r < b.r;
    } else {
        return a.r > b.r;
    }
}

void add(int x) {
    curAns += cnt[x];
    cnt[x]++;
}

void del(int x) {
    cnt[x]--;
    curAns -= cnt[x];
}

void compute() {
    int winl = 1, winr = 0;
    for (int i = 1; i <= cntq; i++) {
        int jobl = query[i].l;
        int jobr = query[i].r;
        int id   = query[i].id;
        while (winl > jobl) {
            add(arr[--winl]);
        }
        while (winr < jobr) {
            add(arr[++winr]);
        }
        while (winl < jobl) {
            del(arr[winl++]);
        }
        while (winr > jobr) {
            del(arr[winr--]);
        }
        ans[id] = curAns;
    }
}

void prepare() {
    basePower[0] = 1;
    for (int i = 1; i <= n; i++) {
        basePower[i] = basePower[i - 1] * base;
        hashValue[i] = hashValue[i - 1] * base + (str[i] - 'a' + 1);
    }
    for (int l = 1, r = k; r <= n; l++, r++) {
        val[l] = hashValue[r] - hashValue[l - 1] * basePower[r - l + 1];
    }
    for (int i = 1; i <= len; i++) {
        sorted[i] = val[i];
    }
    sort(sorted + 1, sorted + len + 1);
    cntv = 1;
    for (int i = 2; i <= len; i++) {
        if (sorted[cntv] != sorted[i]) {
            sorted[++cntv] = sorted[i];
        }
    }
    for (int i = 1; i <= len; i++) {
        arr[i] = kth(val[i]);
    }
    int blen = max(1, (int)((double)n / sqrt((double)m)));
    for (int i = 1; i <= n; i++) {
        bi[i] = (i - 1) / blen + 1;
    }
    sort(query + 1, query + cntq + 1, QueryCmp);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m >> k;
    for (int i = 1; i <= n; i++) {
        cin >> str[i];
    }
    len = n - k + 1;
    cntq = 0;
    for (int i = 1, l, r; i <= m; i++) {
        cin >> l >> r;
        if (l <= len) {
            query[++cntq].l = l;
            query[cntq].r = min(r, len);
            query[cntq].id = i;
        }
    }
    prepare();
    compute();
    for (int i = 1; i <= m; i++) {
        cout << ans[i] << '\n';
    }
    return 0;
}
