#define main real_main
#include "/tmp/cpp145/paipai_all.cpp"
#undef main
#include <map>
// 打印 gm_course 与 gm_brute 的差异 + 树结构
int main() {
    mt19937 rng(145);
    int bad = 0;
    for (int it = 0; it < 600; it++) {
        int n = 2 * (rng() % 3 + 1);
        int m = n >> 1;
        vector<pair<int,int>> edges;
        for (int i = 2; i <= n; i++) edges.push_back({rng() % (i - 1) + 1, i});
        vector<int> arr(n + 1);
        vector<int> tmp;
        for (int i = 0; i < m; i++) tmp.push_back(0);
        for (int i = 0; i < m; i++) tmp.push_back(1);
        shuffle(tmp.begin(), tmp.end(), rng);
        for (int i = 1; i <= n; i++) arr[i] = tmp[i - 1];
        auto f1 = gm_course(n, arr, edges);
        auto f2 = gm_brute(n, arr, edges);
        bool ok = true;
        for (int k = 0; k <= m; k++) if (f1[k] != f2[k]) { ok = false; break; }
        if (!ok && bad < 6) {
            bad++;
            printf("=== n=%d arr=", n);
            for (int i = 1; i <= n; i++) printf("%d", arr[i]);
            printf("\n  边: ");
            for (auto& e : edges) printf("(%d,%d) ", e.first, e.second);
            printf("\n  course=[");
            for (int k = 0; k <= m; k++) printf("%lld ", f1[k]);
            printf("] brute=[");
            for (int k = 0; k <= m; k++) printf("%lld ", f2[k]);
            printf("]\n");
        }
    }
    // 固定链树 1-2-3-4-5-6, arr=010011
    {
        int n = 6;
        vector<pair<int,int>> edges;
        for (int i = 2; i <= n; i++) edges.push_back({i-1, i});
        vector<int> arr = {0,0,1,0,0,1,1};
        auto f1 = gm_course(n, arr, edges);
        auto f2 = gm_brute(n, arr, edges);
        printf("链树 n=6 arr=010011: course=[");
        for (int k = 0; k <= 3; k++) printf("%lld ", f1[k]);
        printf("] brute=[");
        for (int k = 0; k <= 3; k++) printf("%lld ", f2[k]);
        printf("]\n");
    }
    return 0;
}
