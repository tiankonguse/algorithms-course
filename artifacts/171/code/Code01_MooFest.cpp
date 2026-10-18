#include <bits/stdc++.h>

using namespace std;

struct Node {
    int v, x;
};

bool NodeCmp(Node a, Node b) {
    return a.v < b.v;
}

const int MAXN = 50001;
int n;
Node arr[MAXN];
Node tmp[MAXN];

long long merge(int l, int m, int r) {
    int p1, p2;
    long long rsum = 0, lsum = 0, ans = 0;
    for (p1 = l; p1 <= m; p1++) {
        rsum += arr[p1].x;
    }
    for (p1 = l - 1, p2 = m + 1; p2 <= r; p2++) {
        while (p1 + 1 <= m && arr[p1 + 1].x < arr[p2].x) {
            p1++;
            rsum -= arr[p1].x;
            lsum += arr[p1].x;
        }
        ans += (1LL * (p1 - l + 1) * arr[p2].x - lsum + rsum - 1LL * (m - p1) * arr[p2].x) * arr[p2].v;
    }
    p1 = l;
    p2 = m + 1;
    int i = l;
    while (p1 <= m && p2 <= r) {
        tmp[i++] = arr[p1].x <= arr[p2].x ? arr[p1++] : arr[p2++];
    }
    while (p1 <= m) {
        tmp[i++] = arr[p1++];
    }
    while (p2 <= r) {
        tmp[i++] = arr[p2++];
    }
    for (i = l; i <= r; i++) {
        arr[i] = tmp[i];
    }
    return ans;
}

long long cdq(int l, int r) {
    if (l == r) {
        return 0;
    }
    int mid = (l + r) / 2;
    return cdq(l, mid) + cdq(mid + 1, r) + merge(l, mid, r);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i].v >> arr[i].x;
    }
    sort(arr + 1, arr + n + 1, NodeCmp);
    cout << cdq(1, n) << '\n';
    return 0;
}
