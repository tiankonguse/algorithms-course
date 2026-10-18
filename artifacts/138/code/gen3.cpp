#include <bits/stdc++.h>
using namespace std;

mt19937 rng(98765);

int main() {
    int T = 1500;
    for (int t = 0; t < T; t++) {
        int n = 2 + rng() % 5;  // 2..6
        vector<pair<int,int>> pts;
        while ((int)pts.size() < n) {
            pair<int,int> p = {(int)(rng() % 21), (int)(rng() % 21)};
            int dup = 0;
            for (auto& q : pts) if (q == p) { dup = 1; break; }
            if (!dup) pts.push_back(p);
        }
        printf("%d\n", n);
        for (int i = 0; i < n; i++) {
            printf("%d %d %d\n", pts[i].first, pts[i].second, (int)(rng() % 51));
        }
    }
    printf("0\n");
    return 0;
}
