#include <bits/stdc++.h>
using namespace std;

mt19937 rng(24680);

int main() {
    int T = 2000;
    for (int t = 0; t < T; t++) {
        int n = 2 + rng() % 5;        // 2..6
        // 先强制一个环 1->2->...->c->1
        int c = 2 + rng() % (n - 1);  // 环长 2..n
        set<pair<int,int>> es;
        for (int i = 1; i < c; i++) es.insert({i, i + 1});
        es.insert({c, 1});
        // 再随机加边
        int extra = rng() % 7;
        for (int i = 0; i < extra; i++) {
            int u = 1 + rng() % n, v = 1 + rng() % n;
            es.insert({u, v});
        }
        int m = es.size();
        printf("%d %d\n", n, m);
        for (auto& e : es) {
            int u = e.first, v = e.second;
            int w = (int)(rng() % 21) - 10;  // -10..10
            printf("%d %d %d\n", u, v, w);
        }
    }
    return 0;
}
