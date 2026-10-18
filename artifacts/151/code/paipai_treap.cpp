// treap2 P3369 对拍：随机六操作 vs multiset
// 生成输入 → 运行 ./treap2 → 对比 multiset 输出
#include <bits/stdc++.h>
using namespace std;

int main() {
    srand(20261018);
    int total = 0, fails = 0;
    for (int round = 0; round < 400; round++) {
        int steps = 300;
        vector<pair<int,int>> ops; // (op, x)
        for (int i = 0; i < steps; i++) {
            int op = 1 + rand() % 6;
            int x = rand() % 101 - 50; // -50..50
            ops.push_back({op, x});
        }
        // 写输入
        FILE* in = fopen("treap_in.txt", "w");
        fprintf(in, "%d\n", steps);
        for (auto& o : ops) fprintf(in, "%d %d\n", o.first, o.second);
        fclose(in);
        // 跑课程程序
        system("./treap2 < treap_in.txt > treap_out.txt");
        ifstream out("treap_out.txt");
        vector<long long> got;
        long long v;
        while (out >> v) got.push_back(v);
        // multiset 暴力
        multiset<int> ms;
        vector<long long> exp;
        for (auto& o : ops) {
            int op = o.first, x = o.second;
            if (op == 1) ms.insert(x);
            else if (op == 2) {
                auto it = ms.find(x);
                if (it != ms.end()) ms.erase(it);
            } else if (op == 3) {
                exp.push_back((long long)(distance(ms.begin(), ms.lower_bound(x)) + 1));
            } else if (op == 4) {
                auto it = ms.begin();
                advance(it, x - 1);
                exp.push_back(*it);
            } else if (op == 5) {
                auto it = ms.lower_bound(x);
                if (it == ms.begin()) exp.push_back(INT_MIN);
                else { --it; exp.push_back(*it); }
            } else {
                auto it = ms.upper_bound(x);
                if (it == ms.end()) exp.push_back(INT_MAX);
                else exp.push_back(*it);
            }
        }
        total++;
        if (got != exp) {
            fails++;
            if (fails <= 3) {
                printf("FAIL round=%d got=%zu exp=%zu\n", round, got.size(), exp.size());
                for (size_t i = 0; i < min(got.size(), exp.size()); i++)
                    if (got[i] != exp[i]) { printf("  first diff idx=%zu got=%lld exp=%lld\n", i, got[i], exp[i]); break; }
            }
        }
    }
    printf("== treap2: total %d cases, fails %d ==\n", total, fails);
    return 0;
}
