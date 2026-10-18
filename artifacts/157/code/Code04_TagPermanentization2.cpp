#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 100001;
long long arr[MAXN];
long long sum[MAXN << 2];
long long addTag[MAXN << 2];

void build(int l, int r, int i) {
    if (l == r) {
        sum[i] = arr[l];
    } else {
        int mid = (l + r) / 2;
        build(l, mid, i << 1);
        build(mid + 1, r, i << 1 | 1);
        sum[i] = sum[i << 1] + sum[i << 1 | 1];
    }
    addTag[i] = 0;
}

void add(int jobl, int jobr, long long jobv, int l, int r, int i) {
    int a = max(jobl, l), b = min(jobr, r);
    sum[i] += jobv * (b - a + 1);
    if (jobl <= l && r <= jobr) {
        addTag[i] += jobv;
    } else {
        int mid = (l + r) / 2;
        if (jobl <= mid) {
            add(jobl, jobr, jobv, l, mid, i << 1);
        }
        if (jobr > mid) {
            add(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }
    }
}

long long query(int jobl, int jobr, long long addHistory, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return sum[i] + addHistory * (r - l + 1);
    }
    int mid = (l + r) >> 1;
    long long ans = 0;
    if (jobl <= mid) {
        ans += query(jobl, jobr, addHistory + addTag[i], l, mid, i << 1);
    }
    if (jobr > mid) {
        ans += query(jobl, jobr, addHistory + addTag[i], mid + 1, r, i << 1 | 1);
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    build(1, n, 1);
    int op, jobl, jobr;
    long long jobv;
    for (int i = 1; i <= m; i++) {
        cin >> op;
        if (op == 1) {
            cin >> jobl >> jobr >> jobv;
            add(jobl, jobr, jobv, 1, n, 1);
        } else {
            cin >> jobl >> jobr;
            cout << query(jobl, jobr, 0, 1, n, 1) << "\n";
        }
    }
    return 0;
}
