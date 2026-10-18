#include <bits/stdc++.h>
using namespace std;
const int MAXN = 10000001;
int arr[MAXN], ls[MAXN], rs[MAXN], sta[MAXN], n;
void build() {
    int top = 0;
    for (int i = 1; i <= n; i++) {
        int pos = top;
        while (pos > 0 && arr[sta[pos]] > arr[i]) pos--;
        if (pos > 0) rs[sta[pos]] = i;
        if (pos < top) ls[i] = sta[pos + 1];
        sta[++pos] = i;
        top = pos;
    }
}
int main() {
    n = 10000000;
    srand(2026);
    for (int i = 1; i <= n; i++) arr[i] = rand();
    auto t0 = chrono::steady_clock::now();
    build();
    auto t1 = chrono::steady_clock::now();
    printf("descartes n=1e7 建树: %.3fs root=%d\n", chrono::duration<double>(t1-t0).count(), sta[1]);
    return 0;
}
