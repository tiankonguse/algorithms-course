// 题2 P哥的桶：课程（线段树 + 线性基） vs 暴力（桶数组 + 区间合并线性基）
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int BIT2 = 30;

struct SegTree {
  static const int MAXN = 50001;
  int treeBasis[MAXN << 2][BIT2 + 1];
  int basis[BIT2 + 1];
  bool insert(int* bs, int num) {
    for (int i = BIT2; i >= 0; i--) {
      if (num >> i == 1) {
        if (bs[i] == 0) { bs[i] = num; return true; }
        num ^= bs[i];
      }
    }
    return false;
  }
  void add(int jobi, int jobv, int l, int r, int i) {
    insert(treeBasis[i], jobv);
    if (l < r) {
      int mid = (l + r) >> 1;
      if (jobi <= mid) add(jobi, jobv, l, mid, i << 1);
      else add(jobi, jobv, mid + 1, r, i << 1 | 1);
    }
  }
  void merge(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
      for (int j = BIT2; j >= 0; j--) if (treeBasis[i][j] != 0) insert(basis, treeBasis[i][j]);
    } else {
      int mid = (l + r) >> 1;
      if (jobl <= mid) merge(jobl, jobr, l, mid, i << 1);
      if (jobr > mid) merge(jobl, jobr, mid + 1, r, i << 1 | 1);
    }
  }
  int query(int jobl, int jobr, int m) {
    memset(basis, 0, sizeof(basis));
    merge(jobl, jobr, 1, m, 1);
    int ans = 0;
    for (int j = BIT2; j >= 0; j--) ans = max(ans, ans ^ basis[j]);
    return ans;
  }
  void reset() {
    memset(treeBasis, 0, sizeof(treeBasis));
  }
};

// 全局静态：treeBasis 约 25MB，放栈上会溢出
SegTree st;

int bruteMax(const vector<vector<int>>& bucket, int l, int r, int n) {
  int basis[BIT2 + 1];
  memset(basis, 0, sizeof(basis));
  for (int k = max(1, l); k <= min(r, n); k++) {
    for (int v : bucket[k]) {
      int num = v;
      for (int i = BIT2; i >= 0; i--) {
        if (num >> i == 1) {
          if (basis[i] == 0) { basis[i] = num; break; }
          num ^= basis[i];
        }
      }
    }
  }
  int ans = 0;
  for (int i = BIT2; i >= 0; i--) ans = max(ans, ans ^ basis[i]);
  return ans;
}

int main() {
  mt19937 rng(987654321);
  int fails = 0;
  for (int T = 0; T < 3000; T++) {
    int n = 1 + rng() % 8;                 // 桶数
    int m = n + rng() % 20;                // 操作数（≥ n，线段树范围 1..m 覆盖 1..n）
    st.reset();
    vector<vector<int>> bucket(n + 1);
    for (int opi = 1; opi <= m; opi++) {
      int op = 1 + rng() % 2;
      if (op == 1) {
        int k = 1 + rng() % n;             // 桶号 1..n
        int v = rng() % 16;                // 值域小，容易碰撞
        st.add(k, v, 1, m, 1);
        bucket[k].push_back(v);
      } else {
        int l = 1 + rng() % n, r = l + rng() % (n - l + 1);
        int c = st.query(l, r, m);
        int b = bruteMax(bucket, l, r, n);
        if (c != b) {
          fails++;
          printf("MISMATCH T=%d op2 l=%d r=%d 课程=%d 暴力=%d\n", T, l, r, c, b);
          if (fails > 3) return 1;
        }
      }
    }
  }
  if (fails) return 1;
  printf("3000 组 ALL OK\n");
  return 0;
}
