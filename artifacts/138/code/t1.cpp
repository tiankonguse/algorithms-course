#include <bits/stdc++.h>
using namespace std;

const double sml = 1e-6;

struct Node {
    double a, b, m;
};

vector<Node> arr;
int n, k;

bool check(double x) {
    for (int i = 1; i <= n; i++) arr[i].m = arr[i].a - x * arr[i].b;
    sort(arr.begin() + 1, arr.end(), [](const Node& u, const Node& v) {
        return u.m > v.m;
    });
    double sum = 0;
    for (int i = 1; i <= k; i++) sum += arr[i].m;
    return sum >= 0;
}

int main() {
    while (scanf("%d%d", &n, &k) == 2 && (n || k)) {
        arr.resize(n + 1);
        k = n - k;  // 舍弃 k 个 = 保留 n-k 个
        for (int i = 1; i <= n; i++) scanf("%lf", &arr[i].a);
        for (int i = 1; i <= n; i++) scanf("%lf", &arr[i].b);
        double l = 0, r = 0, ans = 0;
        for (int i = 1; i <= n; i++) r += arr[i].a;
        while (l < r && r - l >= sml) {
            double x = (l + r) / 2;
            if (check(x)) {
                ans = x;
                l = x + sml;
            } else {
                r = x - sml;
            }
        }
        printf("%d\n", (int)(100 * (ans + 0.005)));
    }
    return 0;
}
