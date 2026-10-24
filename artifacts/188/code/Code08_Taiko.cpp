#include <bits/stdc++.h>

using namespace std;

const int MAXN = 3001;
int n, k, m;

int cur[MAXN];
int path[MAXN];
int cntp;

void prepare(int len, int num) {
    n = len;
    k = num;
    m = 1;
    for (int i = 1; i <= n - 1; i++) {
        m *= k;
    }
    for (int i = 0; i < m; i++) {
        cur[i] = 0;
    }
    cntp = 0;
}

void euler(int u, int e) {
    while (cur[u] < k) {
        int ne = cur[u]++;
        euler((u * k + ne) % m, ne);
    }
    path[++cntp] = e;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int len;
    cin >> len;
    int num = 2;
    prepare(len, num);
    euler(0, -1);
    cout << (m * k) << " ";
    for (int i = 1; i <= n - 1; i++) {
        cout << "0";
    }
    for (int i = cntp - 1; i >= n; i--) {
        cout << path[i];
    }
    cout << "\n";
    return 0;
}
