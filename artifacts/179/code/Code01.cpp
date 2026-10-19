#include <bits/stdc++.h>

using namespace std;

struct Query {
    int l, r, id;
};

const int MAXN = 60002;
const int MAXV = 1 << 26;
int n, m;
int arr[MAXN];
Query query[MAXN];

int bi[MAXN];
int cnt[MAXV];
long long curAns = 0;
long long ans[MAXN];

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

void add(int s) {
    curAns += cnt[s];
    cnt[s]++;
    for (int i = 0; i < 26; i++) {
        curAns += cnt[s ^ (1 << i)];
    }
}

void del(int s) {
    cnt[s]--;
    curAns -= cnt[s];
    for (int i = 0; i < 26; i++) {
        curAns -= cnt[s ^ (1 << i)];
    }
}

void compute() {
    int winl = 1, winr = 0;
    for (int i = 1; i <= m; i++) {
        int jobl = query[i].l;
        int jobr = query[i].r;
        int id = query[i].id;
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
    for (int i = 1; i <= n; i++) {
        arr[i] ^= arr[i - 1];
    }
    for (int i = n; i >= 0; i--) {
        arr[i + 1] = arr[i];
    }
    n++;
    int blen = (int)sqrt(n);
    for (int i = 1; i <= n; i++) {
        bi[i] = (i - 1) / blen + 1;
    }
    for (int i = 1; i <= m; i++) {
        query[i].r++;
    }
    sort(query + 1, query + m + 1, QueryCmp);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    char c;
    for (int i = 1; i <= n; i++) {
        cin >> c;
        arr[i] = 1 << (c - 'a');
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
