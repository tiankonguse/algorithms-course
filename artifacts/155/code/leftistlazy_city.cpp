#include <bits/stdc++.h>

using namespace std;

const int MAXN = 300001;
int n, m;
long long defend[MAXN];
int belong[MAXN];
int type[MAXN];       
long long gain[MAXN];
long long attack[MAXN];
int first[MAXN];
int deep[MAXN];
int top[MAXN];
int sacrifice[MAXN];
int die[MAXN];
int ls[MAXN];
int rs[MAXN];
int dist[MAXN];
long long mul[MAXN];
long long add[MAXN];

void prepare() {
    dist[0] = -1;
    for (int i = 1; i <= m; i++) {
        ls[i] = rs[i] = dist[i] = 0;
        mul[i] = 1;
        add[i] = 0;
    }
    for (int i = 1; i <= n; i++) {
        sacrifice[i] = top[i] = 0;
    }
}

void upgrade(int i, int t, long long v) {
    if (t == 0) {
        attack[i] += v;
        add[i] += v;
    } else {
        attack[i] *= v;
        mul[i] *= v;
        add[i] *= v;
    }
}

void down(int i) {
    if (mul[i] != 1 || add[i] != 0) {
        int l = ls[i];
        int r = rs[i];
        if (l != 0) {
            attack[l] = attack[l] * mul[i] + add[i];
            mul[l] = mul[l] * mul[i];
            add[l] = add[l] * mul[i] + add[i];
        }
        if (r != 0) {
            attack[r] = attack[r] * mul[i] + add[i];
            mul[r] = mul[r] * mul[i];
            add[r] = add[r] * mul[i] + add[i];
        }
        mul[i] = 1;
        add[i] = 0;
    }
}

int merge(int i, int j) {
    if (i == 0 || j == 0) {
        return i + j;
    }
    if (attack[i] > attack[j]) {
        swap(i, j);
    }
    down(i);
    rs[i] = merge(rs[i], j);
    if (dist[ls[i]] < dist[rs[i]]) {
        swap(ls[i], rs[i]);
    }
    dist[i] = dist[rs[i]] + 1;
    return i;
}

int pop(int i) {
    down(i);
    int ans = merge(ls[i], rs[i]);
    ls[i] = rs[i] = dist[i] = 0;
    return ans;
}

void compute() {
    deep[1] = 1;
    for (int i = 2; i <= n; i++) {
        deep[i] = deep[belong[i]] + 1;
    }
    for (int i = 1; i <= m; i++) {
        if (top[first[i]] == 0) {
            top[first[i]] = i;
        } else {
            top[first[i]] = merge(top[first[i]], i);
        }
    }
    for (int i = n; i >= 1; i--) {
        while (top[i] != 0 && attack[top[i]] < defend[i]) {
            die[top[i]] = i;
            sacrifice[i]++;
            top[i] = pop(top[i]);
        }
        if (top[i] != 0) {
            upgrade(top[i], type[i], gain[i]);
            if (top[belong[i]] == 0) {
                top[belong[i]] = top[i];
            } else {
                top[belong[i]] = merge(top[belong[i]], top[i]);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    prepare();
    for (int i = 1; i <= n; i++) {
        cin >> defend[i];
    }
    for (int i = 2; i <= n; i++) {
        cin >> belong[i] >> type[i] >> gain[i];
    }
    for (int i = 1; i <= m; i++) {
        cin >> attack[i] >> first[i];
    }
    compute();
    for (int i = 1; i <= n; i++) {
        cout << sacrifice[i] << "\n";
    }
    for (int i = 1; i <= m; i++) {
        cout << deep[first[i]] - deep[die[i]] << endl;
    }
    return 0;
}
