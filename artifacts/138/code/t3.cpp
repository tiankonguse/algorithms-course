#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1001;
const double sml = 1e-6;

int n;
int X[MAXN], Y[MAXN], Z[MAXN];
double dist[MAXN][MAXN], cost[MAXN][MAXN];
bool vis[MAXN];
double value[MAXN];

double prim(double x) {
    for (int i = 1; i <= n; i++) {
        vis[i] = false;
        value[i] = cost[1][i] - x * dist[1][i];
    }
    vis[1] = true;
    double sum = 0;
    for (int i = 1; i <= n - 1; i++) {
        double minDist = 1e100;
        int next = 0;
        for (int j = 1; j <= n; j++) {
            if (!vis[j] && value[j] < minDist) {
                minDist = value[j];
                next = j;
            }
        }
        sum += minDist;
        vis[next] = true;
        for (int j = 1; j <= n; j++) {
            if (!vis[j] && value[j] > cost[next][j] - x * dist[next][j]) {
                value[j] = cost[next][j] - x * dist[next][j];
            }
        }
    }
    return sum;
}

int main() {
    while (scanf("%d", &n) == 1 && n != 0) {
        for (int i = 1; i <= n; i++) scanf("%d%d%d", &X[i], &Y[i], &Z[i]);
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (i != j) {
                    dist[i][j] = sqrt((double)(X[i] - X[j]) * (X[i] - X[j]) +
                                      (double)(Y[i] - Y[j]) * (Y[i] - Y[j]));
                    cost[i][j] = abs(Z[i] - Z[j]);
                }
            }
        }
        double l = 0, r = 100, ans = 0;
        while (l < r && r - l >= sml) {
            double x = (l + r) / 2;
            if (prim(x) <= 0) {
                ans = x;
                r = x - sml;
            } else {
                l = x + sml;
            }
        }
        printf("%.3f\n", ans);
    }
    return 0;
}
