#include <iostream>
#include <vector>
#include <stack>
#include <cstdio>

#define LL long long

using namespace std;

const int MAXN = 10000001;

int arr[MAXN];
int ls[MAXN];
int rs[MAXN];
int sta[MAXN];
int n;

void build() {
    int top = 0;
    for (int i = 1; i <= n; i++) {
        int pos = top;
        while (pos > 0 && arr[sta[pos]] > arr[i]) {
            pos--;
        }
        if (pos > 0) {
            rs[sta[pos]] = i;
        }
        if (pos < top) {
            ls[i] = sta[pos + 1];
        }
        sta[++pos] = i;
        top = pos;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }
    build();
    long long ans1 = 0, ans2 = 0;
    for (int i = 1; i <= n; i++) {
        ans1 ^= 1LL * i * (ls[i] + 1);
        ans2 ^= 1LL * i * (rs[i] + 1);
    }
    cout << ans1 << " " << ans2 << endl;
    return 0;
}
