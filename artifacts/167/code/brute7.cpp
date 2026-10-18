#include <bits/stdc++.h>
using namespace std;
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int q; cin >> q;
    multiset<int> s;
    for (int i = 0; i < q; i++) {
        int op; cin >> op;
        if (op == 1) { int x; cin >> x; s.insert(x); }
        else if (op == 2) { int x; cin >> x; s.erase(s.find(x)); }
        else {
            int best = INT_MAX;
            vector<int> v(s.begin(), s.end());
            for (size_t a = 0; a < v.size(); a++)
                for (size_t b = a + 1; b < v.size(); b++)
                    best = min(best, v[a] ^ v[b]);
            cout << best << '\n';
        }
    }
    return 0;
}
