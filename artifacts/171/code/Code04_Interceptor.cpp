#include <bits/stdc++.h>

using namespace std;

struct Node {
    int i, h, v;
};

bool Cmp1(Node a, Node b) {
    return a.h > b.h;
}

bool Cmp2(Node a, Node b) {
    return a.h < b.h;
}

const int MAXN = 50001;
int n, s;
int h[MAXN];
int v[MAXN];
int sortv[MAXN];

Node arr[MAXN];

int treeVal[MAXN];
double treeCnt[MAXN];

int len1[MAXN];
double cnt1[MAXN];

int len2[MAXN];
double cnt2[MAXN];

int lowbit(int i) {
    return i & -i;
}

void more(int i, int val, double cnt) {
    while (i <= s) {
        if (val > treeVal[i]) {
            treeVal[i] = val;
            treeCnt[i] = cnt;
        } else if (val == treeVal[i]) {
            treeCnt[i] += cnt;
        }
        i += lowbit(i);
    }
}

int queryVal;
double queryCnt;

void query(int i) {
    queryVal = 0;
    queryCnt = 0;
    while (i > 0) {
        if (treeVal[i] > queryVal) {
            queryVal = treeVal[i];
            queryCnt = treeCnt[i];
        } else if (treeVal[i] == queryVal) {
            queryCnt += treeCnt[i];
        }
        i -= lowbit(i);
    }
}

void clear(int i) {
    while (i <= s) {
        treeVal[i] = 0;
        treeCnt[i] = 0;
        i += lowbit(i);
    }
}

void merge1(int l, int m, int r) {
    for (int i = l; i <= r; i++) {
        arr[i].i = i;
        arr[i].h = h[i];
        arr[i].v = v[i];
    }
    sort(arr + l, arr + m + 1, Cmp1);
    sort(arr + m + 1, arr + r + 1, Cmp1);
    int p1, p2;
    for (p1 = l - 1, p2 = m + 1; p2 <= r; p2++) {
        while (p1 + 1 <= m && arr[p1 + 1].h >= arr[p2].h) {
            p1++;
            more(s - arr[p1].v + 1, len1[arr[p1].i], cnt1[arr[p1].i]);
        }
        query(s - arr[p2].v + 1);
        if (queryVal + 1 > len1[arr[p2].i]) {
            len1[arr[p2].i] = queryVal + 1;
            cnt1[arr[p2].i] = queryCnt;
        } else if (queryVal + 1 == len1[arr[p2].i]) {
            cnt1[arr[p2].i] += queryCnt;
        }
    }
    for (int i = l; i <= p1; i++) {
        clear(s - arr[i].v + 1);
    }
}

void cdq1(int l, int r) {
    if (l == r) {
        return;
    }
    int m = (l + r) / 2;
    cdq1(l, m);
    merge1(l, m, r);
    cdq1(m + 1, r);
}

void merge2(int l, int m, int r) {
    for (int i = l; i <= r; i++) {
        arr[i].i = i;
        arr[i].h = h[i];
        arr[i].v = v[i];
    }
    sort(arr + l, arr + m + 1, Cmp2);
    sort(arr + m + 1, arr + r + 1, Cmp2);
    int p1, p2;
    for (p1 = l - 1, p2 = m + 1; p2 <= r; p2++) {
        while (p1 + 1 <= m && arr[p1 + 1].h <= arr[p2].h) {
            p1++;
            more(arr[p1].v, len2[arr[p1].i], cnt2[arr[p1].i]);
        }
        query(arr[p2].v);
        if (queryVal + 1 > len2[arr[p2].i]) {
            len2[arr[p2].i] = queryVal + 1;
            cnt2[arr[p2].i] = queryCnt;
        } else if (queryVal + 1 == len2[arr[p2].i]) {
            cnt2[arr[p2].i] += queryCnt;
        }
    }
    for (int i = l; i <= p1; i++) {
        clear(arr[i].v);
    }
}

void cdq2(int l, int r) {
    if (l == r) {
        return;
    }
    int m = (l + r) / 2;
    cdq2(l, m);
    merge2(l, m, r);
    cdq2(m + 1, r);
}

int lower(int num) {
    int l = 1, r = s, ans = 1;
    while (l <= r) {
        int mid = (l + r) / 2;
        if (sortv[mid] >= num) {
            ans = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return ans;
}

void prepare() {
    for (int i = 1; i <= n; i++) {
        sortv[i] = v[i];
    }
    sort(sortv + 1, sortv + n + 1);
    s = 1;
    for (int i = 2; i <= n; i++) {
        if (sortv[s] != sortv[i]) {
            sortv[++s] = sortv[i];
        }
    }
    for (int i = 1; i <= n; i++) {
        v[i] = lower(v[i]);
    }
    for (int i = 1; i <= n; i++) {
        len1[i] = len2[i] = 1;
        cnt1[i] = cnt2[i] = 1.0;
    }
}

void compute() {
    cdq1(1, n);
    for (int l = 1, r = n; l < r; l++, r--) {
        swap(h[l], h[r]);
        swap(v[l], v[r]);
    }
    cdq2(1, n);
    for (int l = 1, r = n; l < r; l++, r--) {
        swap(len2[l], len2[r]);
        swap(cnt2[l], cnt2[r]);
    }
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d%d", &h[i], &v[i]);
    }
    prepare();
    compute();
    int len = 0;
    double cnt = 0.0;
    for (int i = 1; i <= n; i++) {
        len = max(len, len1[i]);
    }
    for (int i = 1; i <= n; i++) {
        if (len1[i] == len) {
            cnt += cnt1[i];
        }
    }
    printf("%d\n", len);
    for (int i = 1; i <= n; i++) {
        if (len1[i] + len2[i] - 1 < len) {
            printf("0 ");
        } else {
            printf("%.5f ", cnt1[i] * cnt2[i] / cnt);
        }
    }
    printf("\n");
    return 0;
}
