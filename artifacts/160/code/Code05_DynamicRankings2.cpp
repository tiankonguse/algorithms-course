#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;
const int MAXT = MAXN * 130;
int n, m, s;
int arr[MAXN];
int ques[MAXN][4];
int sorted[MAXN * 2];
int root[MAXN];
int sum[MAXT];
int ls[MAXT];
int rs[MAXT];
int cntt = 0;
int addTree[MAXN];
int minusTree[MAXN];
int cntadd;
int cntminus;

int kth(int num) {
    int l = 1, r = s, mid;
    while (l <= r) {
        mid = (l + r) / 2;
        if (sorted[mid] == num) {
            return mid;
        } else if (sorted[mid] < num) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    return -1;
}

int lowbit(int i) {
    return i & -i;
}

int innerAdd(int jobi, int jobv, int l, int r, int i) {
    if (i == 0) {
        i = ++cntt;
    }
    if (l == r) {
    	sum[i] += jobv;
    } else {
        int mid = (l + r) / 2;
        if (jobi <= mid) {
        	ls[i] = innerAdd(jobi, jobv, l, mid, ls[i]);
        } else {
        	rs[i] = innerAdd(jobi, jobv, mid + 1, r, rs[i]);
        }
        sum[i] = sum[ls[i]] + sum[rs[i]];
    }
    return i;
}

int innerQuery(int jobk, int l, int r) {
    if (l == r) {
        return l;
    }
    int mid = (l + r) / 2;
    int leftsum = 0;
    for (int i = 1; i <= cntadd; i++) {
        leftsum += sum[ls[addTree[i]]];
    }
    for (int i = 1; i <= cntminus; i++) {
        leftsum -= sum[ls[minusTree[i]]];
    }
    if (jobk <= leftsum) {
        for (int i = 1; i <= cntadd; i++) {
            addTree[i] = ls[addTree[i]];
        }
        for (int i = 1; i <= cntminus; i++) {
            minusTree[i] = ls[minusTree[i]];
        }
        return innerQuery(jobk, l, mid);
    } else {
        for (int i = 1; i <= cntadd; i++) {
            addTree[i] = rs[addTree[i]];
        }
        for (int i = 1; i <= cntminus; i++) {
            minusTree[i] = rs[minusTree[i]];
        }
        return innerQuery(jobk - leftsum, mid + 1, r);
    }
}

void add(int i, int cnt) {
    for (int j = i; j <= n; j += lowbit(j)) {
        root[j] = innerAdd(arr[i], cnt, 1, s, root[j]);
    }
}

void update(int i, int v) {
    add(i, -1);
    arr[i] = kth(v);
    add(i, 1);
}

int number(int l, int r, int k) {
    cntadd = cntminus = 0;
    for (int i = r; i > 0; i -= lowbit(i)) {
        addTree[++cntadd] = root[i];
    }
    for (int i = l - 1; i > 0; i -= lowbit(i)) {
        minusTree[++cntminus] = root[i];
    }
    return sorted[innerQuery(k, 1, s)];
}

void prepare() {
    s = 0;
    for (int i = 1; i <= n; i++) {
    	sorted[++s] = arr[i];
    }
    for (int i = 1; i <= m; i++) {
        if (ques[i][0] == 2) {
        	sorted[++s] = ques[i][2];
        }
    }
    sort(sorted + 1, sorted + s + 1);
    int len = 1;
    for (int i = 2; i <= s; i++) {
        if (sorted[len] != sorted[i]) {
        	sorted[++len] = sorted[i];
        }
    }
    s = len;
    for (int i = 1; i <= n; i++) {
        arr[i] = kth(arr[i]);
        add(i, 1);
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
        string op;
        cin >> op;
        if (op == "Q") {
            ques[i][0] = 1;
        } else {
            ques[i][0] = 2;
        }
        cin >> ques[i][1];
        cin >> ques[i][2];
        if (ques[i][0] == 1) {
            cin >> ques[i][3];
        }
    }
    prepare();
    for (int i = 1, op, x, y, z; i <= m; i++) {
        op = ques[i][0];
        x = ques[i][1];
        y = ques[i][2];
        if (op == 1) {
            z = ques[i][3];
            cout << number(x, y, z) << "\n";
        } else {
            update(x, y);
        }
    }
    return 0;
}
